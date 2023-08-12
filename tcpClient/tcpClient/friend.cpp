#include "friend.h"
#include <protocol.h>
#include <widget.h>
#include "ui_online.h"
#include <QDebug>
#include <QInputDialog>
#include <string>
#include <privatechat.h>
#include <QMessageBox>
Friend::Friend(QWidget *parent)
    : QWidget{parent}
{
    showMsg = new QTextEdit(this);
    showFriend = new QListWidget(this);
    inputMsg = new QLineEdit(this);

    delFriendBtn = new QPushButton("删除好友", this);
    flushFriendBtn = new QPushButton("刷新好友", this);
    showOnlineUserBtn = new QPushButton("显示在线用户", this);
    searchBtn = new QPushButton("查找用户", this);
    sendMsgBtn = new QPushButton("发送",this);
    privateChatBtn = new QPushButton("私聊",this);

    online = new Online(this);


    //给5个按钮来个垂直布局,发送按钮没算
    QVBoxLayout *rightBtnVBL = new QVBoxLayout;
    rightBtnVBL->addWidget(delFriendBtn);
    rightBtnVBL->addWidget(flushFriendBtn);
    rightBtnVBL->addWidget(showOnlineUserBtn);
    rightBtnVBL->addWidget(searchBtn);
    rightBtnVBL->addWidget(privateChatBtn);

    //消息显示框和,好友列表和5个按钮来个水平布局
    QHBoxLayout *topHBL = new QHBoxLayout;
    topHBL->addWidget(showMsg);
    topHBL->addWidget(showFriend);
    topHBL->addLayout(rightBtnVBL);

    //信息输入框和发送按钮来个水平布局
    QHBoxLayout *msgHBL = new QHBoxLayout;
    msgHBL->addWidget(inputMsg);
    msgHBL->addWidget(sendMsgBtn);

    QVBoxLayout *mainVBL = new QVBoxLayout;
    mainVBL->addLayout(topHBL);
    mainVBL->addLayout(msgHBL);
    mainVBL->addWidget(online);
    online->hide();

    setLayout(mainVBL);

    //关联显示在线用户的信号槽
    connect(showOnlineUserBtn, &QPushButton::clicked, this, &Friend::showOnline);

    //关联查找用户的信号槽
    connect(searchBtn, &QPushButton::clicked, this, &Friend::searchUser);

    //关联刷新好友列表的信号槽
    connect(flushFriendBtn, &QPushButton::clicked, this, &Friend::flushFriend);

    //删除好友的信号槽
    connect(delFriendBtn, &QPushButton::clicked, this, &Friend::deleteFriend);

    //私聊的信号槽
    connect(privateChatBtn, &QPushButton::clicked, this, &Friend::privateChat);

    //群聊的信号槽
    connect(sendMsgBtn, &QPushButton::clicked, this, &Friend::groupChat);
#if 0
    showMsg->setText("我槽");
#endif
}

void Friend::showAllOnlineUser(PDU *pdu)
{
    if(pdu == NULL)
    {
        return;
    }
    online->showUser(pdu);
}

void Friend::updateFriendList(PDU *pdu)
{
    showFriend->clear();
    if(NULL == pdu)
    {
        return;
    }
    uint uiSize = pdu->uiMsgLen/32;
    char name[32] = {'\0'};
    for(uint i = 0; i < uiSize; i++)
    {
        strncpy(name, pdu->caMsg + i*32, 32);
        showFriend->addItem(name);
    }
}

void Friend::updateGroupMsg(PDU *pdu)
{
    QString msg = QString("%1: %2").arg(pdu->caData).arg(pdu->caMsg);
    showMsg->append(msg);
}

QListWidget* Friend::getFriendList()
{
    return  showFriend;
}


void Friend::showOnline()     //显示在线用户
{
    online->getUi()->onlineListW->clear();
    if(online->isHidden())
    {
        online->show();
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ALLONLINE_REQUEST;
        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }else{
        online->hide();
        online->getUi()->onlineListW->clear();
    }
}

void Friend::searchUser()
{
    searchName = QInputDialog::getText(this, "查找用户","用户名");
    if(!searchName.isEmpty())
    {
        qDebug()<<searchName;
        PDU *pdu = mkPDU(0);
        strcpy(pdu->caData, searchName.toStdString().c_str());
        qDebug() << "caData = "<<pdu->caData;
        pdu->uiMsgType = SEARCHUSER_REQUEST;
        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }
}

void Friend::flushFriend()
{
    QString myName = Widget::getInstance().getLoginUserName();
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = FLUSHFRIEND_REQUEST;
    strncpy(pdu->caData, myName.toStdString().c_str(), 32);
    qDebug() << pdu->caData;

    Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

    free(pdu);
    pdu = NULL;
}

void Friend::deleteFriend()
{
    if(NULL != showFriend->currentItem())
    {
        QString delName = showFriend->currentItem()->text();
        QString loginName = Widget::getInstance().getLoginUserName();
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = DELFRIEND_REQUEST;
        strncpy(pdu->caData, loginName.toStdString().c_str(), 32);
        strncpy(pdu->caData+32, delName.toStdString().c_str(), 32);
        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }
}

void Friend::privateChat()
{
    if(NULL != showFriend->currentItem())
    {
        QString chatName = showFriend->currentItem()->text();  //获取聊天对象的名字
        PrivateChat::getInstance().setChatName(chatName);      //设置好聊天对象的名字

        if(PrivateChat::getInstance().isHidden())
        {
            PrivateChat::getInstance().show();
        }
    }else{
        QMessageBox::warning(this, "警告", "请选择私聊的对象!");
    }
}

void Friend::groupChat()
{
    QString str = inputMsg->text();
    inputMsg->clear();
    if(!str.isEmpty())
    {
        PDU *pdu = mkPDU((str.size()+1)*3);
        pdu->uiMsgType = GROUPCHAT_REQUEST;

        QString name = Widget::getInstance().getLoginUserName();

        showMsg->append(QString("%1: %2").arg(name).arg(str));

        strncpy(pdu->caData, name.toStdString().c_str(), 32);
        qDebug() << "------------群聊发送人为:"<<pdu->caData;
        strncpy(pdu->caMsg, str.toStdString().c_str(), (str.size()+1)*3);

        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu == NULL;
    }else{
        QMessageBox::warning(this, "群聊", "信息能为空!");
    }
}
