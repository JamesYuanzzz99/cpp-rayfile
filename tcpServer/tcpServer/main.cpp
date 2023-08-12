#include "tcpserver.h"
#include <operatedb.h>
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    OperateDb::getInstance().init();
    TcpServer w;
    w.show();
    return a.exec();
}
