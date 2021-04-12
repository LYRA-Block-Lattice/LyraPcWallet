#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QMainWindow>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QTimer>
#include <QEventLoop>


QT_FORWARD_DECLARE_CLASS(QWebSocket)


class sslwebsocket : public QObject {
    Q_OBJECT
public:
    sslwebsocket(const QUrl &url, QMainWindow *parent = nullptr);
    ~sslwebsocket();
    bool sendTextMessage(QString text);
    QString getTextMessage();
    void setTextMessage(QString text);
    bool getConnected();

private:
    QWebSocket m_webSocket;
    bool connected;
    QStringList receivedTextList;
    QEventLoop *eventLoop;
private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);
};

#endif // WEBSOCKET_H
