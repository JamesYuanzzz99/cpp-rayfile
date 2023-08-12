#include "online.h"
#include "ui_online.h"
#include <QDebug>
#include <widget.h>
#pragma execution_character_set("utf-8")
Online::Online(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Online)
{
    ui->setupUi(this);
    connect(ui->onlineListW, &QListWidget::itemClicked, this, [=](){
        if(ui->onlineListW->currentItem()->text() == Widget::getInstance().getLoginUserName())
        {
            ui->addFriendBtn->setEnabled(false);
        }
        else{
            ui->addFriendBtn->setEnabled(true);
        }
    });
}

Online::~Online()
{
    delete ui;
}

QString Online::unicodeToUtf8(const QString &unicode)
{
    QString result;
    int index = unicode.indexOf("\\u");
    while (index != -1)
    {
        QString s1 = unicode.mid(index + 2, 4);
        result.append(s1.toUShort(0, 16));
        index = unicode.indexOf("\\u", index+5);
    }
    return result.toUtf8().constData();

}

void Online::showUser(PDU *pdu)
{
    if(pdu == NULL)
    {
        return;
    }
    //返回的在线用户名的个数
    uint num = (pdu->uiPDULen - sizeof(PDU))/32;
    qDebug() << "num = "<<num;
    qDebug() << "pdu caMsg = "<< pdu->caMsg;
    char temp[32];
    for(uint i = 0; i < num; i++)
    {
        memcpy(temp, (pdu->caMsg)+i*32, 32);
        //strcpy(temp, pdu->caMsg);
        ui->onlineListW->addItem(temp);
        qDebug() << temp;
    }
}

QString Online::utf8ToUnicode(const QString strUtf8)
{
    QString strOut;
    QString unidata = strUtf8;
    for (int i = 0; i < unidata.length(); ++i)
    {
        ushort num = unidata[i].unicode();
        if (num < 255)
            strOut += unidata[i];
        else
            strOut += QString("&#x%1;").arg(QString::number(num, 16));
    }

    return strOut;
}

Ui::Online *Online::getUi()
{
    return ui;
}

void Online::on_addFriendBtn_clicked()
{
    QListWidgetItem *item = ui->onlineListW->currentItem();
    if(item == NULL)
    {
        return;
    }
    //qDebug() << item->text();
    QString addFriendName = item->text();
    QString loginUserName = Widget::getInstance().getLoginUserName();

    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ADDFRIEND_REQUEST;
//    memcpy(pdu->caData, addFriendName.toStdString().c_str(), addFriendName.size());
//    memcpy(pdu->caData+32, loginUserName.toStdString().c_str(), loginUserName.size());
    strncpy(pdu->caData, addFriendName.toStdString().c_str(), 32);
    strncpy(pdu->caData + 32, loginUserName.toStdString().c_str(), 32);
    //qDebug()<< "on_addFriendBtn_clicked: "<<pdu->caData;

    //发送数据
    Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

    free(pdu);
    pdu = NULL;
}

