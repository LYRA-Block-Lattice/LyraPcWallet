
#include <QtCore/QDebug>
#include <QRandomGenerator>
#include <QThread>
#include <QCoreApplication>
#include <QThread>
#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "plat.h"

#include "rpc.h"
#include "websocket/sslwebsocket.h"
#include "config.h"
#include "wallet/events.h"

QT_USE_NAMESPACE

rpc::rpc(QMainWindow *parent) : QObject(parent) {
    this->parent = parent;
    populateNodeList();

    timerRetryConnect.setInterval(10);
    connect(&timerRetryConnect, SIGNAL(timeout()), this, SLOT(on_connect()));
    timerRetryConnect.start();

    connect(&timerRetryConnect, SIGNAL(timeout()), this, SLOT(on_connect()));
}

rpc::~rpc() {
    delete sslWebSocket;
}

void rpc::populateNodeList(){
    rpcNodeList = events::getNodeList();
}

void rpc::on_connect() {
    timerRetryConnect.stop();
    if(connectionState == rpcConnectionState_e::RPC_CONNECTION_STATE_DISCONNECT) {
        if(network != events::getNetwork()) {
            network = events::getNetwork();
            populateNodeList();
            qDebug() << "Network changed";
        }
        timeoutCount = RPC_CONNECT_TIMEOUT;
        QPair<QString, bool> node;
        foreach(node, rpcNodeList) {
            if(node.second) {
                break;
            }
        }
        if(!node.second) {
            /* Retry with entire list */
            qDebug() << "Repopulate list";
            populateNodeList();
            timerRetryConnect.setInterval(1);
            timerRetryConnect.start();
            return;
        }
        while(1) {
            quint32 value = QRandomGenerator::global()->generate() % (rpcNodeList.count());
            if(rpcNodeList[value].second) {
                rpcNodeList[value].second = false;
                if(sslWebSocket)
                    delete sslWebSocket;
                sslWebSocket = new sslwebsocket(rpcNodeList[value].first, parent);
                qDebug() << "Connecting " << value;
                connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_CONNECTING;
                break;
            }
        }
    } else if(connectionState == rpcConnectionState_e::RPC_CONNECTION_STATE_CONNECTING) {
        if(sslWebSocket->getConnected()) {
            connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_CONNECTED;
            timerRetryConnect.setInterval(100);
            timerRetryConnect.start();
            qDebug() << "Connected";
            events::setNetworkConnected(true);
            return;
        } else {
            timeoutCount--;
            if(!timeoutCount) {
                connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_DISCONNECT;
                qDebug() << "Timed out";
                events::setNetworkConnected(false);
            }
        }
    } else if(connectionState == rpcConnectionState_e::RPC_CONNECTION_STATE_CONNECTED) {
        if(!sslWebSocket->getConnected() || network != events::getNetwork()) {
            connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_DISCONNECT;
            qDebug() << "Disconnected";
            events::setNetworkConnected(false);
        }
    }
    timerRetryConnect.setInterval(10);
    timerRetryConnect.start();
}

void rpc::on_sendMsgTimeout() {
    timerRetryConnect.stop();
    sendMsgTimedOut = true;
}

bool rpc::sendText(QString txt) {
    if(sslWebSocket->getConnected()) {
        sslWebSocket->sendTextMessage(txt);
        return true;
    } else {
        return false;
    }
}

QString rpc::sendMessage(int *id, QString api, QStringList args) {
    QJsonArray array;
    foreach(QString value, args) {
        array.append(value);
    }
    //array.append("2.2");
    //array.append("testnet");

    *id = LocalId++;
    QJsonObject object;
    object.insert("jsonrpc", "2.0");
    object.insert("method", api);
    object.insert("id", *id);
    object.insert("params", array);

    QJsonDocument doc(object);
    QString sendMsg = QString((doc.toJson(QJsonDocument::Compact)));
    qDebug() << sendMsg;
    if(sendText(sendMsg)) {
        sendMsgTimedOut = false;
        timerRetryConnect.setInterval(RPC_MESSAGE_RESPONSE_TIMEOUT);
        timerRetryConnect.start();
        while(!sendMsgTimedOut) {
            if(!timerRetryConnect.isActive())
                timerRetryConnect.start();
            QApplication::processEvents();
            QString response = getResponse();
            if(response.length()) {
                QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
                QJsonObject jsonObject = jsonResponse.object();
                int idRet = jsonObject["id"].toInt();
                if(*id == idRet || jsonObject.contains("method")) {
                    qDebug() << response << Qt::endl;
                    return response;
                } else {
                    sslWebSocket->setTextMessage(response);
                }
            }
            if(sendMsgTimedOut)
                break;
        }
    }
    qDebug() << "Response fail" << Qt::endl;
    return QString("");
}

QString rpc::sendSimpleMessage(QString message) {
    qDebug() << message;
    if(sendText(message)) {
        sendMsgTimedOut = false;
        timerRetryConnect.setInterval(RPC_MESSAGE_RESPONSE_TIMEOUT);
        timerRetryConnect.start();
        while(1) {
            QApplication::processEvents();
            QString response = getResponse();
            if(response.length()) {
                qDebug() << response << Qt::endl;
                return response;
            }
            if(sendMsgTimedOut)
                break;
        }
    }
    qDebug() << "Response fail" << Qt::endl;
    return QString("");
}

QString rpc::getResponse() {
    if(sslWebSocket->getConnected()) {
        return sslWebSocket->getTextMessage();
    }
    return QString("");
}

bool rpc::getConnected() {
    if(sslWebSocket)
        return sslWebSocket->getConnected();
    else
        return false;
}

