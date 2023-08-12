#include "mytcpserver.h"
#include <QDebug>
MyTcpServer::MyTcpServer()
{

}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<< "new client";
    MyTcpSocket *pTcpSocket = new MyTcpSocket(this);
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    myTcpSocketList.append(pTcpSocket);

    connect(pTcpSocket, &MyTcpSocket::offline, this, &MyTcpServer::deleteSocket);
}

void MyTcpServer::sendAddFriend(const char *addFriendName, PDU *pdu)
{
    if(NULL == addFriendName || NULL == pdu)
    {
        return;
    }
    QString name = addFriendName;
    for(int i = 0; i < myTcpSocketList.size(); i++)
    {
        if(myTcpSocketList.at(i)->getName() == name)
        {
            myTcpSocketList.at(i)->write((char*)pdu, pdu->uiPDULen);
            break;
        }
    }
    qDebug() << "sendAddFriend没有崩溃";
}

void MyTcpServer::deleteSocket(MyTcpSocket *mySocket)
{
    QList<MyTcpSocket*>::iterator iter = myTcpSocketList.begin();
    for(; iter != myTcpSocketList.end(); iter++)
    {
        if(mySocket == *iter)
        {
            qDebug() << mySocket->getName()<<"下线了";       //打印一下是谁下线了
            (*iter)->deleteLater();
            *iter = NULL;
            myTcpSocketList.erase(iter);
            break;
        }
    }
    for(int i = 0; i < myTcpSocketList.size(); i++)
    {
        qDebug() << myTcpSocketList.at(i)->getName();
    }
}
