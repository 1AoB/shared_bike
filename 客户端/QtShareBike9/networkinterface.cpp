#include "networkinterface.h" // networkinterface 网络接口类
#include <QDebug>
#include "bike.pb.h"
#include <QAndroidJniObject>
#include <QDateTime>
#include "userlogin.h"

networkInterface* networkInterface::m_instance = nullptr;
QTcpSocket* networkInterface::m_tcpClient = nullptr;
QString networkInterface::m_userName = "123456789";

networkInterface::networkInterface(QObject* parent):QObject(parent)
{
    initMyNumber();         //初始化数据成员
    registerNatives();      //注册java类，便于java中调用Qt的方法
}


void networkInterface::connectServer()
{
    if(!m_tcpClient) m_tcpClient = new QTcpSocket;   //实例化
    m_tcpClient->abort();                    //终止当前连接并重置套接字, 这个函数会立即关闭套接字，丢弃写缓冲区中任何挂起的数据
    m_tcpClient->connectToHost(shareBikeServer_IP, shareBikeServer_port);

    if(m_tcpClient->waitForConnected(2000))  //等待直到套接字被连接，最长可达2000毫秒
    {
        connect(m_tcpClient, &QTcpSocket::readyRead, this, &networkInterface::ReadData);
        connect(m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ReadError(QAbstractSocket::SocketError)));
        emit ServerConnectSuccessSig();
    }
    else
    {
        delete  m_tcpClient;
        m_tcpClient = nullptr;
        emit ServerConnectFalseSig("无法连接到共享单车服务器");
    }
}

void networkInterface::startScanQRCode(QString name)
{
    #ifdef ANDROID
    QAndroidJniObject javaAction = QAndroidJniObject::fromString(name);
    QAndroidJniObject::callStaticMethod<void>
            (
                "an/qt/qtWithJava/MyJavaClass",
                "scanQRCode",
                "(Ljava/lang/String;)V",
                javaAction.object<jstring>()      //返回由QAndroidJniObject持有的jstring类型的对象
            );
    #endif
}

//注册java类，便于java中调用Qt的方法
void networkInterface::registerNatives()
{
    /*    JNINativeMethod结构体
    第一个变量name是Java中函数的名字。
    第二个变量signature，用字符串是描述了Java中函数的参数和返回值
    第三个变量fnPtr是函数指针，指向native函数。前面都要接 (void*)
    第一个变量与第三个变量是对应的，一个是java层方法名，对应着第三个参数的native方法名字
     */
    JNINativeMethod methods[] = {"SendDataToQt", "(Ljava/lang/String;)V", (void*)sendDataToQt};

    QAndroidJniEnvironment env;      //提供了对JNI环境的访问
    const char* mClassName = "an/qt/qtWithJava/MyJavaClass";
    jclass j_class;

    j_class = env->FindClass(mClassName);     //使用所有可用的类加载器搜索className  //返回类指针，如果没有找到则返回null
    if(j_class == nullptr)
    {
        qDebug() << "error j_class";
        return;
    }

    jint mj = env->RegisterNatives(j_class, methods, sizeof(methods) / sizeof(methods[0]));
    if(mj != JNI_OK) qDebug()<<"env->RegisterNatives false!";

    if(env->ExceptionCheck()) env->ExceptionClear(); //operator-> //提供对QAndroidJniEnvironment的JNIEnv指针的访问
}

void networkInterface::sendDataToQt(JNIEnv *env, jobject type, jstring jdata)
{

    if(!m_tcpClient)
    {
        networkInterface::getInstance()->connectServer();
        if(!m_tcpClient) return;       //连接失败
    }

    const char* cdata = env->GetStringUTFChars(jdata, 0);    //将Java的String转为C的字符串
    QString qdata(cdata);

    QStringList dataSplit = qdata.split("-");

    //当找不到分隔符时，返回全部的字符串，保存在.at(0)中
    if(dataSplit.at(0) == qdata)
    {
        emit m_instance->msgTipPrintToHtmlSig("您扫的码不是共享单车的二维码哦！");
    }
    else if(dataSplit.at(0) == "#bike" && dataSplit.at(1) == QString::number(m_instance->m_bikeId))
    {
        m_instance->scanQRRequest(EEVENTID_SCANQRSTART_REQ, m_instance->m_bikeId, m_instance->m_longitude, m_instance->m_latitude);
    }
    else if(dataSplit.at(0) == "#bike")
    {
        emit m_instance->msgTipPrintToHtmlSig("您扫的码不是这台共享单车的二维码哦！");
    }
    else
    {
        emit m_instance->msgTipPrintToHtmlSig("无法识别此二维码！");
    }
}

networkInterface *networkInterface::getInstance()
{
    if(m_instance == nullptr) m_instance = new networkInterface;
    return m_instance;
}

void networkInterface::initMyNumber()
{
    m_tcpClient = nullptr;
    m_tipTimer = new QTimer(this);
    countTime = 0;

    m_longitude = 0;       //经度
    m_latitude = 0;        //维度
    m_rideStartTime = 0;
    m_rideEndTime = 0;

    m_sendMsgLen = 0;
    m_readMsgLen = 0;
    m_sendMessage = nullptr;
    m_readMessage = nullptr;
}

void networkInterface::ReadData()
{
    m_readMessage = new char[1024];
    int readLen = m_tcpClient->read(m_readMessage, 1024);
    m_readMessage[readLen] = '\0';

    qDebug() << "ReadData->readLen: " << readLen;
    if(readLen != -1)
    {
        if(strncmp(m_readMessage, "FBEB", 4) == 0)
        {
            u16 id = *(u16*)(m_readMessage + 4);
            m_readMsgLen = *(i32*)(m_readMessage + 6);
            ReadMessageFromServer(id);
        }
        else
        {
            emit msgTipPrintToHtmlSig("校验信息错误(!=\"FBEB\")");
        }
    }
}

void networkInterface::ReadMessageFromServer(u32 id)
{
    switch (id)
    {

    //响应请求

    case EEVENTID_ADDBIKE_RSP:         //添加单车响应
    {
        tutorial::addBike_response ar;
        ar.ParseFromArray(m_readMessage + 10, m_readMsgLen);

        if(ar.rescode() == ERRC_SUCCESS)
        {
            int bikeID = ar.bikeid();
            //msgTipPrintToHtmlSig(QString("开始添加单车[%1]").arg(bikeID));
            emit addBikeResSig(bikeID, ar.longitude(), ar.latitude());
        }
        else
        {
            emit msgTipPrintToHtmlSig(QString("添加单车请求失败！错误码：%1").arg(ar.rescode()));
        }
        break;
    }

    case EEVENTID_DELETEBIKE_RSP:   //删除单车响应
    {
        tutorial::deleteBike_response dr;
        dr.ParseFromArray(m_readMessage + 10, m_readMsgLen);

        if(dr.rescode() == ERRC_SUCCESS)
        {
            int bikeID = dr.bikeid();
            emit deleteBikeResSig(bikeID);
        }
        else
        {
            emit msgTipPrintToHtmlSig(QString("删除单车请求失败！错误码：%1").arg(dr.rescode()));
        }
        break;
    }

    case EEVENTID_SCANQRSTART_PSP:        //单车扫码开始骑行响应
    {
        tutorial::bike_scanQR_start_response bssr;
        bssr.ParseFromArray(m_readMessage + 10, m_readMsgLen);
        if(bssr.rescode() == ERRC_SUCCESS)
        {
            m_rideStartTime = bssr.timestamp();
            emit sendBikeStartResSig();
        }
        else
        {
            emit msgTipPrintToHtmlSig(QString("扫码请求骑行失败!错误码：%1").arg(bssr.rescode()));
        }
        break;
    }

    case EEVENTID_SCANQREND_PSP:          //单车扫码结束骑行响应
    {
        tutorial::bike_scanQR_end_response bser;
        bser.ParseFromArray(m_readMessage + 10, m_readMsgLen);
        if(bser.rescode() == ERRC_SUCCESS)
        {
            m_rideEndTime = bser.timestamp();
            emit sendBikeEndResSig();
        }
        else
        {

            emit msgTipPrintToHtmlSig(QString("扫码还车请求失败!错误码：%1").arg(bser.rescode()));
        }
        break;
    }

    case EEVENTID_FAULT_PSP:             //单车扫码故障报修响应
    {
        tutorial::bike_fault_response bfr;
        bfr.ParseFromArray(m_readMessage + 10, m_readMsgLen);
        qDebug() << "bser.code():" << bfr.rescode();
        if(bfr.rescode() == ERRC_SUCCESS)
        {
            emit msgTipPrintToHtmlSig(QString("单车扫码故障报修响应成功!"));
        }
        break;
    }

    case EEVENTID_REPAIRFINISH_PSP:       //单车扫码修复完成响应
    {
        tutorial::bike_repair_finish_response brfr;
        brfr.ParseFromArray(m_readMessage + 10, m_readMsgLen);
        qDebug() << "brfr.code():" << brfr.rescode();
        if(brfr.rescode() == ERRC_SUCCESS)
        {
            emit msgTipPrintToHtmlSig(QString("单车扫码修复完成响应成功!"));
        }
        break;
    }

    case EEVENTID_BROADCAST_MSG_RSP:
    {
        tutorial::broadcast_msg_response bmr;
        bmr.ParseFromArray(m_readMessage + 10, m_readMsgLen);
        emit bikeInfoBroadcastResSig(bmr.bikeid(), bmr.bikestatus(), bmr.longitude(), bmr.latitude());
        break;
    }

    case EEVENTID_LIST_BIKEINFO_RSP:       //例举单车信息响应
    {
        tutorial::list_all_bikeinfo_response labr;
        labr.ParseFromArray(m_readMessage + 10, m_readMsgLen);
        if(labr.rescode() != ERRC_SUCCESS)
        {
            emit msgTipPrintToHtmlSig("请求错误，单车信息同步失败！");
            break;
        }

        int bikeNum = labr.records_size();
        qDebug() << "bikeNum:" << bikeNum;
        for(int i = 0; i < bikeNum; i++)
        {
            const tutorial::list_all_bikeinfo_response_bikeinfo& bikeinfo = labr.records(i);
            emit ListAllBikeMsgSig(bikeinfo.bikeid(), bikeinfo.bikestatus(), bikeinfo.longitude(), bikeinfo.latitude());
        }
        break;
    }
    default:         //other
        break;
    }
}

void networkInterface::setTcpClient(QTcpSocket *tcpclient)
{
    if(!tcpclient) return;
    qDebug() << "tcpClient" << tcpclient;
    m_tcpClient = tcpclient;
    qDebug() << "setTcpClient(QTcpSocket* tcpclient)this->m_tcpClient:" << this->m_tcpClient;

    connect(m_tcpClient, &QTcpSocket::readyRead, this, &networkInterface::ReadData);
    connect(m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ReadError(QAbstractSocket::SocketError)));
}

void networkInterface:: scanQRRequest(int eventID, int bikeID, double longitude, double latitude)
{
    if(!m_tcpClient)
    {
        connectServer();
        if(!m_tcpClient) return;
    }

    if(!m_sendMessage) m_sendMessage = new char[1024];
    QDateTime time = QDateTime::currentDateTime();     //获取当前时间
    int timeT = time.toTime_t();      //将当前时间转换为时间戳
    std::string userName = m_userName.toStdString();

    switch(eventID)
    {
    case EEVENTID_SCANQRSTART_REQ:             //单车扫码开始骑行请求
    {
        tutorial::bike_scanQR_start_request bssr;
        bssr.set_username(userName);
        bssr.set_bikeid(bikeID);
        bssr.set_longitude(longitude);
        bssr.set_latitude(latitude);
        bssr.set_timestamp(timeT);
        m_sendMsgLen = bssr.ByteSizeLong();    //发送数据长度

        memcpy(m_sendMessage, "FBEB", 4);
        *(u16*)(m_sendMessage + 4) = eventID;     //EEVENTID_SCANQRSTART_REQ
        *(i32*)(m_sendMessage + 6) = m_sendMsgLen;
        bssr.SerializeToArray(m_sendMessage + 10, m_sendMsgLen);
        break;
    }

    case EEVENTID_SCANQREND_REQ:               //单车扫码结束骑行请求
    {
        tutorial::bike_scanQR_end_request bser;
        bser.set_username(userName);
        bser.set_bikeid(bikeID);
        bser.set_longitude(longitude);
        bser.set_latitude(latitude);
        bser.set_timestamp(timeT);
        m_sendMsgLen = bser.ByteSizeLong();

        memcpy(m_sendMessage, "FBEB", 4);
        *(u16*)(m_sendMessage + 4) = eventID;       //EEVENTID_SCANQREND_REQ
        *(i32*)(m_sendMessage + 6) = m_sendMsgLen;
        bser.SerializeToArray(m_sendMessage + 10, m_sendMsgLen);
        break;
    }

    case EEVENTID_FAULT_REQ:                   //单车扫码故障报修请求
    {
        tutorial::bike_fault_request bfr;
        bfr.set_username(userName);
        bfr.set_bike_id(bikeID);
        bfr.set_longitude(longitude);
        bfr.set_latitude(latitude);
        bfr.set_timestamp(timeT);
        m_sendMsgLen = bfr.ByteSizeLong();

        memcpy(m_sendMessage, "FBEB", 4);

        *(u16*)(m_sendMessage + 4) = eventID;     //EEVENTID_FAULT_REQ
        printf("*(u16*)(msg + 4) = %d\n", *(u16*)(m_sendMessage + 4));
        *(i32*)(m_sendMessage + 6) = m_sendMsgLen;
        printf("*(i32*)(msg + 6) = %d\n", *(i32*)(m_sendMessage + 6));
        bfr.SerializeToArray(m_sendMessage + 10, m_sendMsgLen);
        break;
    }

    case EEVENTID_REPAIRFINISH_REQ:            //单车扫码修复完成请求
    {
        tutorial::bike_repair_finish_request brfr;
        brfr.set_username(userName);
        brfr.set_bike_id(bikeID);
        brfr.set_longitude(longitude);
        brfr.set_latitude(latitude);
        brfr.set_timestamp(timeT);
        m_sendMsgLen = brfr.ByteSizeLong();

        memcpy(m_sendMessage, "FBEB", 4);

        *(u16*)(m_sendMessage + 4) = eventID;      //EEVENTID_REPAIRFINISH_REQ
        printf("*(u16*)(msg + 4) = %d\n", *(u16*)(m_sendMessage + 4));
        *(i32*)(m_sendMessage + 6) = m_sendMsgLen;
        printf("*(i32*)(msg + 6) = %d\n", *(i32*)(m_sendMessage + 6));
        brfr.SerializeToArray(m_sendMessage + 10, m_sendMsgLen);
        break;
    }

    case EEVENTID_ADDBIKE_REQ:                //添加单车请求
    {
        tutorial::addBike_request ar;
        ar.set_bikeid(bikeID);
        ar.set_longitude(longitude);
        ar.set_latitude(latitude);
        m_sendMsgLen = ar.ByteSizeLong();

        memcpy(m_sendMessage, "FBEB", 4);
        *(u16*)(m_sendMessage + 4) = eventID;      //EEVENTID_ADDBIKE_REQ
        *(i32*)(m_sendMessage + 6) = m_sendMsgLen;
        ar.SerializeToArray(m_sendMessage + 10, m_sendMsgLen);
        break;
    }

    case EEVENTID_DELETEBIKE_REQ:                //删除单车请求
    {
        tutorial::deleteBike_request dr;
        dr.set_bikeid(bikeID);

        m_sendMsgLen = dr.ByteSizeLong();
        memcpy(m_sendMessage, "FBEB", 4);

        *(u16*)(m_sendMessage + 4) = eventID;    //EEVENTID_DELETEBIKE_REQ
        *(i32*)(m_sendMessage + 6) = m_sendMsgLen;
        dr.SerializeToArray(m_sendMessage + 10, m_sendMsgLen);
        break;
    }

    default:          //other
        break;
    }

    if(m_tcpClient->write(m_sendMessage, m_sendMsgLen + 10) != -1)
    {
        qDebug() << "请求[" << eventID << "]success!";
    }

    m_tcpClient->flush();
    m_sendMsgLen = 0;
    delete [] m_sendMessage;
    m_sendMessage = nullptr;
}

void networkInterface::ReadError(QAbstractSocket::SocketError)
{
    emit msgTipPrintToHtmlSig("与服务器连接断开！");
    if(m_tcpClient)
    {
        //试图关闭套接字。如果有挂起的数据等待写入，QAbstractSocket将进入ClosingState并等待，直到所有数据都已写入
        m_tcpClient->disconnectFromHost();
        delete  m_tcpClient;
        m_tcpClient = nullptr;
    }
}

void networkInterface::getPermission()
{
    #ifdef ANDROID
    QAndroidJniObject::callStaticMethod<void>
                (
                "an/qt/qtWithJava/MyJavaClass",
                "getPermission",
                "()V"
                );
#endif
}

void networkInterface::connectBikeServer()
{
    m_tcpClient = UserLogin::getInstance()->getTcpClient();
    m_userName = UserLogin::getInstance()->getUserName();

    qDebug() << "m_tcpClient: %p" << m_tcpClient;
    if(m_tcpClient)
    {
        connect(m_tcpClient, &QTcpSocket::readyRead, this, &networkInterface::ReadData);
        connect(m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ReadError(QAbstractSocket::SocketError)));
        emit ServerConnectSuccessSig();
    }
    else
    {
        connectServer();
    }
}

void networkInterface::connectBikeServer_test()
{
    connectServer();
}

void networkInterface::quiteBikeClient()
{
    UserLogin::getInstance()->openQmlLoginPage();
}

void networkInterface::sendRequestToGetAllBikeMsg()
{
    if(!m_sendMessage) m_sendMessage = new char[1024];
    tutorial::list_all_bikeinfo_request labr;

    std::string userName = m_userName.toStdString();
    labr.set_username(userName.c_str());
    m_sendMsgLen = labr.ByteSizeLong();

    memcpy(m_sendMessage, "FBEB", 4);
    *(u16*)(m_sendMessage + 4) = EEVENTID_LIST_BIKEINFO_REQ;
    *(i32*)(m_sendMessage + 6) = m_sendMsgLen;

    labr.SerializeToArray(m_sendMessage+10, m_sendMsgLen);

    if(m_tcpClient->write(m_sendMessage, m_sendMsgLen+10)==-1)
    {
        emit msgTipPrintToHtmlSig("单车信息同步请求失败！");
    }

    m_tcpClient->flush();
    m_sendMsgLen = 0;
    delete[] m_sendMessage;
    m_sendMessage = nullptr;
}

void networkInterface::addBikeSlot(int bikeId, double longitude, double latitude)
{
    scanQRRequest(EEVENTID_ADDBIKE_REQ, bikeId, longitude, latitude);
}

void networkInterface::deleteBikeSlot(int bikeId)
{
    scanQRRequest(EEVENTID_DELETEBIKE_REQ, bikeId, 0, 0);
}

void networkInterface::openScanCodeWindow(int bikeId, double longitude, double latitude)
{
    m_bikeId = bikeId;
    m_longitude = longitude;
    m_latitude = latitude;
    startScanQRCode("MediaStore.ACTION_IMAGE_CAPTURE");     //调用系统相机
}

void networkInterface::sendBroadcastRequest(int bikeId, int status, double longitude, double latitude)
{
    tutorial::broadcast_msg_request bmr;

    if(!m_sendMessage) m_sendMessage = new char[1024];
    bmr.set_bikeid(bikeId);
    bmr.set_bikestatus(status);
    bmr.set_longitude(longitude);
    bmr.set_latitude(latitude);

    int len = bmr.ByteSizeLong();
    memcpy(m_sendMessage, "FBEB", 4);

    *(u16*)(m_sendMessage + 4) = EEVENTID_BROADCAST_MSG_REQ;
    *(i32*)(m_sendMessage + 6) = len;
    bmr.SerializeToArray(m_sendMessage + 10, len);

    if(m_tcpClient->write(m_sendMessage, len + 10) != -1)
    {
        qDebug() << "sendBroadcastRequest success!";
    }
    m_tcpClient->flush();   //该函数尽可能多地从内部写缓冲区写入底层网络套接字，而不会阻塞
    delete [] m_sendMessage;
    m_sendMessage = nullptr;
}
