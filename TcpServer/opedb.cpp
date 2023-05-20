#include "opedb.h"
#include <QMessageBox>
#include <QDebug>
OpeDB::OpeDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

OpeDB &OpeDB::getInstance()
{
    static OpeDB instance;
    return instance;
}

void OpeDB::init()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName(":/cloud.db");
    if(m_db.open())
    {

        QSqlQuery query;
        query.exec("select * from usrInfo");
        while (query.next())
        {


        }
    }
    else
    {

    }
}

OpeDB::~OpeDB()
{
    m_db.close();
}
