#ifndef TICKEDUPDATES_H
#define TICKEDUPDATES_H

#include <QWidget>
#include <QTimer>

#include "network/rpc.h"


class tickedupdates : public QWidget
{
    Q_OBJECT
public:
    tickedupdates();
    ~tickedupdates();
    void init(QWidget *parent = nullptr, tickedupdates *instance = nullptr);
    void run();
private:
    QTimer fetchPairPrice;
    QTimer fetchNode;
    QTimer saveWallet;

    int selectedNameKeyIndex = -1;
    int walletNameKeyListChanged = -1;
    int count = 0;
    rpc::network_e network = rpc::network_e::NETWORK_TESTNET;
private slots:
    void on_FetchPairPrice();
    void on_FetchNode();
    void on_SaveWallet();
};

#endif // TICKEDUPDATES_H
