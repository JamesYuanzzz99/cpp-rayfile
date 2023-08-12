#include "sharefile.h"
#include <QCheckBox>
#include <widget.h>
#include <operatorwidget.h>
#include <QDebug>

ShareFile::ShareFile(QWidget *parent)
    : QWidget{parent}
{  

    sltAllBtn = new QPushButton("全选");     //全选
    cancelStlBtn = new QPushButton("取消选择");  //取消选择

    okBtn = new QPushButton("确定");         //确定分享选择的人
    cancelBtn = new QPushButton("取消");     //取消此次分享

    sa = new QScrollArea;
    friendW = new QWidget;
    friendWVbl = new QVBoxLayout(friendW);
    btnGroup = new QButtonGroup(friendW);   //放入friendW窗口里
    btnGroup->setExclusive(false);          //允许多选

    QHBoxLayout *tophbl = new QHBoxLayout;
    tophbl->addWidget(sltAllBtn);
    tophbl->addWidget(cancelStlBtn);
    tophbl->addStretch();  //放个弹簧让按钮在最左边

    QHBoxLayout *downhbl = new QHBoxLayout;
    downhbl->addWidget(okBtn);
    downhbl->addWidget(cancelBtn);

    QVBoxLayout *vbl = new QVBoxLayout;
    vbl->addLayout(tophbl);
    vbl->addWidget(sa);
    vbl->addLayout(downhbl);

    setLayout(vbl);
    resize(400,300);

    connect(cancelStlBtn, &QPushButton::clicked, this, &ShareFile::cancelSlt);

    connect(sltAllBtn, &QPushButton::clicked, this, &ShareFile::sltAll);

    connect(okBtn, &QPushButton::clicked, this, &ShareFile::okShare);

    connect(cancelBtn, &QPushButton::clicked, this, &ShareFile::cancelShare);
}

ShareFile &ShareFile::getInstance()
{
    static ShareFile instance;
    return instance;
}

void ShareFile::updateFriend(QListWidget *friendList)
{
    if(friendList == NULL)
    {
        return;
    }

    QAbstractButton* tmp = NULL;
    QList<QAbstractButton*> preFriendList = btnGroup->buttons();
    for(int i = 0; i < preFriendList.size(); i++)
    {
        tmp = preFriendList[i];
        friendWVbl->removeWidget(tmp);
        btnGroup->removeButton(tmp);
        preFriendList.removeOne(tmp);
        delete tmp;
        tmp = NULL;
    }
    QCheckBox *cb = NULL;
    qDebug() <<"分享文件时在线的好友有:" <<friendList->count() << "个";
    for(int i = 0; i < friendList->count(); i++)
    {
        cb = new QCheckBox(friendList->item(i)->text());
        qDebug() << "分享好友时能够分享给:" <<friendList->item(i)->text();
        friendWVbl->addWidget(cb);
        btnGroup->addButton(cb);
    }

    sa->setWidget(friendW);
}

void ShareFile::cancelSlt()
{
    QList<QAbstractButton*> cbList = btnGroup->buttons();
    QAbstractButton* tmp = NULL;

    for(int i = 0; i < cbList.size(); i++)
    {
        tmp = cbList[i];
        if(tmp->isChecked())
        {
            tmp->setChecked(false);
        }
    }
}

void ShareFile::sltAll()
{
    QList<QAbstractButton*> cbList = btnGroup->buttons();
    QAbstractButton* tmp = NULL;

    for(int i = 0; i < cbList.size(); i++)
    {
        tmp = cbList[i];
        if(!tmp->isChecked())
        {
            tmp->setChecked(true);
        }
    }
}

void ShareFile::okShare()
{
    //获得分享者
    QString loginName = Widget::getInstance().getLoginUserName();
    QString curPath = Widget::getInstance().getCurPath();
    QString shareFileName = OperatorWidget::getInstance().getBook()->getShareFileName();
    QString sharePath = curPath + "/" + shareFileName;  //分享路径
    qDebug() << "分享文件的路径:" << sharePath;

    QList<QAbstractButton*> cbList = btnGroup->buttons();
    QAbstractButton* tmp = NULL;
    int num = 0;
    for(int i = 0; i < cbList.size(); i++)
    {
        tmp = cbList[i];
        if(tmp->isChecked())
        {
            num++;
            qDebug() << "分享---" <<  shareFileName << "给->" << tmp->text();
        }
    }

    PDU *pdu = mkPDU(32*num + sharePath.size()+32);
    pdu->uiMsgType = SHAREFILE_RESQUEST;
    sprintf(pdu->caData, "%s %d", loginName.toStdString().c_str(), num);       //分享者和分享人数
    qDebug() << "分享人和所分享的人数为:" << pdu->caData;
    int j = 0;
    for(int i = 0; i < cbList.size(); i++)
    {
        tmp = cbList[i];
        if(tmp->isChecked())
        {
            strncpy((pdu->caMsg)+j*32, tmp->text().toStdString().c_str(), 32);//将所分享的人的名字拷贝进去
            qDebug() << "分享给了:" << ((pdu->caMsg)+j*32);
            j++;

        }
//        if(j == num)
//        {
//            break;
//        }
    }

    strncpy((pdu->caMsg)+num*32, sharePath.toStdString().c_str(), sharePath.size()+32); //拷贝分享文件的路径

    Widget::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

    free(pdu);
    pdu = NULL;
}

void ShareFile::cancelShare()
{
    this->hide();
}
