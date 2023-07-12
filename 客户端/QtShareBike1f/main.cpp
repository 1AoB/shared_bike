#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QGuiApplication>
#include <QtWebView>
#include <QWebSocketServer>
#include <QWebChannel>
#include "websocketclientwrapper.h"
#include "networkinterface.h"
#include "userlogin.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}


/*
int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    //此函数用于初始化不同后端所需的资源或设置选项。
    //注意：需要在创建QGuiApplication实例后立即调用Initialize()函数
    QtWebView::initialize();

    //此属性保存此应用程序的用户可见名称
    QGuiApplication::setApplicationDisplayName(QCoreApplication::translate("main", "QtWebView Example"));

    QFileInfo jsFileInfo(":/qwebchannel.js");
    if(!jsFileInfo.exists())
    {
        qDebug() << "qwebchannel.js no exists!";
        return -1;
    }

    //设置QWebSocketServer
    //参数一：服务器名称     参数二：服务器在非安全模式下运行
    QWebSocketServer server(QStringLiteral("QWebChannel Standalone Example Server"), QWebSocketServer::NonSecureMode);
    if(!server.listen(QHostAddress::LocalHost, 12345))    //告诉服务器侦听地址地址和端口上的传入连接
    {
        qFatal("Failed to open web socket server.");
        return 1;
    }

    //在QWebChannel AbstractTransport对象中包装WebSocket服务端
    WebSocketClientWrapper clientWrapper(&server);

    //设置 channel
    QWebChannel channel(nullptr);

    //将QWebChannel连接到给定的传输对象。然后传输对象处理c++应用程序和远程HTML客户机之间的通信
    QObject::connect(&clientWrapper, &WebSocketClientWrapper::clientConnected, &channel, &QWebChannel::connectTo);

    //设置 networkInterface 并将其发布到QWebChannel
    //注册名不可改，bikeManager内使用
    channel.registerObject(QStringLiteral("msg_interface"), networkInterface::getInstance());

    //注册C++类型UserLogin，便于在qml中调用UserLogin的方法
    qmlRegisterType<UserLogin>("UserLogin.module", 1, 0, "UserLogin");

    //QQmlApplicationEngine提供了一种方便的方式来从单个QML文件加载应用程序
    QQmlApplicationEngine engine;

    //QQmlComponent类封装了一个QML组件定义
    //从给定的url创建一个QQmlComponent，并给它指定父组件和引擎
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/main.qml")));

    //从该组件创建一个对象实例。如果创建失败，返回nullptr
    QObject* object = component.create();

    //绑定Qml对象，便于对qml文件操作
    UserLogin::setQmlObject(object);        //UserLogin类自定义方法
    UserLogin::getInstance()->connectToServer();

    return app.exec();
}
*/
