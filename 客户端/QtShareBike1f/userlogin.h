#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QObject>
#include <QAbstractSocket>
#include "global_def.h"
#include <QTcpSocket>

class UserLogin : public QObject
{
    Q_OBJECT
public:
    explicit UserLogin(QObject *parent = nullptr);
    ~UserLogin();

    void connectToServer();        //连接到服务器
    void sendMsgToServer(EventID_t eventID);    //给服务器发送数据

    void openQmlMsgWindow(QString msg);      //打开消息提示框
    void refreshLoginWindow();               //刷新登录页面
    void openPhoneNotifyWindow(QString notifyMsg);      //手机提示窗口

    void distroyFreeMemory();
signals:
    //发送信号给地图界面
    void sendloginSigToMapwindow(QString userName);

    //qml发送的信号
    void getVerificationCode_sig(const QString& userName);      //获取验证码按钮点击
    void registerUser_sig(const QString& userName, const QString& password, const QString& verCode);     //注册按钮点击
    void loginToBikeClient_sig(const QString& userName, const QString& password);    //登录

    void connectToServer_sig();        //连接服务器
    void reconnectToServer_sig();      //重连服务器

private slots:
    void getVerificationCode_slot(const QString& userName);     //获取验证码
    void registerUser_slot(const QString& userName, const QString& password, const QString& verCode);   //响应注册
    void loginToBikeClient_slot(const QString& userName, const QString& password);    //响应登录

    void connectToServer_slot();         //连接到服务器
    void reconnectToServer_slot();       //重新连接到服务器

    //客户端tcp监听槽函数
    void ReadData();          //读取数据
    void ReadError(QAbstractSocket::SocketError);    //连接出错

public:
    static UserLogin* getInstance();
    static QObject* getQmlObject();
    static QString getUserName();

    //调用qml内的函数接口
    static void setQmlObject(QObject* qmlObject);     //设置Qml对象
    static void openQmlMainPage();                    //打开主页面
    static void openQmlBikeManagePage();              //打开管理页面
    static void openQmlLoginPage();                   //打开登录页面

    static QTcpSocket* getTcpClient();   //获取客户端tcp套接字
private:
    static QTcpSocket* m_tcpClient;     //Tcp通信套接字
    static QString m_userName;          //用户名
    static QString m_userPwd;           //密码
    static i32 m_verCode;               //验证码
    static UserLogin* m_instance;       //单例
    static QObject* m_qmlObject;        //qml对象
};

#endif // USERLOGIN_H
