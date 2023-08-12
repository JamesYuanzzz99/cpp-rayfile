#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <protocol.h>
#include <QTimer>
#include <QProgressBar>
class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = nullptr);
    void updateFileList(const PDU *pdu);
    void setDownloadStatus(bool status);
    bool getDownloadStatus();
    QString getSavePath();
    QProgressBar* getProgressBar();
    QString getShareFileName();

    qint64 total;       //下载的文件的总大小
    qint64 recvSize;    //下载的文件已经接收的大小

signals:

public slots:
    void createDir();
    void flushDir();
    void delDir();
    void renameDir();
    void enterDir(const QModelIndex &index);
    void changeDir(const PDU *pdu);             //进入新的文件夹并且刷新文件夹
    void back();
    void uploadFile();      //发送上传文件请求给服务器
    void uploadFileData();  //发送上传文件的内容给服务器
    void delFile();         //删除常规文件
    void downloadFile();    //下载文件
    void shareFile();
    void moveFile();
    void sltDestDir();

private:
    QListWidget *bookList;
    QPushButton *backBtn;
    QPushButton *createDirBtn;
    QPushButton *delDirBtn;
    QPushButton *renameDirBtn;
    QPushButton *flushDirBtn;
    QPushButton *selectDestDirBtn;
    QPushButton *uploadFileBtn;
    QPushButton *delFileBtn;
    QPushButton *downloadFileBtn;
    QPushButton *shareFileBtn;
    QPushButton *renameFileBtn;
    QPushButton *moveFileBtn;


    QString savePath;
    QString uploadFilePath;
    QTimer *timer;

    QProgressBar *proBar;

    bool dlStatus;

    QString shareFileName;

    QString moveFileName;
    QString moveFilePath;
    QString destDir;
};

#endif // BOOK_H
