#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public slots:
    void showConnect();
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    static Widget &getInstance();
    QTcpSocket &getTcpSocket();
    QString getLoginUserName();
    QString getCurPath();
    void setCurPath(QString path);

    void loadConfig();
    void recvMsg();

private slots:
    //void on_send_clicked();

    void on_loginBtn_clicked();

    void on_registBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::Widget *ui;
    QString ip;
    quint16 port;
    QTcpSocket tcpSocket;

    QString loginUserName;

    QString currentPath;

    QFile m_file;
};
#endif // WIDGET_H
