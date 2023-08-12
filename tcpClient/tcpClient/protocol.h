#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERV_DIR "F:/Code/Qt project/wangpan/servDir"


#define RESGIST_OK "regist ok"
#define RESGIST_FAILED "regist failed: username existed"

#define LOGIN_OK "登录成功"
#define LOGIN_FAILED "登录失败: 用户名或密码错误或重复登录!"

#define SEARCH_USER_ONLINE "查找的用户在线!"
#define SEARCH_USER_OFFLINE "查找的用户不在线!"
#define SEARCH_USER_NO      "查找的用户不存在!"

#define UNKNOW_ERROR    "未知的错误!"
#define ADDFRIEND_EXIST    "对方已经是好友!"
#define ADDFRIEND_OFFLINE "添加的用户不在线!"
#define ADDFRIEND_NOEXIST "添加的用户不存在!"

#define DELFRIEND_OK    "删除好友成功!"

#define DIR_NOTEXISTS "当前目录不存在!"
#define DIRNAME_EXISTS "文件夹名已存在!"
#define CREATEDIR_OK    "文件夹创建成功!"
#define DELDIR_OK       "删除文件夹成功!"
#define DELDIR_FAILED   "删除失败: 这是一个文件!"
#define RENAMEDIR_FAILED    "重命名失败!输入非法或已存在"
#define RENAMEDIR_OK    "重命名成功!"
#define ENTERDIR_OK     "进入文件夹!"
#define OPENFILE_OK     "打开文件!"

#define UPLOADFILE_OK   "上传成功!"
#define UPLOADFILE_FAILED   "上传失败!"

#define DELFILE_OK      "删除成功!"
#define DELFILE_FAILED  "删除失败!"
typedef unsigned int uint;

enum msgType{
    Min = 0,
    REGIST_REQUEST,     //注册请求
    REGIST_RESPOND,     //注册回复
    LOGIN_REQUEST,      //登录请求
    LOGIN_RESPOND,      //登录回复

    ALLONLINE_REQUEST,  //查询所有在线用户请求
    ALLONLINE_RESPOND,  //查询所有在线用户回复

    SEARCHUSER_REQUEST, //搜索用户请求
    SEARCHUSER_RESPOND, //搜索用户回复

    ADDFRIEND_REQUEST,  //加好友请求
    ADDFRIEND_RESPOND,  //加好友回复

    ADDFRIEND_AGREE,    //同意添加好友
    ADDFRIEND_REFUSE,   //拒绝添加好友

    FLUSHFRIEND_REQUEST,//刷新好友请求
    FLUSHFRIEND_RESPOND,//刷新好友回复

    DELFRIEND_REQUEST,  //删除好友请求
    DELFRIEND_RESPOND,  //删除好友回复

    PRIVATECHAT_REQUEST,  //私聊请求
    PRIVATECHAT_RESPOND,  //私聊回复

    GROUPCHAT_REQUEST,  //群聊请求
    GROUPCHAT_RESPOND,  //群聊回复

    CREATE_DIR_REQUEST, //创建文件夹请求
    CREATE_DIR_RESPOND, //创建文件夹回复

    FLUSHDIR_REQUEST,   //刷新文件夹请求
    FLUSHDIR_RESPOND,   //刷新文件夹回复

    DELDIR_REQUEST,   //删除文件夹请求
    DELDIR_RESPOND,   //删除文件夹回复

    RENAMEDIR_REQUEST,   //重命名文件夹请求
    RENAMEDIR_RESPOND,   //重命名文件夹回复

    ENTERDIR_REQUEST,   //进入文件夹请求
    ENTERDIR_RESPOND,   //进入文件夹回复

    UPLOADFILE_REQUEST, //上传文件请求
    UPLOADFILE_RESPOND, //上传文件回复

    DELFILE_REQUEST,    //删除常规文件请求
    DELFILE_RESPOND,    //删除常规文件回复

    DOWNLOAD_RESQUEST,  //下载文件请求
    DOWNLOAD_RESPOND,   //下载文件回复

    SHAREFILE_RESQUEST,  //分享文件请求
    SHAREFILE_RESPOND,   //分享文件回复
    SHAREFILE_NOTE,      //共享文件的通知
    SHAREFILE_NOTE_RESPOND,//共享文件的通知的回复

    MOVEFILE_REQUEST,    //移动文件请求
    MOVEFILE_RESPOND,    //移动文件回复
    Max = 0x00ffffff
};

struct FileInfo{
    char fileName[64];
    int fileType;
};

struct PDU{
    uint uiPDULen;  //总的协议数据单元大小
    uint uiMsgType; //消息类型
    char caData[64];
    uint uiMsgLen;  //实际消息长度
    char caMsg[];    //实际消息
};

PDU* mkPDU(uint uiMsgLen);
#endif // PROTOCOL_H
