#include "widget.h"
#include <QApplication>
//#include <online.h>
//#include <friend.h>
//#include <operatorwidget.h>
#include <sharefile.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
    Widget::getInstance().show();
//    Online w;
//    w.show();
//    Friend w;
//    w.show();
//    OperatorWidget w;
//    w.show();
//    ShareFile w;
//    w.show();
    return a.exec();
}
