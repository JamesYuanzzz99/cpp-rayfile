#include "operatedb.h"
#include <QDebug>
#include <QMessageBox>

OperateDb::OperateDb(QObject *parent)
    : QObject{parent}
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}

OperateDb &OperateDb::getInstance()
{
    static OperateDb instance;
    return instance;
}

void OperateDb::init()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("mydb5");
    m_db.setPort(3306);
    m_db.setUserName("root");
    m_db.setPassword("021126");
    if(m_db.open())
    {
        QSqlQuery q;
        q.exec("select name, pwd from userInfo");
        while(q.next())
        {
            QString data = QString("%1, %2").arg(q.value(0).toString()).arg(q.value(1).toString());
            qDebug() << data;
        }
    }else{
        QMessageBox::critical(NULL, "打开数据库", "打开数据库失败");
    }
}

OperateDb::~OperateDb()
{
    m_db.close();
}

bool OperateDb::handleRegist(const char *name, const char *pwd)
{
    if(name == NULL || pwd == NULL)
    {
        return false;
    }
    QString sql = QString("insert into userInfo(name, pwd) values(\'%1\', \'%2\')").arg(name).arg(pwd);
    QSqlQuery q;
    return q.exec(sql);
}

bool OperateDb::handleLogin(const char *name, const char *pwd)
{
    if(name == NULL || pwd == NULL)
    {
        return false;
    }
    QString sql = QString("select * from userInfo where name=\'%1\' and pwd=\'%2\' and isOnline=0").arg(name).arg(pwd);
    QSqlQuery q;
    q.exec(sql);
    if(q.next())   //查询有数据返回true,没有数据返回false
    {
        //将登录状态改成1
        sql = QString("update userInfo set isOnline=1 where name=\'%1\' and pwd=\'%2\'").arg(name).arg(pwd);
        q.exec(sql);

        return true;
    }else{
        return false;
    }
}

void OperateDb::handleOffline(const char *name)
{
    if(name == NULL)
    {
        return;
    }
    QString sql = QString("update userInfo set isOnline=0 where name=\'%1\'").arg(name);
    QSqlQuery q;
    q.exec(sql);
}

QStringList OperateDb::handleAllOnline()
{
    QString sql = QString("select name from userInfo where isOnline=1");
    QSqlQuery q;
    q.exec(sql);

    QStringList res;
    res.clear();

    while(q.next())
    {

        res.append(q.value(0).toString());
    }
    return res;
}

//查找用户
int OperateDb::handleSearchUser(const char *name)
{
    if(name == NULL)
    {
        return -1;
    }
    QString sql = QString("select isOnline from userInfo where name=\'%1\'").arg(name);
    QSqlQuery q;
    q.exec(sql);
    if(q.next())
    {
        int ret = q.value(0).toInt();
        if(ret == 1)
        {
            return 1;          //表示存在且在线
        }
        else if(ret == 0)
        {
            return 0;        //表示存在且不在线
        }
    }else{
        return -1;        //表示不存在
    }
}

//-2 形参为空
//-1 添加的好友不存在
//2  双方已经是好友
//0  不是好友且不在线
//1  不是好友但是在线
int OperateDb::handleAddFriend(const char *addFriendName, const char *loginName)
{
    //qDebug()<< "addFriendName: "<<addFriendName;
    if(NULL == addFriendName || NULL == loginName)
    {
        return -2;
    }
    QString sql = QString("select * from friend where (id=(select id from userInfo where name=\'%1\') and friendId=(select id from userInfo where name=\'%2\')) or (id=(select id from userInfo where name=\'%3\') and friendId=(select id from userInfo where name=\'%4\'))").arg(loginName).arg(addFriendName).arg(addFriendName).arg(loginName);
    QSqlQuery q;
    q.exec(sql);
    //qDebug() << "添加好友:"<<q.value(0).toInt();
    if(q.next())
    {
        //qDebug() << "进入了双方已经是好友";
        return 2;       //双方已经是好友
    }
    QString sql2 = QString("select isOnline from userInfo where name=\'%1\'").arg(addFriendName);
    q.exec(sql2);
    if(q.next())
    {
        int ret = q.value(0).toInt();
        if(ret == 1)
        {
            return 1;
        }
        else if(ret == 0)
        {
            return 0;        //表示存在且不在线
        }
    }else{
        return -1;        //表示不存在
    }
}

bool OperateDb::handleAddFriendAgree(const char *addFriendName, const char *loginName)
{
    if(addFriendName == NULL || loginName == NULL)
    {
        return false;
    }
    qDebug() << "添加好友成功fname:"<<addFriendName;
    qDebug() << "添加好友成功lname:"<<loginName;
    QString sql = QString("insert into friend(id,friendId) values((select id from userInfo where name=\'%1\'),(select id from userInfo where name=\'%2\'))").arg(loginName).arg(addFriendName);
    QSqlQuery q;
    return q.exec(sql);
}

QStringList OperateDb::handleFlushFriend(const char *name)
{
    QStringList friendList;
    friendList.clear();

    if(NULL == name)
    {
        return friendList;
    }
//    QString sql = QString("select name from userInfo where isOnline = 0 and id = (select id from friend where friendId = (select id from userInfo where name = \'%1\'));").arg(name);
//    QSqlQuery q;
//    q.exec(sql);

//    while(q.next())
//    {
//        friendList.append(q.value(0).toString());
//        qDebug() << name<<"的好友有:"<<q.value(0).toString();
//    }

//    sql = QString("select name from userInfo where isOnline = 1 and id = (select friendId from friend where id = (select id from userInfo where name = \'%1\'))").arg(name);
//    q.exec(sql);
//    while(q.next())
//    {
//        friendList.append(q.value(0).toString());
//        qDebug() << name<<"的好友有:"<<q.value(0).toString();
//    }
    QString sql = QString("select friendId from friend where id = (select id from userInfo where name = \'%1'\)").arg(name);
    QSqlQuery q1, q2;
    q1.exec(sql);
    while(q1.next())
    {
        sql = QString("select name from userInfo where isOnline = 1 and id = %1").arg(q1.value(0).toInt());
        q2.exec(sql);
        if(q2.next())
        {
            friendList.append(q2.value(0).toString());
            qDebug() << name<<"的好友有:"<<q2.value(0).toString();
        }
    }

    sql = QString("select id from friend where friendId = (select id from userInfo where name = \'%1'\)").arg(name);
    q1.exec(sql);
    while(q1.next())
    {
        sql = QString("select name from userInfo where isOnline = 1 and id = %1").arg(q1.value(0).toInt());
        q2.exec(sql);
        if(q2.next())
        {
            friendList.append(q2.value(0).toString());
            qDebug() << name<<"的好友有:"<<q2.value(0).toString();
        }
    }
    return friendList;
}

bool OperateDb::handleDelFriend(const char *loginName, const char *friendName)
{
    if(NULL == loginName || NULL == friendName){
        return false;
    }
    qDebug()<<"删除好友的名字"<<friendName;
    QString sql = QString("delete from friend where id=(select id from userInfo where name=\'%1\') and friendId = (select id from userInfo where name = \'%2\');").arg(loginName).arg(friendName);
    QSqlQuery q;
    q.exec(sql);

    sql = QString("delete from friend where id=(select id from userInfo where name=\'%1\') and friendId = (select id from userInfo where name = \'%2\');").arg(friendName).arg(loginName);
    q.exec(sql);
    return true;
}
