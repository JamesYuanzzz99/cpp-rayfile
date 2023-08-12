#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QScrollArea>
#include <QListWidget>

class ShareFile : public QWidget
{
    Q_OBJECT
public:
    explicit ShareFile(QWidget *parent = nullptr);

    static ShareFile &getInstance();

    void updateFriend(QListWidget *friendList);

signals:

public slots:
    void cancelSlt();       //取消选择
    void sltAll();          //全选

    void okShare();
    void cancelShare();

private:
    QPushButton *sltAllBtn;     //全选
    QPushButton *cancelStlBtn;  //取消选择

    QPushButton *okBtn;         //确定分享选择的人
    QPushButton *cancelBtn;     //取消此次分享

    QScrollArea * sa;
    QWidget *friendW;
    QButtonGroup *btnGroup;

    QVBoxLayout *friendWVbl;
};

#endif // SHAREFILE_H
