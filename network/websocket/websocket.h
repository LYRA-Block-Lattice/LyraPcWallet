#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QObject>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>

class websocket : public QObject
{
    Q_OBJECT
public:
    websocket(const QUrl &url, QObject *parent = nullptr);
    ~websocket();
Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
};

#endif // WEBSOCKET_H
