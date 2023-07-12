#ifndef WEBSOCKETCLIENTWRAPPER_H
#define WEBSOCKETCLIENTWRAPPER_H

#include <QObject>
#include <QWebSocketServer>
#include "websockettransport.h"

class WebSocketClientWrapper : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClientWrapper(QWebSocketServer *server, QObject *parent = nullptr);

signals:
    void clientConnected(WebSocketTransport* client);

private slots:
    void handleNewConnection();

private:
    QWebSocketServer* m_server;
};

#endif // WEBSOCKETCLIENTWRAPPER_H

