#ifndef WEBSOCKETTRANSPORT_H
#define WEBSOCKETTRANSPORT_H

#include <QWebChannelAbstractTransport>  //c++ QWebChannel服务器和HTML/JS客户端之间的通信通道
#include <QWebSocket>

class WebSocketTransport : public QWebChannelAbstractTransport
{
    Q_OBJECT
public:
    explicit WebSocketTransport(QWebSocket* socket);
    virtual ~WebSocketTransport();

    //向远程客户端发送JSON消息。实现序列化消息并将其传输到远程JavaScript客户机
    void sendMessage(const QJsonObject &message) override;

private slots:
    void textMessageReceived(const QString& messageData);

private:
    QWebSocket* m_socket;
};

#endif // WEBSOCKETTRANSPORT_H
