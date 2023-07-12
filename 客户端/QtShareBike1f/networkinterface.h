#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#include <jni.h>
#include <QTimer>
#include <QTcpSocket>
#include "global_def.h"

class networkInterface : public QObject
{
    Q_OBJECT
private:
    networkInterface(QObject* parent = nullptr);

public:
    void startScanQRCode(QString name);        //扫描二维码

    static void registerNatives();     //注册java类，便于java中调用Qt的方法
    static void sendDataToQt(JNIEnv *env, jobject type, jstring jdata);     //主要是对扫码的响应
    static networkInterface* getInstance();       //单例//////////////////////////////////////////////

public:
    void initMyNumber();         //初始化数据成员
    void connectServer();        //连接服务器

    void setTcpClient(QTcpSocket *tcpclient);        //设置tcp套接字
    void ReadData();             //读套接字数据
    void ReadMessageFromServer(u32 id);

signals:
    /*
        这个信号是从qt端发出的，与HTML客户端交互
    */
    void ServerConnectSuccessSig();             //连接服务器成功信号
    void ServerConnectFalseSig(QString msg);    //连接服务器失败信号

    void msgTipPrintToHtmlSig(QString tipMsg);  //打印消息提示

    void addBikeResSig(const int bikeId, const double longitude, const double latitude);       //添加单车响应
    void deleteBikeResSig(const int bikeId);              //删除单车响应
    void sendBikeStartResSig();         //开始骑行单车响应
    void sendBikeEndResSig();           //结束骑行响应
    void bikeInfoBroadcastResSig(const int bikeid, const int status, const double longitude, const double latitude);  //广播响应
    void ListAllBikeMsgSig(const int bikeId, const int status, const double longitude, const double latitude);   //例举单车信息响应

    void isSuccess(bool success);   //该信号不能删除
    //void bikeInfoBroadcastReqSig(const int bikeId, const int status, const double longitude, const double latitude);

public slots:      //这里一定要是public slot, bikeManager.html、map.html内需要调用部分槽（下方带*、**号函数）
    //注意：在与html的交互中不能使用i32，u16等类型的参数传入，因为javascript没有这些类型，导致无法识别
    /*
        从HTML客户端调用这个槽函数，实现与qt端的交互.
    */
    void scanQRRequest(int eventID, int bikeID, double longitude, double latitude);      //处理二维码请求
    void ReadError(QAbstractSocket::SocketError);            //套接字出错

    void getPermission();                 //获取权限(调用Java类方法) **
    void connectBikeServer();             //*
    void connectBikeServer_test();
    void quiteBikeClient();               //退出单车客户端 *
    void sendRequestToGetAllBikeMsg();    //*

    void addBikeSlot(int bikeId, double longitude, double latitude);    //发出添加单车请求 *
    void deleteBikeSlot(int bikeId);       //发出删除单车请求 *
    void openScanCodeWindow(int bikeId, double longitude, double latitude);    //调用系统相机   //**
    void sendBroadcastRequest(int bikeId, int status, double longitude, double latitude);     //**

private:
    static networkInterface* m_instance;    //单例/////////////////////////////////////////////////
    static QTcpSocket* m_tcpClient;
    static QString m_userName;

    QTimer* m_tipTimer;   //计时器
    int countTime;

    long m_rideStartTime;
    long m_rideEndTime;
    int m_bikeId;
    double m_longitude;    //经度
    double m_latitude;     //纬度

    char* m_sendMessage;   //发送数据信息
    char* m_readMessage;   //接收数据信息
    int m_readMsgLen;
    int m_sendMsgLen;
};

#endif // NETWORKINTERFACE_H
