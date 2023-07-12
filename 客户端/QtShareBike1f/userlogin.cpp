#include "userlogin.h"
#include "bike.pb.h"
#include <QAndroidJniObject>      //提供从c++调用Java代码的api

QTcpSocket* UserLogin::m_tcpClient = nullptr;
UserLogin* UserLogin::m_instance = nullptr;
QObject* UserLogin::m_qmlObject = nullptr;
QString UserLogin::m_userName = "";
QString UserLogin::m_userPwd = "";
int UserLogin::m_verCode = 0;

UserLogin::UserLogin(QObject *parent) : QObject(parent)
{
    //连接信号与槽
    connect(this, &UserLogin::getVerificationCode_sig, this, &UserLogin::getVerificationCode_slot);    //获取验证码
    connect(this, &UserLogin::registerUser_sig, this, &UserLogin::registerUser_slot);                  //注册
    connect(this, &UserLogin::loginToBikeClient_sig, this, &UserLogin::loginToBikeClient_slot);        //登录
    connect(this, &UserLogin::connectToServer_sig, this, &UserLogin::connectToServer_slot);            //连接服务器
    connect(this, &UserLogin::reconnectToServer_sig, this, &UserLogin::reconnectToServer_slot);        //重连服务器
}

UserLogin::~UserLogin()
{

}

void UserLogin::connectToServer()
{
    //初始化Tcp套接字
    if(m_tcpClient) return;
    if(!m_tcpClient) m_tcpClient = new QTcpSocket;   //实例化
    m_tcpClient->abort();   //终止当前连接并重置套接字, 这个函数会立即关闭套接字，丢弃写缓冲区中任何挂起的数据
    m_tcpClient->connectToHost(shareBikeServer_IP, shareBikeServer_port);

    if(m_tcpClient->waitForConnected(2000))  //等待直到套接字被连接，最长可达2000毫秒
    {
        connect(m_tcpClient, &QTcpSocket::readyRead, this, &UserLogin::ReadData);
        connect(m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ReadError(QAbstractSocket::SocketError)));
    }
    else
    {
        openQmlMsgWindow("无法连接到服务器！");
        refreshLoginWindow();
        delete  m_tcpClient;
        m_tcpClient = nullptr;
    }
}

void UserLogin::sendMsgToServer(EventID_t eventID)
{
    connectToServer();
    if(!m_tcpClient)
    {
        openQmlMsgWindow("请求失败，无法连接服务器！");
        return;
    }

    std::string str_userName = m_userName.toStdString();
    const char* m_userName_char = str_userName.c_str();

    std::string str_userPwd = m_userPwd.toStdString();

    char msg[1024];

    switch (eventID)
    {
    //获取手机验证码请求
    case EEVENTID_GET_MOBILE_CODE_REQ:
    {
        tutorial::mobile_request mr;
        mr.set_username(str_userName);

        int len = mr.ByteSizeLong();
        memcpy(msg, "FBEB", 4);          //数据头

        *(u16*)(msg + 4) = EEVENTID_GET_MOBILE_CODE_REQ;
        *(i32*)(msg + 6) = len;

        qDebug() << "*(u16*)(msg + 4) =" << *(u16*)(msg + 4);//u16 表示无符号的 16 位整数，可以存放 2 个字节的数据。
        qDebug() << "*(i32*)(msg + 6) =" << *(i32*)(msg + 6);//i32表示带符号的32位整数。由于一个字节是8位，所以i32可以存放4个字节的数据。
        mr.SerializeToArray(msg + 10, len);    //数据序列化到数组

        if(m_tcpClient->write(msg, len + 10) != -1)
        {
            qDebug() << "获取验证码请求已发送";
        }
        m_tcpClient->flush();     //QAbstractSocket立即开始发送缓冲数据
        break;
    }

    //注册请求
    case EEVENTID_REGISTER_USER_REQ:
    {
        tutorial::registerUser_request rr;
        rr.set_username(str_userName);
        rr.set_userpwd(str_userPwd);
        rr.set_vercode(m_verCode);
        int len = rr.ByteSizeLong();
        memcpy(msg, "FBEB", 4);

        *(u16*)(msg + 4) = EEVENTID_REGISTER_USER_REQ;
        *(i32*)(msg + 6) = len;
        rr.SerializeToArray(msg + 10, len);

        if(m_tcpClient->write(msg, len + 10) != -1)
        {
            qDebug() << "注册请求已发送";
        }
        m_tcpClient->flush();
        break;
    }

    //登录请求
    case EEVENTID_LOGIN_REQ:
    {
        tutorial::login_request lr;
        lr.set_username(m_userName_char);
        lr.set_userpwd(str_userPwd);
        int len = lr.ByteSizeLong();
        memcpy(msg, "FBEB", 4);

        *(u16*)(msg + 4) = EEVENTID_LOGIN_REQ;
        *(i32*)(msg + 6) = len;
        lr.SerializeToArray(msg + 10, len);

        if(m_tcpClient->write(msg, len + 10) != -1)
        {
            qDebug() << "登录请求已发送";
        }
        m_tcpClient->flush();
        break;
    }
    }
}

void UserLogin::openQmlMsgWindow(QString msg)
{
    if(m_qmlObject)
    {
        QVariant returnedValue;
        QVariant message = msg;
        //调用对象上的成员(信号或槽名)。如果可以调用成员，则返回true。如果没有这样的成员或参数不匹配，则返回false
        //返回值和参数必须用Q_RETURN_ARG()，Q_ARG()宏封装
        QMetaObject::invokeMethod(m_qmlObject, "openMsgWindow",
                                  Q_RETURN_ARG(QVariant, returnedValue),
                                  Q_ARG(QVariant, message));
    }
}

void UserLogin::refreshLoginWindow()
{
    if(m_qmlObject)
    {
        QMetaObject::invokeMethod(m_qmlObject, "refreshLoginPage");
    }
}

void UserLogin::openPhoneNotifyWindow(QString notifyMsg)
{
    #ifdef ANDROID
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(notifyMsg);
    QAndroidJniObject::callStaticMethod<void>   //使用模板  //<>中的表示返回值
            (
                "an/qt/qtWithJava/MyJavaClass",        //表示调用JAVA中类的路径
                "notify",                              //表示方法名
         /*
          V表示void，签名写法
          基本类型就这样表示，如果是JAVA中的类，则表示规则要写成Ljava/lang/String，L做为前缀，后面写上类的包，用分号隔开
          (Ljava/lang/String;)V 括号内部表示参数，括号后面就是返回值，表示参数是Ljava/lang/String，返回值是void
         */
                "(Ljava/lang/String;)V",
                javaNotification.object<jstring>()  //返回由QAndroidJniObject持有的jstring类型的对象
            );
    #endif
}

void UserLogin::distroyFreeMemory()
{
    //不能释放掉m_tcpClient，因为传递给了networkInterface的m_tcpClient
    if(m_instance)  delete m_instance;
    if(m_qmlObject) delete m_qmlObject;
}

//获取验证码
void UserLogin::getVerificationCode_slot(const QString &userName)
{
    m_userName = userName;
    sendMsgToServer(EEVENTID_GET_MOBILE_CODE_REQ);
}

void UserLogin::registerUser_slot(const QString &userName, const QString &password, const QString &verCode)
{
    m_userName = userName;
    m_userPwd = password;
    m_verCode = verCode.toInt();
    sendMsgToServer(EEVENTID_REGISTER_USER_REQ);
}

void UserLogin::loginToBikeClient_slot(const QString &userName, const QString &password)
{
    m_userName = userName;
    m_userPwd = password;
    sendMsgToServer(EEVENTID_LOGIN_REQ);
}

void UserLogin::connectToServer_slot()
{
    connectToServer();
}

void UserLogin::reconnectToServer_slot()
{
    connectToServer();
}

void UserLogin::ReadData()
{
    char readMessage[1024];
    int readLen = m_tcpClient->read(readMessage, 1024);
    qDebug() << "readLen: " <<readLen;
    readMessage[readLen] = '\0';

    if(readLen != -1)
    {
        if(strncmp(readMessage, "FBEB", 4) == 0)
        {
            u16 code = *(u16*)(readMessage + 4);   //EventID
            i32 len = *(i32*)(readMessage + 6);    //数据长度
            qDebug() << "ReadData()--code[EventID]" << code;


            if(code == EEVENTID_GET_MOBILE_CODE_RSP)   //手机验证码响应
            {
                tutorial::mobile_response mr;
                mr.ParseFromArray(readMessage + 10, len);
                m_verCode = mr.vercode();
                QString data(mr.data().c_str());

                if(mr.rescode() == ERRC_SUCCESS)
                {
                    openPhoneNotifyWindow(QString("您正在注册共享单车，验证码为：%1，请保密哦").arg(m_verCode));
                    openQmlMsgWindow(QString("验证码已发送，请留意信息通知"));
                }
                else
                {
                    openQmlMsgWindow(QString("验证码获取失败，错误码[%1]").arg(mr.rescode()));
                }
            }
            else if(code == EEVENTID_REGISTER_USER_RSP)   //注册响应
            {
                tutorial::registerUser_response rr;
                rr.ParseFromArray(readMessage + 10, len);

                if(rr.rescode() == ERRC_SUCCESS)
                {
                    openQmlMsgWindow("账号注册成功！");
                    openQmlLoginPage();
                }
                else if(rr.rescode() == ERRC_INVALID_MSG)
                {
                    openQmlMsgWindow("此用户名已被注册！");
                }
                else if(rr.rescode() == ERRC_INVALID_DATA)
                {
                    openQmlMsgWindow("验证码错误！");
                }
                else openQmlMsgWindow(QString("注册时出错了！错误码[%1]").arg(rr.rescode()));
            }
            else if(code == EEVENTID_LOGIN_RSP)       //登录响应
            {
                tutorial::login_response lr;
                lr.ParseFromArray(readMessage + 10, len);

                if(lr.rescode() == ERRC_SUCCESS)
                {
                    //断开m_tcpClient的信号与槽函数，否则，m_tcpClient再次连接相同的信号时，绑定的槽函数仍然是之前的槽函数，即无效连接
                    disconnect(m_tcpClient, &QTcpSocket::readyRead, this, &UserLogin::ReadData);
                    disconnect(m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
                               this, SLOT(ReadError(QAbstractSocket::SocketError)));

                    if(lr.userlevel() == 1) openQmlBikeManagePage();     //管理员页面
                    else if(lr.userlevel() == 0) openQmlMainPage();      //显示主页面
                }
                else
                {
                    openQmlMsgWindow("用户名或密码错误！请重新输入！");
                    refreshLoginWindow();
                }
            }
            m_userName = "";
            m_userPwd = "";
            m_verCode = 0;
        }
    }
}

void UserLogin::ReadError(QAbstractSocket::SocketError)
{
    openQmlMsgWindow("与服务器连接断开！");
    if(m_tcpClient)
    {
        //试图关闭套接字。如果有挂起的数据等待写入，QAbstractSocket将进入ClosingState并等待，直到所有数据都已写入
        m_tcpClient->disconnectFromHost();
        delete m_tcpClient;
        m_tcpClient = nullptr;
    }
}

/*                                       静态成员函数                                 */

UserLogin *UserLogin::getInstance()
{
    if(!m_instance)
    {
        m_instance = new UserLogin();
    }
    return m_instance;
}

QObject *UserLogin::getQmlObject()
{
    if(m_qmlObject)
        return m_qmlObject;
    else
        return nullptr;
}

QString UserLogin::getUserName()
{
    return m_userName;
}

void UserLogin::setQmlObject(QObject *qmlObject)
{
    m_qmlObject = qmlObject;
}

void UserLogin::openQmlMainPage()
{
    if(m_qmlObject)
    {
        QMetaObject::invokeMethod(m_qmlObject, "enterToMainPage");
    }
}

void UserLogin::openQmlBikeManagePage()
{
    if(m_qmlObject)
    {
        QMetaObject::invokeMethod(m_qmlObject, "enterToBikeManagePage");
    }
}

void UserLogin::openQmlLoginPage()
{
    if(m_qmlObject)
    {
        QMetaObject::invokeMethod(m_qmlObject, "returnToLoginPage");
    }
}

QTcpSocket *UserLogin::getTcpClient()
{
    qDebug() << "m_tcpClient: %p" << m_tcpClient;
    if(m_tcpClient)
        return m_tcpClient;
    else
        return nullptr;
}
