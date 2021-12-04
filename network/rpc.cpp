
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
        if(events::getCustomIp(events::getNetwork()).length() == 0) {
            if(network != events::getNetwork()) {
                network = events::getNetwork();
                populateNodeList();
                qDebug() << "RPC 1: Network changed";
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
                qDebug() << "RPC 2: Repopulate list";
                populateNodeList();
                timerRetryConnect.setInterval(1);
                timerRetryConnect.start();
                return;
            }
            while(!events::getAppClosing()) {
#if VORBOSE_LEVEL >= 5
                qDebug() << "RPC 15 :Loop ";
#endif
                if(events::getCustomIp().length())
                    break;
                quint32 value = QRandomGenerator::global()->generate() % (rpcNodeList.count());
                if(rpcNodeList[value].second) {
                    rpcNodeList[value].second = false;
                    if(sslWebSocket)
                        delete sslWebSocket;
                    sslWebSocket = new sslwebsocket(rpcNodeList[value].first, parent);
                    qDebug() << "RPC 3 :Connecting " << value;
                    qDebug() << "RPC 3 :Connecting " << rpcNodeList[value].first;
                    connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_CONNECTING;
                    break;
                }
            }
            events::setRpcNetwork(events::getNetwork());
        } else {
            if(network != events::getNetwork()) {
                network = events::getNetwork();
                qDebug() << "RPC 4: Network changed";
            }
            timeoutCount = RPC_CONNECT_TIMEOUT;
            if(sslWebSocket)
                delete sslWebSocket;
            QString tmp = "wss://" + events::getCustomIp(events::getNetwork()) + ":" + (events::getNetwork() == events::network_e::NETWORK_TESTNET ? "4504" : "5504") + "/api/v1/socket";
            sslWebSocket = new sslwebsocket(QUrl(tmp), parent);

            qDebug() << "RPC 5: Connecting " << events::getCustomIp(events::getNetwork());
            connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_CONNECTING;
            events::setRpcNetwork(events::getNetwork());
        }
    } else if(connectionState == rpcConnectionState_e::RPC_CONNECTION_STATE_CONNECTING) {
        if(sslWebSocket->getConnected()) {
            connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_CONNECTED;
            timerRetryConnect.setInterval(100);
            timerRetryConnect.start();
            qDebug() << "RPC 6: Connected";
            events::setNetworkConnected(true);
            return;
        } else {
            timeoutCount--;
            if(!timeoutCount) {
                connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_DISCONNECT;
                qDebug() << "RPC 7: Timed out";
                events::setNetworkConnected(false);
            }
        }
    } else if(connectionState == rpcConnectionState_e::RPC_CONNECTION_STATE_CONNECTED) {
        if(!sslWebSocket->getConnected() || network != events::getNetwork() || customIpChanged != events::getCustomIpChanged()) {
            customIpChanged = events::getCustomIpChanged();
            connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_DISCONNECT;
            qDebug() << "RPC 8: Disconnected";
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
    qDebug() << "RPC 9: " + sendMsg;
    if(sendText(sendMsg)) {
        sendMsgTimedOut = false;
        timerRetryConnect.setInterval(RPC_MESSAGE_RESPONSE_TIMEOUT);
        timerRetryConnect.start();
        while(!sendMsgTimedOut && !events::getAppClosing() && rpc::getConnected()) {
#if VORBOSE_LEVEL >= 5
            qDebug() << "RPC 16 :Loop ";
#endif
            if(!timerRetryConnect.isActive())
                timerRetryConnect.start();
            QApplication::processEvents();
            QString response = getResponse();
            if(response.length()) {
                QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
                QJsonObject jsonObject = jsonResponse.object();
                int idRet = jsonObject["id"].toInt();
                if(*id == idRet || jsonObject.contains("method")) {
                    qDebug() << "RPC 10: " + response << Qt::endl;
                    return response;
                } else {
                    sslWebSocket->setTextMessage(response);
                }
            }
        }
    }
    qDebug() << "RPC 11: Response fail" << Qt::endl;
    return QString("");
}

QString rpc::sendSimpleMessage(QString message) {
#if VORBOSE_LEVEL >= 1
    qDebug() << "RPC 12: " + message;
#endif
    if(sendText(message)) {
        sendMsgTimedOut = false;
        timerRetryConnect.setInterval(RPC_MESSAGE_RESPONSE_TIMEOUT);
        timerRetryConnect.start();
        while(!sendMsgTimedOut && !events::getAppClosing() && rpc::getConnected()) {
#if VORBOSE_LEVEL >= 5
            qDebug() << "RPC 17 :Loop ";
#endif
            QApplication::processEvents();
            QString response = getResponse();
            if(response.length()) {
#if VORBOSE_LEVEL >= 1
                qDebug() << "RPC 13: " + response << Qt::endl;
#endif
                return response;
            }
        }
    }
#if VORBOSE_LEVEL >= 1
    qDebug() << "RPC 14: Response fail" << Qt::endl;
#endif
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

