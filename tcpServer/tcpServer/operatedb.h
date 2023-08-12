#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class OperateDb : public QObject
{
    Q_OBJECT
public:
    explicit OperateDb(QObject *parent = nullptr);

    static OperateDb& getInstance();
    void init();
    ~OperateDb();

    bool handleRegist(const char* name, const char* pwd);
    bool handleLogin(const char* name, const char* pwd);
    void handleOffline(const char* name);
    QStringList handleAllOnline();
    int handleSearchUser(const char* name);
    int handleAddFriend(const char* addFriendName, const char* loginName);
    bool handleAddFriendAgree(const char* addFriendName, const char* loginName);
    QStringList handleFlushFriend(const char* name);
    bool handleDelFriend(const char* loginName, const char* friendName);
private:
    QSqlDatabase m_db;
signals:

};

#endif // OPERATEDB_H
