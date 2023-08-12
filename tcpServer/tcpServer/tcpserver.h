#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class TcpServer; }
QT_END_NAMESPACE

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    TcpServer(QWidget *parent = nullptr);
    ~TcpServer();

    void loadConfig();
private:
    Ui::TcpServer *ui;
    QString ip;
    quint16 port;
};
#endif // TCPSERVER_H
