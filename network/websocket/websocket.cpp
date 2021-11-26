#include "websocket.h"

websocket::websocket(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url) {
    connect(&m_webSocket, &QWebSocket::connected, this, &websocket::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &websocket::closed);
    m_webSocket.open(QUrl(url));
}

websocket::~websocket() {

}

//! [onConnected]
void websocket::onConnected()
{
    qDebug() << "WEBSOCKET 1: WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &websocket::onTextMessageReceived);
    m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}
//! [onConnected]

//! [onTextMessageReceived]
void websocket::onTextMessageReceived(QString message)
{
    qDebug() << "WEBSOCKET 2: Message received:" << message;
    m_webSocket.close();
}
//! [onTextMessageReceived]
