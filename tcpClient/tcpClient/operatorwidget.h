#ifndef OPERATORWIDGET_H
#define OPERATORWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <friend.h>
#include <book.h>
#include <QStackedWidget>
class OperatorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OperatorWidget(QWidget *parent = nullptr);
    static OperatorWidget &getInstance();
    Friend *getFriend();
    Book *getBook();

signals:

private:
    QListWidget *listW;
    Friend *frd;
    Book *book;
    QStackedWidget *stackWidget;
};

#endif // OPERATORWIDGET_H
