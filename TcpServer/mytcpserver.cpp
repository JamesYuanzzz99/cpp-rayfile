#include "mytcpserver.h"
#include<QDebug>
MyTcpServer::MyTcpServer()
{

}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"123456";
}




MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}
