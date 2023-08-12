#include "widget.h"
#include "ui_widget.h"
#include <sharefile.h>

#include <QByteArray>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>
#include <protocol.h>
#include <operatorwidget.h>
#include <privatechat.h>
#include <QDesktopServices>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    loadConfig();
    ui->loginBtn->setEnabled(false);

    //连接服务器
    tcpSocket.connectToHost(QHostAddress(ip), port);
    //关联连接成功信号槽
    connect(&tcpSocket, &QTcpSocket::connected, this, &Widget::showConnect);

    //关联有信息接收信号槽
    connect(&tcpSocket, &QTcpSocket::readyRead, this, &Widget::recvMsg);
}

Widget::~Widget()
{
    delete ui;
}

Widget &Widget::getInstance()
{
    static Widget instance;
    return instance;
}

QTcpSocket &Widget::getTcpSocket()
{
    return tcpSocket;
}

QString Widget::getLoginUserName()
{
    return loginUserName;
}

QString Widget::getCurPath()
{
    return currentPath;
}

void Widget::setCurPath(QString path)
{
    this->currentPath = path;
}

void Widget::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray baData = file.readAll();
        QString strData = baData.toStdString().c_str();
//        qDebug() << strData;
        file.close();

        //将读取配置文件的信息127.0.0.1\r\n8888\r\n 把\r\n替换为空格
        strData.replace("\r\n", " ");

        //按空格切分数据
        QStringList strList = strData.split(" ");
//        for(int i = 0; i < strList.size(); i++)
//        {
//            qDebug() << "---->"<< strList[i];

//        }
        ip = strList.at(0);
        port = strList.at(1).toUShort();
//        qDebug()<< ip << " " << port;
    }
    else{
        QMessageBox::warning(this, "open config", "open configfile error");
    }

}

void Widget::recvMsg()
{
    if(!OperatorWidget::getInstance().getBook()->getDownloadStatus())
    {
        qDebug() << tcpSocket.bytesAvailable();
        uint uiPDULen = 0;
        tcpSocket.read((char*)&uiPDULen, sizeof(uint));

        uint uiMsgLen = uiPDULen - sizeof(PDU);
        PDU *pdu = mkPDU(uiMsgLen);
        tcpSocket.read((char*)pdu+sizeof(uint), uiPDULen-sizeof(uint));
        //qDebug() << pdu->uiMsgType << (char*)(pdu->caMsg);
        switch (pdu->uiMsgType) {
        case REGIST_RESPOND:               //是注册回复
            if(0 == strcmp(pdu->caData, RESGIST_OK))    //注册成功
            {
                QMessageBox::information(this, "注册", RESGIST_OK);
            }
            else if(0 == strcmp(pdu->caData, RESGIST_FAILED))   //注册失败
            {
                QMessageBox::warning(this, "注册", RESGIST_FAILED);
            }
            break;
        case LOGIN_RESPOND:        //是登录回复
            if(0 == strcmp(pdu->caData, LOGIN_OK))    //登录成功
            {
                currentPath = QString("%1/%2").arg(SERV_DIR).arg(loginUserName);
                qDebug() << "==========登陆时curPath" << currentPath;

                QMessageBox::information(this, "登录", LOGIN_OK);
                OperatorWidget::getInstance().show();
                this->hide();
            }
            else if(0 == strcmp(pdu->caData, LOGIN_FAILED))   //登录失败
            {
                QMessageBox::warning(this, "登录", LOGIN_FAILED);
            }
        case ALLONLINE_RESPOND:            //在线用户回复
        {
            OperatorWidget::getInstance().getFriend()->showAllOnlineUser(pdu);
            break;
        }

        //查找用户回复
        case SEARCHUSER_RESPOND:
        {
            if(0 == strcmp(SEARCH_USER_NO, pdu->caData))
            {
                QMessageBox::information(this, "查找用户", QString("%1: 不存在").arg(OperatorWidget::getInstance().getFriend()->searchName));

            }
            else if(0 == strcmp(SEARCH_USER_ONLINE, pdu->caData))
            {
                //QMessageBox::information(this, "查找用户", QString("%1: 在线").arg(OperatorWidget::getInstance().getFriend()->searchName));
                QString str = QString("%1在线!是否添加对方为好友?").arg(OperatorWidget::getInstance().getFriend()->searchName);
                QMessageBox *msgBox = new QMessageBox(QMessageBox::Question, "查找结果", str, QMessageBox::Yes | QMessageBox::No);
                msgBox->button(QMessageBox::Yes)->setText("添加好友");
                msgBox->button(QMessageBox::No)->setText("取消");
                int ret =msgBox->exec();
                if(ret == QMessageBox::Yes)
                {
                    PDU *res = mkPDU(0);
                    char addFriendName[32] = {'\0'};
                    char loginName[32] = {'\0'};
                    strncpy(addFriendName, OperatorWidget::getInstance().getFriend()->searchName.toStdString().c_str(), 32);
                    strncpy(loginName, this->getLoginUserName().toStdString().c_str(), 32);

                    strncpy(res->caData, addFriendName, 32);
                    strncpy(res->caData+32, loginName, 32);

                    //qDebug()<<"查找中的添加好友的addFriendName:"<<addFriendName;
                    //qDebug()<<"查找中的添加好友的loginName:"<<loginName;
                    res->uiMsgType = ADDFRIEND_REQUEST;

                    tcpSocket.write((char*)res, res->uiPDULen);

                    free(res);
                    res = NULL;
                }else{

                }
            }
            else if(0 == strcmp(SEARCH_USER_OFFLINE, pdu->caData))
            {
                QMessageBox::information(this, "查找用户", QString("%1: 不在线").arg(OperatorWidget::getInstance().getFriend()->searchName));
            }
            break;
        }


        //添加好友请求
        case ADDFRIEND_REQUEST:
        {
            char name[32] = {'\0'};
            //char strname[32] = {'\0'};
            strncpy(name, pdu->caData + 32, 32);
           // strncpy(strname, pdu->caData, 32);

            int ret = QMessageBox::information(this,"添加好友",QString("%1请求添加您为好友!").arg(name), QMessageBox::Yes, QMessageBox::No);

            PDU *res = mkPDU(0);
            strncpy(res->caData, pdu->caData, 32);
            strncpy(res->caData+32, pdu->caData + 32, 32);
            //qDebug() << "客户端添加好友前2" <<name;
            //qDebug() << "客户端添加好友前1" <<strname;
            if(ret == QMessageBox::Yes)
            {
                res->uiMsgType = ADDFRIEND_AGREE;
            }
            else
            {
                res->uiMsgType = ADDFRIEND_REFUSE;
            }

            tcpSocket.write((char*)res, res->uiPDULen);       //发给服务器让服务器对数据库进行操作

            free(res);
            res = NULL;
            break;
        }

        case ADDFRIEND_RESPOND:              //添加好友回复
        {
            qDebug()<< pdu->caData;
            QMessageBox::information(this, "添加好友", pdu->caData);
            break;
        }

        case FLUSHFRIEND_RESPOND:       //刷新好友列表回复
        {
            OperatorWidget::getInstance().getFriend()->updateFriendList(pdu);
            break;
        }

        case DELFRIEND_RESPOND:     //删除好友回复
        {
            QMessageBox::information(this, "删除好友", QString(pdu->caData));
            break;
        }
        case DELFRIEND_REQUEST:     //谁删除了我的提示
        {
            char name[32] = {'\0'};
            strncpy(name, pdu->caData, 32);
            QMessageBox::information(this, "提示", QString("%1 删了您!").arg(name));
            break;
        }

        case PRIVATECHAT_REQUEST:       //私聊请求
        {
            if(PrivateChat::getInstance().isHidden())
            {
                PrivateChat::getInstance().show();
            }
            char sendName[32] = {'\0'};
            strncpy(sendName, pdu->caData, 32);
            PrivateChat::getInstance().setChatName(QString(sendName));

            PrivateChat::getInstance().updateMsg(pdu);
            break;
        }

        case GROUPCHAT_REQUEST:
        {
            OperatorWidget::getInstance().getFriend()->updateGroupMsg(pdu);
            break;
        }

        case CREATE_DIR_RESPOND:
        {
            QMessageBox::information(this, "提示", pdu->caData);
            OperatorWidget::getInstance().getBook()->flushDir();
            break;
        }

        case FLUSHDIR_RESPOND:
        {
            OperatorWidget::getInstance().getBook()->updateFileList(pdu);
            break;
        }

        case DELDIR_RESPOND:
        {
            QMessageBox::information(this, "提示", pdu->caData);
            OperatorWidget::getInstance().getBook()->flushDir();
            break;
        }

        case RENAMEDIR_RESPOND:
        {
            if(strcmp(pdu->caData, RENAMEDIR_OK) == 0)  //重命名文件夹成功则刷新文件夹
            {
                OperatorWidget::getInstance().getBook()->flushDir();
            }else{                                      //重命名文件夹失败则给出提示
                QMessageBox::information(this, "提示", pdu->caData);
            }
            break;
        }

        case ENTERDIR_RESPOND:
        {
            if(0 == strcmp(pdu->caData, ENTERDIR_OK))   //进入文件夹
            {
                OperatorWidget::getInstance().getBook()->changeDir(pdu);
            }
            else            //打开文件
            {
                //QDesktopServices::openUrl(QUrl(pdu->caMsg));
                QDesktopServices::openUrl(QUrl::fromLocalFile(pdu->caMsg));
                //qDebug() << "打开文件的路径:" << QString(pdu->caMsg);
            }
            break;
        }

        case UPLOADFILE_RESPOND:
        {
            if(strcmp(pdu->caData, UPLOADFILE_OK) == 0)
            {
                OperatorWidget::getInstance().getBook()->flushDir();
            }
            else
            {
                QMessageBox::information(this, "上传文件", pdu->caData);
            }
            break;
        }

        case DELFILE_RESPOND:
        {
            if(strcmp(pdu->caData, DELFILE_FAILED) == 0)
            {
                QMessageBox::information(this, "删除文件", "请选择一个文件而非目录!");
            }
            else if(strcmp(pdu->caData, DELFILE_OK) == 0)
            {
                OperatorWidget::getInstance().getBook()->flushDir();        //删除文件成功则刷新文件夹
            }
            else{
                QMessageBox::warning(this, "删除文件", "删除失败!");
            }
            break;
        }

        case DOWNLOAD_RESPOND:
        {
            char fileName[32] = {'\0'};
            sscanf(pdu->caData, "%s %lld", fileName, &(OperatorWidget::getInstance().getBook()->total));
            qDebug() << "下载文件的名字和大小:" << fileName << OperatorWidget::getInstance().getBook()->total;

            if(strlen(fileName) > 0 && (OperatorWidget::getInstance().getBook()->total) > 0)
            {
                OperatorWidget::getInstance().getBook()->setDownloadStatus(true);
                m_file.setFileName(OperatorWidget::getInstance().getBook()->getSavePath());
                qDebug() << "下载的文件保存在:" <<OperatorWidget::getInstance().getBook()->getSavePath();

                OperatorWidget::getInstance().getBook()->getProgressBar()->setValue(0);

                if(!m_file.open(QIODevice::WriteOnly))
                {
                    QMessageBox::warning(this, "下载文件", "获得保存路劲失败!");
                }

            }
            break;
        }

        case SHAREFILE_RESPOND:         //分享文件的回复
        {
            QMessageBox::information(this, "分享文件", pdu->caData);

            ShareFile::getInstance().hide();
            break;
        }

        case SHAREFILE_NOTE:            //分享通知
        {
            char *path = new char[pdu->uiMsgLen];
            strncpy(path, pdu->caMsg, pdu->uiMsgLen);
            char *index = strrchr(path, '/');               //从右边开始找到匹配的字符返回其地址
            if(index != NULL)
            {
                index++;            //得到文件名首的地址
                QString note = QString("%1 分享了 %2 \n 你是否接收该文件?").arg(pdu->caData).arg(index);
                int ret = QMessageBox::question(this, "分享文件", note);
                if(ret == QMessageBox::Yes)
                {
                    PDU *res = mkPDU(pdu->uiMsgLen);
                    res->uiMsgType = SHAREFILE_NOTE_RESPOND;
                    strncpy(res->caMsg, pdu->caMsg, pdu->uiMsgLen);
                    QString loginName = Widget::getInstance().getLoginUserName();
                    strcpy(res->caData, loginName.toStdString().c_str());

                    tcpSocket.write((char*)res, res->uiPDULen);

                    free(res);
                    res = NULL;
                }
            }
            delete [] path;
            path = NULL;
            delete index;
            break;
        }

        case MOVEFILE_RESPOND:
        {
            QMessageBox::information(this, "移动文件", pdu->caData);
            OperatorWidget::getInstance().getBook()->flushDir();
            break;
        }
        default:
            break;
        }


        //释放pdu
        free(pdu);
        pdu = NULL;
    }
    else        //接收下载文件的数据
    {
        Book *pbook = OperatorWidget::getInstance().getBook();
        int percent = 0;

        QByteArray buf = tcpSocket.readAll();
        m_file.write(buf, buf.size());
        pbook->recvSize += buf.size();

        percent = ((pbook->recvSize)*100)/(pbook->total);
        pbook->getProgressBar()->setValue(percent);
        if(pbook->recvSize == pbook->total)
        {
            m_file.close();
            pbook->recvSize = 0;
            pbook->total = 0;
            pbook->setDownloadStatus(false);

            QMessageBox::information(this, "下载文件", "下载完成!");
        }
        else if(pbook->recvSize > pbook->total)
        {
            qDebug() << "recvSize:" <<pbook->recvSize << "total" << pbook->total;
            m_file.close();
            pbook->recvSize = 0;
            pbook->total = 0;
            pbook->setDownloadStatus(false);

            QMessageBox::critical(this, "下载文件", "下载文件失败");
        }
    }
}

void Widget::showConnect()
{
    //QMessageBox::information(this, "connect info", "连接成功!");
    ui->loginBtn->setEnabled(true);
}

#if 0
void Widget::on_send_clicked()
{
    QString strMsg = ui->lineEdit->text();
    if(!strMsg.isEmpty())
    {
        PDU *pdu = mkPDU(strMsg.size());
        pdu->uiMsgType = 8888;
        memcpy(pdu->caMsg, strMsg.toStdString().c_str(), strMsg.size());
        tcpSocket.write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }else{
        QMessageBox::warning(this,"信息发送","信息不能为空");
    }
}
#endif


void Widget::on_loginBtn_clicked()
{
    QString strName = ui->userLineEdit->text();
    QString pwd = ui->passwordLineEdit->text();
    if(!strName.isEmpty() && !pwd.isEmpty())
    {
        loginUserName = strName;
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = LOGIN_REQUEST;
        strncpy(pdu->caData, strName.toStdString().c_str(), 32);
        strncpy(pdu->caData+32, pwd.toStdString().c_str(), 32);
        tcpSocket.write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }else{
        QMessageBox::critical(this, "登录信息", "登录失败: 用户名或密码不能为空");
    }
}


void Widget::on_registBtn_clicked()
{
    QString strName = ui->userLineEdit->text();
    QString pwd = ui->passwordLineEdit->text();
    if(!strName.isEmpty() && !pwd.isEmpty())
    {
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = REGIST_REQUEST;
        strncpy(pdu->caData, strName.toStdString().c_str(), 32);
        strncpy(pdu->caData+32, pwd.toStdString().c_str(), 32);
        tcpSocket.write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }else{
        QMessageBox::critical(this, "注册信息", "注册失败: 用户名或密码不能为空");
    }
}


void Widget::on_cancelBtn_clicked()
{

}

