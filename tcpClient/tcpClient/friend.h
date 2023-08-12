#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <online.h>
class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = nullptr);
    void showAllOnlineUser(PDU *pdu);
    void updateFriendList(PDU *pdu);
    void updateGroupMsg(PDU *pdu);
    QListWidget* getFriendList();

    QString searchName;       //保存查找的用户名

signals:

public slots:
    void showOnline();
    void searchUser();
    void flushFriend();
    void deleteFriend();
    void privateChat();
    void groupChat();
private:
    QTextEdit *showMsg;         //显示消息的文本编辑框
    QListWidget *showFriend;    //显示好友列表
    QLineEdit *inputMsg;        //信息输入框

    QPushButton *delFriendBtn;  //删除好友
    QPushButton *flushFriendBtn;  //刷新好友列表
    QPushButton *showOnlineUserBtn;   //显示在线用户
    QPushButton *searchBtn;       //查询用户
    QPushButton *sendMsgBtn;      //发送消息
    QPushButton *privateChatBtn;     //私聊

    Online *online;

};

#endif // FRIEND_H
