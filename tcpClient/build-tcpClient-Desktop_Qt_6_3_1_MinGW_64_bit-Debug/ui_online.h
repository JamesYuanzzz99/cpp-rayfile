/********************************************************************************
** Form generated from reading UI file 'online.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONLINE_H
#define UI_ONLINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Online
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QListWidget *onlineListW;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *addFriendBtn;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *Online)
    {
        if (Online->objectName().isEmpty())
            Online->setObjectName(QString::fromUtf8("Online"));
        Online->resize(370, 402);
        verticalLayout_2 = new QVBoxLayout(Online);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        onlineListW = new QListWidget(Online);
        new QListWidgetItem(onlineListW);
        new QListWidgetItem(onlineListW);
        new QListWidgetItem(onlineListW);
        new QListWidgetItem(onlineListW);
        onlineListW->setObjectName(QString::fromUtf8("onlineListW"));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font.setPointSize(16);
        onlineListW->setFont(font);

        horizontalLayout->addWidget(onlineListW);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        addFriendBtn = new QPushButton(Online);
        addFriendBtn->setObjectName(QString::fromUtf8("addFriendBtn"));
        QFont font1;
        font1.setPointSize(16);
        addFriendBtn->setFont(font1);

        verticalLayout->addWidget(addFriendBtn);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(Online);

        QMetaObject::connectSlotsByName(Online);
    } // setupUi

    void retranslateUi(QWidget *Online)
    {
        Online->setWindowTitle(QCoreApplication::translate("Online", "Form", nullptr));

        const bool __sortingEnabled = onlineListW->isSortingEnabled();
        onlineListW->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = onlineListW->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("Online", "1111", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = onlineListW->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("Online", "2222", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = onlineListW->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("Online", "3333", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = onlineListW->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("Online", "4444", nullptr));
        onlineListW->setSortingEnabled(__sortingEnabled);

        addFriendBtn->setText(QCoreApplication::translate("Online", "\345\212\240\345\245\275\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Online: public Ui_Online {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONLINE_H
