#include "mytcpserver.h"
#include<QDebug>
MyTcpServer::MyTcpServer()
{

}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"123456";
    MyTcpSocket *pTcpSocket = new MyTcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);
}




MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}
