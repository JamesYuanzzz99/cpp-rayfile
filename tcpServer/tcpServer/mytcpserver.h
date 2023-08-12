#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QList>
#include "mytcpsocket.h"
#include <protocol.h>
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();

    static MyTcpServer &getInstance();
    void incomingConnection(qintptr socketDescriptor);

    void sendAddFriend(const char *addFriendName, PDU *pdu);
private:
    QList<MyTcpSocket*> myTcpSocketList;

public slots:
    void deleteSocket(MyTcpSocket* mySocket);
};

#endif // MYTCPSERVER_H
