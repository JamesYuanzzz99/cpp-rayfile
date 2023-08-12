#include "privatechat.h"
#include "ui_privatechat.h"
#include <widget.h>
#include<QMessageBox>
#include <QDebug>
PrivateChat::PrivateChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateChat)
{
    ui->setupUi(this);

    //设置输入框为默认焦点
    ui->inputLineEdit->setFocus();
}

PrivateChat::~PrivateChat()
{
    delete ui;
}

PrivateChat &PrivateChat::getInstance()
{
    static PrivateChat instance;
    return instance;
}

void PrivateChat::setChatName(QString chatName)
{
    this->chatName = chatName;
    this->loginName = Widget::getInstance().getLoginUserName();
}

void PrivateChat::updateMsg(const PDU *pdu)
{
    if(NULL == pdu)
    {
        return;
    }
    char sendName[32] = {'\0'};
    strncpy(sendName, pdu->caData, 32);

    //QString msg = QString(pdu->caMsg);
    //QString str = QString("%1: %2").arg(sendName).arg(msg);
    //qDebug() << sendName << "发过来的信息:" << msg;
    qDebug() << sendName << "发过来的信息:" << pdu->caMsg;


    ui->msgTextEdit->append(QString("%1: %2").arg(sendName).arg(pdu->caMsg));
}

void PrivateChat::on_sendBtn_clicked()
{
    QString str = ui->inputLineEdit->text();
    ui->inputLineEdit->clear();
    //QString test = "你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好你好";
    //qDebug() << test <<"的长度为:"<<test.size();
    //qDebug() << "从输入框获得的长度为: " << str.size();
    uint uiMsgLen = (uint)(str.size()+1);
    qDebug() << "从输入框获得的长度为: " << uiMsgLen;
    if(!str.isEmpty())
    {
        PDU *pdu = mkPDU(uiMsgLen*3);
        //PDU *pdu = mkPDU(str.size());
        pdu->uiMsgType = PRIVATECHAT_REQUEST;
        strncpy(pdu->caData, loginName.toStdString().c_str(), 32);
        strncpy(pdu->caData+32, chatName.toStdString().c_str(), 32);

        //strncpy(pdu->caMsg, str.toStdString().c_str(), str.size()+1);
        strcpy(pdu->caMsg, str.toStdString().c_str());

        //memcpy(pdu->caMsg, str.toStdString().c_str(), uiMsgLen);
        qDebug() << loginName << "发送的信息为:"<< pdu->caMsg;
        //ui->msgTextEdit->alignment();
        ui->msgTextEdit->append(QString("我: %1").arg(pdu->caMsg));
        //ui->msgTextEdit->append(QString("%1 :%2").arg(pdu->caMsg).arg(loginName));
        //ui->msgTextEdit->alignment();
        qDebug()<< "发送的pdu大小为:"<<(qint64)pdu->uiPDULen;
        Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::warning(this, "警告", "发送的信息不能为空!");
    }
}

