#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include <protocol.h>
namespace Ui {
class PrivateChat;
}

class PrivateChat : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChat(QWidget *parent = nullptr);
    ~PrivateChat();

    static PrivateChat &getInstance();

    void setChatName(QString chatName);
    void updateMsg(const PDU *pdu);

private slots:
    void on_sendBtn_clicked();

private:
    Ui::PrivateChat *ui;

    QString chatName;
    QString loginName;
};

#endif // PRIVATECHAT_H
