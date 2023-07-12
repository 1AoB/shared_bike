#include "websocketclientwrapper.h"

/*
 * 简短地将连接的QWebSockets客户端包装在WebSocketTransport对象中。
   将传入的WebSocket连接到WebChannel只需此代码。
   因此，任何支持WebSocket的远程JavaScript客户端都可以接收消息并访问发布的对象。
*/

/*
    所有连接到QWebSocketServer的客户端都将自动包装在WebSocketTransport对象中。
*/
WebSocketClientWrapper::WebSocketClientWrapper(QWebSocketServer *server, QObject *parent) :
    QObject(parent), m_server(server)
{
    connect(server, &QWebSocketServer::newConnection, this, &WebSocketClientWrapper::handleNewConnection);
}

//在WebSocketTransport对象中包装传入的WebSocket连接
void WebSocketClientWrapper::handleNewConnection()
{
    //返回下一个挂起的连接作为已连接的QWebSocket对象
    emit clientConnected(new WebSocketTransport(m_server->nextPendingConnection()));
}
