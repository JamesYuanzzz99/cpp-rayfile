#include "mytcpsocket.h"
#include <QDebug>
#include <protocol.h>
#include <operatedb.h>
#include <QStringList>
#include <mytcpserver.h>
#include <QFileInfoList>
#pragma execution_character_set("utf-8")

MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{
    connect(this, &MyTcpSocket::readyRead, this, &MyTcpSocket::recvMsg);

    connect(this, &MyTcpSocket::disconnected, this, &MyTcpSocket::clientOffline);

    uplStatus = false;

    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &MyTcpSocket::sendFileToClient);
}

QString MyTcpSocket::getName()
{
    return username;
}

void MyTcpSocket::copyDir(QString srcDir, QString destDir)
{
    QDir dir;
    dir.mkdir(destDir);

    dir.setPath(srcDir);
    QFileInfoList fileInfoList = dir.entryInfoList();


    QString srcTmp;
    QString destTmp;
    for(int i = 0; i < fileInfoList.size(); i++)
    {
        if(fileInfoList[i].isFile())
        {
            srcTmp = srcDir + "/" + fileInfoList[i].fileName();
            destTmp = destDir + "/" + fileInfoList[i].fileName();
            QFile::copy(srcTmp, destTmp);
        }
        else if(fileInfoList[i].isDir())
        {
            if(QString(".") == fileInfoList[i].fileName()
               || QString("..") == fileInfoList[i].fileName())
            {
                continue;
            }

            srcTmp = srcDir + "/" + fileInfoList[i].fileName();
            destTmp = destDir + "/" + fileInfoList[i].fileName();
            copyDir(srcTmp, destTmp);
        }
    }
}

void MyTcpSocket::recvMsg()
{
    if(!uplStatus)              //不是上传文件状态就采用PDU协议处理
    {
        qDebug() << this->bytesAvailable();
        uint uiPDULen = 0;
        this->read((char*)&uiPDULen, sizeof(uint));

        uint uiMsgLen = uiPDULen - sizeof(PDU);
        PDU *pdu = mkPDU(uiMsgLen);
        this->read((char*)pdu+sizeof(uint), uiPDULen-sizeof(uint));
        //qDebug() << pdu->uiMsgType << (char*)(pdu->caMsg);
        //qDebug() << "服务器收到的pdu的caMsg的大小为:"<<uiMsgLen;
        //qDebug() << "服务器收到的caMsg消息为: "<< pdu->caMsg;

        switch(pdu->uiMsgType) {
        case LOGIN_REQUEST:            //登录请求
        {
            char name[32] = {'\0'};
            char pwd[32] = {'\0'};
            strncpy(name, pdu->caData, 32);        //读取出用户名
            strncpy(pwd, pdu->caData+32, 32);       //读取出密码
            bool ret = OperateDb::getInstance().handleLogin(name, pwd);     //执行查询sql
            PDU *res = mkPDU(0);
            res->uiMsgType = LOGIN_RESPOND;
            if(ret)
            {
                strcpy(res->caData, LOGIN_OK);
                username = name;
            }
            else
            {
                strcpy(res->caData, LOGIN_FAILED);
            }
            write((char*)res, res->uiPDULen);        //回发登录是否成功
            //释放res
            free(res);
            res = NULL;
            break;
        }
        case REGIST_REQUEST:               //是注册请求
        {
            char name[32] = {'\0'};
            char pwd[32] = {'\0'};
            strncpy(name, pdu->caData, 32);        //读取出用户名
            strncpy(pwd, pdu->caData+32, 32);       //读取出密码
            bool ret = OperateDb::getInstance().handleRegist(name, pwd);     //执行插入sql
            PDU *res = mkPDU(0);
            res->uiMsgType = REGIST_RESPOND;
            if(ret)
            {
                strcpy(res->caData, RESGIST_OK);

                //给新用户在服务器根目录创建一个用户文件夹
                QDir dir;
                qDebug()<< "创建文件夹:"<<dir.mkdir(QString("%1/%2").arg(SERV_DIR).arg(name));
            }
            else
            {
                strcpy(res->caData, RESGIST_FAILED);
            }
            write((char*)res, res->uiPDULen);        //回发注册是否成功
            //释放res
            free(res);
            res = NULL;
            break;
        }
        case ALLONLINE_REQUEST:             //在线用户请求
        {
            QStringList ret = OperateDb::getInstance().handleAllOnline();
            uint uiMsgLen = ret.size() * 32;
            PDU *res = mkPDU(uiMsgLen);
            res->uiMsgType = ALLONLINE_RESPOND;
            for(int i = 0; i < ret.size(); i++)
            {
                //参数1 目标地址   参数2 被拷贝的对象的首地址  参数3 被拷贝的对象的大小
                //必须先转化为char*,这样才是表示地址的偏移
                //memcpy((res->caMsg)+i*32, ret.at(i).toStdString().c_str(), ret.at(i).size());
                strncpy(res->caMsg + i*32, ret.at(i).toStdString().c_str(), 32);
                //qDebug() << "服务器发送在线用户前:"<<ret.at(i);
                //qDebug() << "服务器发送在线用户后:"<<res->caMsg;
            }

            write((char*)res, res->uiPDULen);        //回发在线用户回复
            //释放res
            free(res);
            res = NULL;
            break;
        }

        //查找请求
        case SEARCHUSER_REQUEST:
        {
            int ret = OperateDb::getInstance().handleSearchUser(pdu->caData);

            PDU *res = mkPDU(0);
            res->uiMsgType = SEARCHUSER_RESPOND;
            if(-1 == ret)
            {
                strcpy(res->caData, SEARCH_USER_NO);
            }
            else if(ret == 1)
            {
                strcpy(res->caData, SEARCH_USER_ONLINE);
            }
            else if(ret == 0)
            {
                strcpy(res->caData, SEARCH_USER_OFFLINE);
            }

            write((char*)res, res->uiPDULen);        //回发查找用户回复
            //释放res
            free(res);
            res = NULL;
            break;
        }

        //添加好友请求
        case ADDFRIEND_REQUEST:
        {
            char addFreienName[32] = {'\0'};
            char loginName[32] = {'\0'};
            strncpy(addFreienName, pdu->caData, 32);        //读取需要添加好友的用户名
            strncpy(loginName, pdu->caData+32, 32);       //读取自己的用户名
            int ret = OperateDb::getInstance().handleAddFriend(addFreienName, loginName);
            //qDebug() << "添加好友的ret: "<<ret;

            PDU *res = mkPDU(0);
            if(ret == -2)              //参数错误
            {
                res->uiMsgType = ADDFRIEND_RESPOND;
                strcpy(res->caData, UNKNOW_ERROR);
                write((char*)res, res->uiPDULen);
            }
            else if(ret == 0)           //不在线
            {
                res->uiMsgType = ADDFRIEND_RESPOND;
                strcpy(res->caData, ADDFRIEND_OFFLINE);
                write((char*)res, res->uiPDULen);
            }
            else if(ret == 1)           //在线
            {
                //qDebug()<<"进到了添加好友且在线";
                MyTcpServer::getInstance().sendAddFriend(addFreienName, pdu);
            }
            else if(ret == 2)     //双方已经是好友
            {
                res->uiMsgType = ADDFRIEND_RESPOND;
                strcpy(res->caData, ADDFRIEND_EXIST);
                write((char*)res, res->uiPDULen);
            }
            else if(ret == -1)      //不存在
            {
                res->uiMsgType = ADDFRIEND_RESPOND;
                strcpy(res->caData, ADDFRIEND_NOEXIST);
                write((char*)res, res->uiPDULen);
            }
            //qDebug() << res->caData;
            //释放res
            free(res);
            res = NULL;
            break;
        }


        case ADDFRIEND_AGREE:             //同意添加好友
        {
            char addFriendName[32] = {'\0'};
            char loginName[32] = {'\0'};
            strncpy(addFriendName, pdu->caData, 32);
            strncpy(loginName, pdu->caData+32, 32);
            qDebug()<<"服务器执行插入前的loginName:" <<loginName;

            bool ret = OperateDb::getInstance().handleAddFriendAgree(addFriendName, loginName);
            if(ret)
            {
                qDebug() <<"添加好友成功";
            }
            else
            {
                qDebug() << "添加好友失败";
            }
            break;
        }

        case ADDFRIEND_REFUSE:             //拒绝添加好友
        {
            break;
        }

        case FLUSHFRIEND_REQUEST:       //刷新好友请求
        {
            char name[32] = {'\0'};
            strncpy(name, pdu->caData, 32);
            QStringList ret =  OperateDb::getInstance().handleFlushFriend(name);

            PDU *res = mkPDU(ret.size() * 32);
            res->uiMsgType = FLUSHFRIEND_RESPOND;
            for(int i = 0; i < ret.size(); i++)
            {
                strncpy((res->caMsg)+i*32, ret.at(i).toStdString().c_str(), 32);
            }

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
            break;
        }


        case DELFRIEND_REQUEST:         //删除好友请求
        {
            char loginName[32] = {'\0'};
            char friendName[32] = {'\0'};
            strncpy(loginName, pdu->caData, 32);
            strncpy(friendName, pdu->caData+32, 32);
            bool ret = OperateDb::getInstance().handleDelFriend(loginName, friendName);

            if(ret)
            {
                PDU *res = mkPDU(0);
                res->uiMsgType = DELFRIEND_RESPOND;
                strcpy(res->caData, DELFRIEND_OK);

                write((char*)res, res->uiPDULen);

                free(res);
                res = NULL;

                //给删除的人发个提示过去
                MyTcpServer::getInstance().sendAddFriend(friendName, pdu);
            }
            break;
        }

        case PRIVATECHAT_REQUEST:       //私聊请求
        {
            char chatName[32] = {'\0'};
            strncpy(chatName, pdu->caData+32, 32);
            qDebug() << "私聊需要转发给:"<<chatName;

            MyTcpServer::getInstance().sendAddFriend(chatName, pdu);
            qDebug() << "服务器收到的私聊消息为: "<< pdu->caMsg;

            break;
        }

        case GROUPCHAT_REQUEST:         //群聊请求
        {
            char name[32] = {'\0'};
            strncpy(name, pdu->caData, 32);
            QStringList ret = OperateDb::getInstance().handleAllOnline();
            QString tmpName;
            for(int i = 0; i < ret.size(); i++)
            {
                tmpName = ret.at(i);
                qDebug()<<"群聊需要转发给:"<<tmpName.toStdString().c_str();
                if(tmpName != QString(name))
                {
                    MyTcpServer::getInstance().sendAddFriend(tmpName.toStdString().c_str(), pdu);
                }
            }
            break;
        }

        case CREATE_DIR_REQUEST:
        {
            QDir dir;
            QString curPath = QString("%1").arg(pdu->caMsg);
            qDebug() << "---------->>>>" << curPath;

            PDU *res = NULL;
            if(dir.exists(curPath))     //当前目录存在
            {
                char newDir[32] = {'\0'};
                strncpy(newDir, pdu->caData+32, 32);        //取出新文件夹的名字
                QString newPath = curPath + "/" + newDir;         //新文件夹路径

                qDebug() << "新建文件夹路径:" << newPath;

                if(dir.exists(newPath))    //创建的文件夹名已存在
                {
                    res = mkPDU(0);
                    res->uiMsgType = CREATE_DIR_RESPOND;
                    strcpy(res->caData, DIRNAME_EXISTS);
                }else{                      //创建的文件夹名不存在
                    dir.mkdir(newPath);
                    res = mkPDU(0);
                    res->uiMsgType = CREATE_DIR_RESPOND;
                    strcpy(res->caData, CREATEDIR_OK);
                }
            }else{                      //当前目录不存在
                res = mkPDU(0);
                res->uiMsgType = CREATE_DIR_RESPOND;
                strcpy(res->caData, DIR_NOTEXISTS);
            }

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
            break;
        }

        case FLUSHDIR_REQUEST:
        {
            char *curPath = new char[pdu->uiMsgLen];
            memcpy(curPath, pdu->caMsg, pdu->uiMsgLen);
            QDir dir(curPath);
            QFileInfoList fileInfoList = dir.entryInfoList();         //获取当前文件夹下的所有文件
            int fileNum = fileInfoList.size();              //获取文件个数

            PDU *res = mkPDU(sizeof(FileInfo)*fileNum);
            res->uiMsgType = FLUSHDIR_RESPOND;
            FileInfo *fileInfo = NULL;
            QString fileName;
            for(int i = 0; i < fileNum; i++)
            {
                fileInfo = (FileInfo*)(res->caMsg)+i;    //每次偏移一个FileInfo结构体的大小
                fileName = fileInfoList[i].fileName();

                strncpy(fileInfo->fileName, fileName.toStdString().c_str(), 64);
                if(fileInfoList[i].isDir())
                {
                    fileInfo->fileType = 0;       //目录
                }
                else if(fileInfoList[i].isFile())
                {
                    fileInfo->fileType = 1;     //常规文件
                }

                //qDebug() << fileInfoList[i].fileName() << fileInfoList[i].size();
            }

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
            delete [] curPath;
            break;
        }

        case DELDIR_REQUEST:
        {
            char dirName[32] = {'\0'};
            strncpy(dirName, pdu->caData, 32);
            char *curPath = new char[pdu->uiMsgLen];
            memcpy(curPath, pdu->caMsg, pdu->uiMsgLen);
            QString path = QString("%1/%2").arg(curPath).arg(dirName);

            qDebug() << "要删除的文件夹为:" << path;

            QFileInfo fileInfo(path);
            PDU *res = mkPDU(0);
            res->uiMsgType = DELDIR_RESPOND;
            if(fileInfo.isDir())
            {
                QDir dir(path);

                bool ret = dir.removeRecursively();          //删除文件夹及包含的所有文件
                if(ret == true)
                {
                    strcpy(res->caData, DELDIR_OK);
                }else{
                    strcpy(res->caData, "删除失败!");
                }

            }
            else if(fileInfo.isFile())
            {
                strcpy(res->caData, DELDIR_FAILED);
            }

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
            delete [] curPath;
            break;
        }

        case RENAMEDIR_REQUEST:
        {
            char oldDirName[32] = {'\0'};
            char newDirName[32] = {'\0'};
            strncpy(oldDirName, pdu->caData, 32);
            strncpy(newDirName, pdu->caData+32, 32);

            char *curPath = new char[pdu->uiMsgLen];
            memcpy(curPath, pdu->caMsg, pdu->uiMsgLen);

            QString oldPath = QString("%1/%2").arg(curPath).arg(oldDirName);
            QString newPath = QString("%1/%2").arg(curPath).arg(newDirName);

            QDir dir;
            bool ret = dir.rename(oldPath, newPath);
            PDU *res = mkPDU(0);
            res->uiMsgType = RENAMEDIR_RESPOND;
            if(ret)
            {
                strcpy(res->caData, RENAMEDIR_OK);
            }else{
                strcpy(res->caData, RENAMEDIR_FAILED);
            }

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
            delete [] curPath;
            break;
        }

        case ENTERDIR_REQUEST:
        {
            char dirName[32] = {'\0'};
            strncpy(dirName, pdu->caData, 64);

            char *curPath = new char[pdu->uiMsgLen];
            memcpy(curPath, pdu->caMsg, pdu->uiMsgLen);

            QString enterPath = QString("%1/%2").arg(curPath).arg(dirName);

            QFileInfo fileInfo(enterPath);
            PDU *res = NULL;
            if(fileInfo.isDir())        //要进入的是目录就进入
            {
                res = mkPDU(enterPath.size()+32);
                res->uiMsgType = ENTERDIR_RESPOND;
                strcpy(res->caData, ENTERDIR_OK);           //ENTERDIR_OK用来告诉客户端进入文件夹
                strncpy(res->caMsg, enterPath.toStdString().c_str(), enterPath.size()+32);
            }
            else if(fileInfo.isFile())  //要进入的是文件则打开这个文件
            {
                res = mkPDU(enterPath.size()+32);
                res->uiMsgType = ENTERDIR_RESPOND;
                strcpy(res->caData, OPENFILE_OK);           //OPENFILE_OK告诉客户端打开文件
                strncpy(res->caMsg, enterPath.toStdString().c_str(), enterPath.size()+32);
            }

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
            delete [] curPath;
            break;
            break;
        }

        case UPLOADFILE_REQUEST:
        {
            char fileName[32] = {'\0'};
            qint64 fileSize;

            sscanf(pdu->caData, "%s %lld", fileName, &fileSize);
            char *curPath = new char[pdu->uiMsgLen];
            memcpy(curPath, pdu->caMsg, pdu->uiMsgLen);

            QString path = QString("%1/%2").arg(curPath).arg(fileName);

            delete [] curPath;

            //上传文件操作
            m_file.setFileName(path);

            //以只写的方式打开,若文件不存在会自动创建
            if(m_file.open(QIODevice::WriteOnly))
            {
                uplStatus = true;
                total = fileSize;
                recvSize = 0;

            }
            break;
        }

        case DELFILE_REQUEST:
        {
            char dirName[64] = {'\0'};
            strncpy(dirName, pdu->caData, 64);
            char *curPath = new char[pdu->uiMsgLen];
            memcpy(curPath, pdu->caMsg, pdu->uiMsgLen);
            QString path = QString("%1/%2").arg(curPath).arg(dirName);

            QFileInfo fileInfo(path);
            PDU *res = mkPDU(0);
            res->uiMsgType = DELFILE_RESPOND;
            if(fileInfo.isDir())
            {
                strcpy(res->caData, DELFILE_FAILED);
            }
            else if(fileInfo.isFile())
            {
                QDir dir;
                bool ret = dir.remove(path);

                if(ret)
                {
                    strcpy(res->caData, DELFILE_OK);
                }
            }

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
            delete [] curPath;
            break;
        }

        case DOWNLOAD_RESQUEST:
        {
            char fileName[32] = {'\0'};
            strncpy(fileName, pdu->caData, 32);
            char *curPath = new char[pdu->uiMsgLen];
            memcpy(curPath, pdu->caMsg, pdu->uiMsgLen);
            QString path = QString("%1/%2").arg(curPath).arg(fileName);

            delete [] curPath;
            curPath = NULL;

            QFileInfo fileInfo(path);
            qint64 fileSize= fileInfo.size();

            PDU *res = mkPDU(0);
            res->uiMsgType = DOWNLOAD_RESPOND;
            sprintf(res->caData, "%s %lld", fileName, fileSize);

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;

            m_file.setFileName(path);
            if(m_file.open(QIODevice::ReadOnly))
            {
                timer->start(1000);
            }
            else{
                qDebug() << "服务器未能打开客户端需要下载的文件!";
            }

            break;
        }

        case SHAREFILE_RESQUEST:
        {
            char shareName[32] = {'\0'};
            int num = 0;
            sscanf(pdu->caData, "%s %d", shareName, &num);
            PDU *res = mkPDU(pdu->uiMsgLen - num*32);       //分享路径所占的空间=pdu->uiMsgLen - 分享人数*32
            res->uiMsgType = SHAREFILE_NOTE;
            strcpy(res->caData, shareName);         //拷贝分享者的名字
            strncpy(res->caMsg, (pdu->caMsg)+num*32, pdu->uiMsgLen - num*32);   //拷贝分享的文件的路径

            char recvName[32] = {'\0'};
            for(int i = 0; i < num; i++)
            {
                strncpy(recvName, (pdu->caMsg)+i*32, 32);
                MyTcpServer::getInstance().sendAddFriend(recvName, res);    //转发给每一个被分享人那里去
            }
            free(res);
            res = NULL;

            res = mkPDU(0);
            res->uiMsgType = SHAREFILE_RESPOND;
            strcpy(res->caData, "分享成功!");
            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;

            break;
        }

        case SHAREFILE_NOTE_RESPOND:
        {
            QString recvPath = QString("%1/%2").arg(SERV_DIR).arg(pdu->caData);
            QString sharePath = QString("%1").arg(pdu->caMsg);
            int index = sharePath.lastIndexOf('/');
            QString fileName = sharePath.right(sharePath.size() - index -1);
            recvPath = recvPath + "/" + fileName;

            QFileInfo fileInfo(sharePath);
            if(fileInfo.isFile())
            {
                QFile::copy(sharePath, recvPath);
            }
            else if(fileInfo.isDir())
            {
                copyDir(sharePath, recvPath);
            }
            break;
        }

        case MOVEFILE_REQUEST:
        {
            char fileName[32] = {'\0'};
            int srcLen = 0;
            int destLen = 0;
            sscanf(pdu->caData, "%d %d %s", &srcLen, &destLen, fileName);

            char *srcPath = new char[srcLen];
            char *destPath = new char[destLen+64];
            memset(srcPath, '\0', srcLen);
            memset(destPath, '\0', destLen+64);

            strncpy(srcPath, pdu->caMsg, srcLen);
            strncpy(destPath, (pdu->caMsg)+srcLen, destLen);

            PDU *res = mkPDU(0);
            res->uiMsgType = MOVEFILE_RESPOND;
            QFileInfo fileInfo(destPath);
            if(fileInfo.isDir())
            {
                strcat(destPath, "/");
                strcat(destPath, fileName);

                bool ret = QFile::rename(srcPath, destPath);
                if(ret)
                {
                    strcpy(res->caData, "移动成功!");
                }
                else
                {
                    strcpy(res->caData, "移动失败!");
                }
            }
            else if(fileInfo.isFile())
            {
                strcpy(res->caData, "移动失败: 目标是一个文件!");
            }

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
            delete [] srcPath;
            delete [] destPath;
            srcPath = NULL;
            destPath = NULL;
            break;
        }
        default:
            break;
        }
        //释放pdu
        free(pdu);
        pdu = NULL;
    }
    else                        //是上传文件
    {
        QByteArray buf = readAll();
        m_file.write(buf);
        recvSize += buf.size();

        PDU *res = mkPDU(0);
        res->uiMsgType = UPLOADFILE_RESPOND;

        if(total == recvSize)
        {
            m_file.close();
            uplStatus = false;

            strcpy(res->caData, UPLOADFILE_OK);

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
        }
        else if(total < recvSize)
        {
            strcpy(res->caData, UPLOADFILE_FAILED);

            write((char*)res, res->uiPDULen);

            free(res);
            res = NULL;
        }
    }
}

void MyTcpSocket::clientOffline()
{
    OperateDb::getInstance().handleOffline(username.toStdString().c_str());
    emit offline(this);
}

void MyTcpSocket::sendFileToClient()
{
    timer->stop();

    char *buf = new char[4096];
    qint64 len = 0;
    while(1)
    {
        len = m_file.read(buf, 4096);
        if(len > 0 && len <= 4096)
        {
            write(buf, len);
        }
        else if(len == 0)
        {
            m_file.close();
            break;
        }
        else if(len < 0)
        {
            qDebug() << "发送文件给客户端过程中出错!";
            m_file.close();
            break;
        }
    }

    delete [] buf;
    buf = NULL;
}
