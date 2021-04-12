#include "sslwebsocket.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QApplication>
#include <QCoreApplication>
#include <QThread>

#include <QSslSocket>

#include "config.h"

QT_USE_NAMESPACE

sslwebsocket::sslwebsocket(const QUrl &url, QMainWindow *parent) : QObject(parent) {
    //qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
    //qDebug() << "Needed OpenSSL version: " << QSslSocket::sslLibraryBuildVersionString();
    connect(&m_webSocket, &QWebSocket::connected, this, &sslwebsocket::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &sslwebsocket::onDisconnected);
    connect(&m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &sslwebsocket::onSslErrors);
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &sslwebsocket::onTextMessageReceived);
    //m_webSocket.setParent(parent);
    m_webSocket.open(QUrl(url));
    connected = false;
}

sslwebsocket::~sslwebsocket() {
    m_webSocket.disconnect();
    m_webSocket.close();
}

bool sslwebsocket::sendTextMessage(QString text) {
    //receivedTextList.clear();
    m_webSocket.sendTextMessage(text);
    return true;
}

QString sslwebsocket::getTextMessage() {
    QString message = "";
    if(receivedTextList.count()) {
        message = receivedTextList.at(0);
        receivedTextList.removeAt(0);
    }
    return message;
}

void sslwebsocket::setTextMessage(QString text) {
    receivedTextList.append(text);
}

bool sslwebsocket::getConnected() {
    return connected;
}

void sslwebsocket::onConnected() {
    connected = true;
}

void sslwebsocket::onDisconnected() {
    connected = false;
    qDebug() << m_webSocket.errorString();
}

void sslwebsocket::onTextMessageReceived(QString message) {
    receivedTextList.append(message);
}

void sslwebsocket::onSslErrors(const QList<QSslError> &errors) {
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    m_webSocket.ignoreSslErrors();
}

