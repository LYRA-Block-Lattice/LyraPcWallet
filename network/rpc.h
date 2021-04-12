#ifndef RPC_H
#define RPC_H

#include <QMainWindow>
#include <QMap>
#include <QTimer>

#include "websocket/sslwebsocket.h"

#include "config.h"

class rpc : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        NETWORK_TESTNET = 0,
        NETWORK_MAINNET
    }network_e;

    typedef enum {
        RPC_CONNECTION_STATE_DISCONNECT = 0,
        RPC_CONNECTION_STATE_CONNECTING,
        RPC_CONNECTION_STATE_CONNECTED,
    }rpcConnectionState_e;

    explicit rpc(QMainWindow *parent = nullptr);
    ~rpc();

    bool sendText(QString txt);
    QString sendMessage(int *id, QString api, QStringList args);
    QString sendSimpleMessage(QString message);
    QString getResponse();
    bool getConnected();
private:
    void populateNodeList();
    QMainWindow *parent;
    network_e network = network_e::DEFAULT_NETWORK;
    QList<QPair<QString, bool>> rpcNodeList;
    QList<bool> usedRpc;

    rpcConnectionState_e connectionState = rpcConnectionState_e::RPC_CONNECTION_STATE_DISCONNECT;
    int timeoutCount = RPC_CONNECT_TIMEOUT;


    QTimer timerRetryConnect;
    QTimer timerSendMsgTimeout;
    bool sendMsgTimedOut = false;
    int LocalId = 1;

    sslwebsocket *sslWebSocket = nullptr;
signals:

private slots:
    void on_connect();
    void on_sendMsgTimeout();
};

#endif // RPC_H
