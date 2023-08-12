#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "mytcpserver.h"

#include <QMessageBox>
#include <qfile.h>
#include <QHostAddress>
#include <QDebug>
#include <qbytearray.h>

TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress::Any, port);

}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    QFile file(":/server.config");
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


