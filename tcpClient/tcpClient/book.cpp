#include "book.h"
#include <QInputDialog>
#include <widget.h>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <operatorwidget.h>
#include <sharefile.h>
Book::Book(QWidget *parent)
    : QWidget{parent}
{
    bookList = new QListWidget(this);
    backBtn = new QPushButton("返回", this);
    createDirBtn = new QPushButton("创建文件夹", this);
    delDirBtn = new QPushButton("删除文件夹", this);
    renameDirBtn = new QPushButton("重命名文件夹", this);
    flushDirBtn = new QPushButton("刷新文件夹", this);
    selectDestDirBtn = new QPushButton("目标目录", this);
    uploadFileBtn = new QPushButton("上传文件", this);
    delFileBtn = new QPushButton("删除文件", this);
    downloadFileBtn = new QPushButton("下载文件", this);
    shareFileBtn = new QPushButton("分享文件", this);
    renameFileBtn = new QPushButton("重命名文件", this);
    moveFileBtn = new QPushButton("移动文件", this);

    timer = new QTimer;

    dlStatus = false;

    recvSize = 0;

    selectDestDirBtn->setEnabled(false);


    proBar = new QProgressBar(this);
    proBar->setRange(0, 100);
    proBar->setValue(0);
    proBar->resize(150,30);
    proBar->move(425, 345);
    //proBar->show();

    QVBoxLayout *vbl1 = new QVBoxLayout;
    vbl1->addWidget(backBtn);
    vbl1->addWidget(createDirBtn);
    vbl1->addWidget(delDirBtn);
    vbl1->addWidget(renameDirBtn);
    vbl1->addWidget(flushDirBtn);
    vbl1->addWidget(selectDestDirBtn);

    QVBoxLayout *vbl2 = new QVBoxLayout;
    vbl2->addWidget(uploadFileBtn);
    vbl2->addWidget(downloadFileBtn);
    vbl2->addWidget(delFileBtn);
    vbl2->addWidget(shareFileBtn);
    vbl2->addWidget(renameFileBtn);
    vbl2->addWidget(moveFileBtn);

    QHBoxLayout *hbl = new QHBoxLayout;
    hbl->addWidget(bookList);
    hbl->addLayout(vbl1);
    hbl->addLayout(vbl2);

    setLayout(hbl);

    //创建文件夹的信号槽
    connect(createDirBtn, &QPushButton::clicked, this, &Book::createDir);

    //刷新文件夹
    connect(flushDirBtn, &QPushButton::clicked, this, &Book::flushDir);

    //删除文件夹
    connect(delDirBtn, &QPushButton::clicked, this, &Book::delDir);

    //重命名文件夹
    connect(renameDirBtn, &QPushButton::clicked, this, &Book::renameDir);

    //进入文件夹  双击进入
    connect(bookList, &QListWidget::doubleClicked, this, &Book::enterDir);

    //返回上一级目录
    connect(backBtn, &QPushButton::clicked, this, &Book::back);

    //上传文件
    connect(uploadFileBtn, &QPushButton::clicked, this, &Book::uploadFile);

    //上传文件的内容
    connect(timer, &QTimer::timeout, this, &Book::uploadFileData);

    //删除常规文件
    connect(delFileBtn, &QPushButton::clicked, this, &Book::delFile);

    //下载文件
    connect(downloadFileBtn, &QPushButton::clicked, this, &Book::downloadFile);

    //分享文件
    connect(shareFileBtn, &QPushButton::clicked, this, &Book::shareFile);

    //
    connect(moveFileBtn, &QPushButton::clicked, this, &Book::moveFile);

    //
    connect(selectDestDirBtn, &QPushButton::clicked, this, &Book::sltDestDir);
}

//刷新文件列表
void Book::updateFileList(const PDU *pdu)
{
    bookList->clear();
    if(NULL == pdu)
    {
        return;
    }
    FileInfo *fileInfo = NULL;
    int num = pdu->uiMsgLen/sizeof(FileInfo);
    for(int i = 0; i < num; i++)
    {
        fileInfo = (FileInfo*)(pdu->caMsg)+i;
        qDebug() << fileInfo->fileName << fileInfo->fileType;

        //屏蔽掉 . 和 .. 两个隐藏文件夹
        if(strcmp(fileInfo->fileName, ".") == 0 || strcmp(fileInfo->fileName, "..") == 0)
        {
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem;
        if(0 == fileInfo->fileType)
        {
            item->setIcon(QIcon(QPixmap(":/icon/dir.png")));
        }
        else if(1 == fileInfo->fileType)
        {
            item->setIcon(QIcon(QPixmap(":/icon/file.png")));
        }
        item->setText(fileInfo->fileName);

        bookList->addItem(item);
    }


    //可以用下面的方法获取指定文件的图标
//    QFileInfo file_info(name);
//    QFileIconProvider icon_provider;
    //    QIcon icon = icon_provider.icon(file_info);
}

void Book::setDownloadStatus(bool status)
{
    dlStatus = status;
}

bool Book::getDownloadStatus()
{
    return dlStatus;
}

QString Book::getSavePath()
{
    return savePath;
}

QProgressBar* Book::getProgressBar()
{
    return proBar;
}

QString Book::getShareFileName()
{
    return shareFileName;
}

//创建文件夹
void Book::createDir()
{
    QString newDir = QInputDialog::getText(this, "新建文件夹", "新文件夹名");

    if(!newDir.isEmpty())
    {
        if(newDir.size() > 32)
        {
            QMessageBox::warning(this, "新建文件夹", "输入内容过长!");
        }
        else
        {
            QString loginName = Widget::getInstance().getLoginUserName();
            QString curPath = Widget::getInstance().getCurPath();
            PDU *pdu = mkPDU(curPath.size()+20);

            pdu->uiMsgType = CREATE_DIR_REQUEST;
            strncpy(pdu->caData, loginName.toStdString().c_str(), 32);
            strncpy(pdu->caData+32, newDir.toStdString().c_str(), 32);
            strncpy(pdu->caMsg, curPath.toStdString().c_str(), curPath.size()+20);

            Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

            free(pdu);
            pdu = NULL;
        }
    }
    else
    {
        QMessageBox::warning(this, "新建文件夹", "输入内容不能为空!");
    }
}

//刷新文件夹
void Book::flushDir()
{
    QString curPath = Widget::getInstance().getCurPath();
    PDU *pdu = mkPDU(curPath.size()+30);
    pdu->uiMsgType = FLUSHDIR_REQUEST;
    strncpy(pdu->caMsg, curPath.toStdString().c_str(), curPath.size()+30);

    Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

    free(pdu);
    pdu = NULL;
}

//删除文件夹
void Book::delDir()
{
    QString curPath = Widget::getInstance().getCurPath();
    QListWidgetItem *item = bookList->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "提示", "请选择要删除的文件夹!");
    }
    else{
        QString delName = item->text();
        PDU *pdu = mkPDU(curPath.size()+32);
        pdu->uiMsgType = DELDIR_REQUEST;
        strncpy(pdu->caData, delName.toStdString().c_str(), 32);
        strncpy(pdu->caMsg, curPath.toStdString().c_str(), curPath.size()+32);

        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }
}

//重命名文件夹
void Book::renameDir()
{
    QString curPath = Widget::getInstance().getCurPath();
    QListWidgetItem *item = bookList->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "提示", "请选择要重命名的文件夹!");
    }
    else{
        QString oldDirName = item->text();
        QString newDirName = QInputDialog::getText(this, "提示", "新文件夹名");
        if(!newDirName.isEmpty())
        {
            PDU *pdu = mkPDU(curPath.size()+32);
            pdu->uiMsgType = RENAMEDIR_REQUEST;
            strncpy(pdu->caData, oldDirName.toStdString().c_str(), 32);
            strncpy(pdu->caData+32, newDirName.toStdString().c_str(), 32);
            strncpy(pdu->caMsg, curPath.toStdString().c_str(), curPath.size()+32);

            Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

            free(pdu);
            pdu = NULL;
        }
        else
        {
            QMessageBox::warning(this, "提示", "新文件夹名不能为空!");
        }
    }
}


//进入文件夹
void Book::enterDir(const QModelIndex &index)
{
    QString dirName = index.data().toString();
    QString curPath = Widget::getInstance().getCurPath();

    PDU *pdu = mkPDU(curPath.size()+32);
    pdu->uiMsgType = ENTERDIR_REQUEST;
    strncpy(pdu->caData, dirName.toStdString().c_str(), 64);
    strncpy(pdu->caMsg, curPath.toStdString().c_str(), curPath.size()+32);

    Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

    free(pdu);
    pdu = NULL;
}

void Book::changeDir(const PDU *pdu)
{
    char *enterPath = new char[pdu->uiMsgLen];
    memcpy(enterPath, pdu->caMsg, pdu->uiMsgLen);
    PDU *res = mkPDU((pdu->uiMsgLen)+32);
    res->uiMsgType = FLUSHDIR_REQUEST;
    strncpy(res->caMsg, enterPath, (pdu->uiMsgLen)+32);

    Widget::getInstance().setCurPath(QString(enterPath));       //这一步巨特么重要,切换目录之后一定要更改当前目录
    Widget::getInstance().getTcpSocket().write((char*)res, res->uiPDULen);

    free(res);
    res = NULL;
    delete [] enterPath;
}

void Book::back()
{
    QString curPath = Widget::getInstance().getCurPath();
    QString rootPath = QString("%1/%2").arg(SERV_DIR).arg(Widget::getInstance().getLoginUserName());
    if(rootPath == curPath)
    {
        QMessageBox::information(this, "提示", "已经是最顶层目录!");
    }
    else
    {
        //QMessageBox::information(this, "提示", "不是最顶层");
        int index = curPath.lastIndexOf('/');    //这个函数返回从最后后开始往前匹配到指定字符的位置
        curPath.remove(index, curPath.size()-index);
        Widget::getInstance().setCurPath(curPath);
        flushDir();
    }
}

void Book::uploadFile()
{
    QString curPath = Widget::getInstance().getCurPath();
    uploadFilePath = QFileDialog::getOpenFileName();

    if(!uploadFilePath.isEmpty())
    {
        int index = uploadFilePath.lastIndexOf('/');
        QString fileName = uploadFilePath.right(uploadFilePath.size()-index-1);     //获取文件名

        QFile file(uploadFilePath);

        PDU *pdu = mkPDU(curPath.size()+32);
        pdu->uiMsgType = UPLOADFILE_REQUEST;
        strncpy(pdu->caMsg, curPath.toStdString().c_str(), curPath.size()+32);
        sprintf(pdu->caData, "%s %lld", fileName.toStdString().c_str(), file.size());

        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;

        timer->start(1000);     //1s后开始发送文件内容防止粘包
    }
    else
    {
        QMessageBox::warning(this, "提示", "请选择一个文件!");
    }
}

void Book::uploadFileData()
{
    proBar->setValue(0);
    timer->stop();
    QFile file(uploadFilePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "上传文件", "打开文件失败!");
        return;
    }

    //proBar->show();             //显示上传进度条

    char *buf = new char[4096];
    qint64 len = 0;
    int percent = 0;
    qint64 sendSize = 0;
    while(1)
    {
        len = file.read(buf, 4096);
        if(len > 0 && len <=4096)
        {
            sendSize += len;
            Widget::getInstance().getTcpSocket().write(buf, len);
            percent = (sendSize * 100)/file.size();
            //qDebug() << "进度:" << percent;
            proBar->setValue(percent);
        }
        else if(len == 0)       //读到文件末尾
        {
            //proBar->close();
            break;
        }
        else
        {
            QMessageBox::warning(this, "上传文件", "读取文件失败!");
            break;
        }
    }

    file.close();
    delete [] buf;
    buf = NULL;
}

void Book::delFile()
{
    QString curPath = Widget::getInstance().getCurPath();
    QListWidgetItem *item = bookList->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "提示", "请选择要删除的文件!");
    }
    else{
        QString delName = item->text();
        PDU *pdu = mkPDU(curPath.size()+32);
        pdu->uiMsgType = DELFILE_REQUEST;
        strncpy(pdu->caData, delName.toStdString().c_str(), 64);
        strncpy(pdu->caMsg, curPath.toStdString().c_str(), curPath.size()+32);

        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }
}

void Book::downloadFile()
{
    QListWidgetItem *item = bookList->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "提示", "请选择下载的文件!");
    }
    else
    {
        QString saveFilePath = QFileDialog::getSaveFileName();
        if(saveFilePath.isEmpty())
        {
            QMessageBox::warning(this, "提示", "请指定保存的位置!");
            savePath.clear();
        }
        else
        {
            savePath = saveFilePath;
        }

       QString curPath = Widget::getInstance().getCurPath();
       PDU *pdu = mkPDU(curPath.size()+32);
       pdu->uiMsgType = DOWNLOAD_RESQUEST;
       QString fileName = item->text();

       strncpy(pdu->caData, fileName.toStdString().c_str(), 32);
       strncpy(pdu->caMsg, curPath.toStdString().c_str(), curPath.size()+32);

       Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

       free(pdu);
       pdu = NULL;
    }
}

void Book::shareFile()
{
    QListWidgetItem *item = bookList->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "分享文件", "请选择分享的文件!");
        return;
    }
    else
    {
        shareFileName = item->text();

        Friend *pfriend = OperatorWidget::getInstance().getFriend();
        QListWidget *friendList = pfriend->getFriendList();         //获得在线好友列表

        ShareFile::getInstance().updateFriend(friendList);
        if(ShareFile::getInstance().isHidden())
        {
            ShareFile::getInstance().show();
        }
    }

}

void Book::moveFile()
{
    QListWidgetItem *item = bookList->currentItem();
    if(NULL != item)
    {
        selectDestDirBtn->setEnabled(true);

        moveFileName = item->text();
        QString curPath = Widget::getInstance().getCurPath();
        moveFilePath = curPath + "/" + moveFileName;
    }
    else
    {
        QMessageBox::warning(this, "移动文件", "请选择要移动的文件");
    }
}

void Book::sltDestDir()
{
    QListWidgetItem *item = bookList->currentItem();
    if(NULL != item)
    {
        QString destDir = item->text();
        QString curPath = Widget::getInstance().getCurPath();
        this->destDir = curPath + "/" + destDir;

        selectDestDirBtn->setEnabled(false);

        int srcLen = moveFilePath.size()+32;
        int destLen = this->destDir.size()+32;
        PDU *pdu = mkPDU(srcLen+destLen);
        pdu->uiMsgType = MOVEFILE_REQUEST;
        sprintf(pdu->caData, "%d %d %s", srcLen, destLen, moveFileName.toStdString().c_str());

        strncpy(pdu->caMsg, moveFilePath.toStdString().c_str(), srcLen);
        strncpy((pdu->caMsg)+srcLen, this->destDir.toStdString().c_str(), destLen);

        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }
    else
    {
        //QMessageBox::warning(this, "移动文件", "请选择要移动的文件");
        this->destDir = Widget::getInstance().getCurPath();
        selectDestDirBtn->setEnabled(false);

        int srcLen = moveFilePath.size()+32;
        int destLen = this->destDir.size()+32;
        PDU *pdu = mkPDU(srcLen+destLen);
        pdu->uiMsgType = MOVEFILE_REQUEST;
        sprintf(pdu->caData, "%d %d %s", srcLen, destLen, moveFileName.toStdString().c_str());

        strncpy(pdu->caMsg, moveFilePath.toStdString().c_str(), srcLen);
        strncpy((pdu->caMsg)+srcLen, this->destDir.toStdString().c_str(), destLen);

        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }
}


