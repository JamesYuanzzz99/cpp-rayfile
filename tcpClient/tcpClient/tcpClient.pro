QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    book.cpp \
    friend.cpp \
    main.cpp \
    online.cpp \
    operatorwidget.cpp \
    privatechat.cpp \
    protocol.cpp \
    sharefile.cpp \
    widget.cpp

HEADERS += \
    book.h \
    friend.h \
    online.h \
    operatorwidget.h \
    privatechat.h \
    protocol.h \
    sharefile.h \
    widget.h

FORMS += \
    online.ui \
    privatechat.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../icon/icon.qrc \
    ../res/res.qrc
