#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QDir>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);
    QString getName();
    void copyDir(QString srcDir, QString destDir);

public slots:
    void recvMsg();
    void clientOffline();
    void sendFileToClient();

private:
    QString username;

    QFile m_file;       //用与接收文件
    qint64 total;       //接收的文件的总大小
    qint64 recvSize;    //已经接收的大小
    bool uplStatus;     //接收的状态

    QTimer *timer;
signals:
    void offline(MyTcpSocket* mySocket);
};

#endif // MYTCPSOCKET_H
