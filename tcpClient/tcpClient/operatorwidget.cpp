#include "operatorwidget.h"
#include <widget.h>
OperatorWidget::OperatorWidget(QWidget *parent)
    : QWidget{parent}
{
    listW = new QListWidget(this);
    listW->addItem("好友");
    listW->addItem("我的文件");
    frd = new Friend(this);
    book = new Book(this);
    stackWidget = new QStackedWidget(this);
    stackWidget->addWidget(frd);
    stackWidget->addWidget(book);

    QHBoxLayout *mainHBL = new QHBoxLayout(this);
    mainHBL->addWidget(listW);
    mainHBL->addWidget(stackWidget);

    setLayout(mainHBL);

    //关联左边列表与堆栈窗口的对应关系,点击好友显示好友窗口点击图书显示图书窗口
    connect(listW, &QListWidget::currentRowChanged, stackWidget, &QStackedWidget::setCurrentIndex);

    connect(listW, &QListWidget::currentRowChanged, this, [=](){
        QListWidgetItem *item =  listW->currentItem();
        if(item->text() == "我的文件")
        {
            getBook()->flushDir();
        }
    });

    resize(800,400);
    setWindowTitle(Widget::getInstance().getLoginUserName());
    setWindowIcon(QIcon(":/icon/file.png"));
}

OperatorWidget &OperatorWidget::getInstance()
{
    static OperatorWidget instance;
    return instance;
}

Friend *OperatorWidget::getFriend()
{
    return frd;
}

Book *OperatorWidget::getBook()
{
    return book;
}
