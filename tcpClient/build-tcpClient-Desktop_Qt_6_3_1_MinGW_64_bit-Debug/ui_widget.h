/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *password;
    QLabel *username;
    QLineEdit *userLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginBtn;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *registBtn;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelBtn;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(412, 181);
        verticalLayout_2 = new QVBoxLayout(Widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        password = new QLabel(Widget);
        password->setObjectName(QString::fromUtf8("password"));

        gridLayout->addWidget(password, 1, 0, 1, 1);

        username = new QLabel(Widget);
        username->setObjectName(QString::fromUtf8("username"));

        gridLayout->addWidget(username, 0, 0, 1, 1);

        userLineEdit = new QLineEdit(Widget);
        userLineEdit->setObjectName(QString::fromUtf8("userLineEdit"));

        gridLayout->addWidget(userLineEdit, 0, 1, 1, 1);

        passwordLineEdit = new QLineEdit(Widget);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordLineEdit, 1, 1, 1, 1);

        loginBtn = new QPushButton(Widget);
        loginBtn->setObjectName(QString::fromUtf8("loginBtn"));

        gridLayout->addWidget(loginBtn, 2, 0, 1, 2);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        registBtn = new QPushButton(Widget);
        registBtn->setObjectName(QString::fromUtf8("registBtn"));

        horizontalLayout->addWidget(registBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        horizontalLayout_2->addLayout(horizontalLayout);

        cancelBtn = new QPushButton(Widget);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));

        horizontalLayout_2->addWidget(cancelBtn);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        password->setText(QCoreApplication::translate("Widget", "\345\257\206\347\240\201:", nullptr));
        username->setText(QCoreApplication::translate("Widget", "\347\224\250\346\210\267\345\220\215:", nullptr));
        loginBtn->setText(QCoreApplication::translate("Widget", "\347\231\273\345\275\225", nullptr));
        registBtn->setText(QCoreApplication::translate("Widget", "\346\263\250\345\206\214", nullptr));
        cancelBtn->setText(QCoreApplication::translate("Widget", "\346\263\250\351\224\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
