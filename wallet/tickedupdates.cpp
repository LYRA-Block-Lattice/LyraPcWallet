
#include <QList>
#include <QPair>
#include <QVariant>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "tickedupdates.h"
#include "network/tokenpairing.h"
#include "wallet/events.h"
#include "wallet/rpc/wallethistory.h"
#include "wallet/rpc/walletbalance.h"
#include "pages/pc/populate.h"
#include "crypto/signatures.h"
#include "storage/walletfile.h"
#include "language/translate.h"

#include "config.h"

tickedupdates *tickedUpdatesl = nullptr;

tickedupdates::tickedupdates() {
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
}

tickedupdates::~tickedupdates() {

}

void tickedupdates::init(QWidget *parent, tickedupdates *instance) {
    this->setParent(parent);
    tickedUpdatesl = instance;
    fetchPairPrice.setParent(this);
    fetchPairPrice.setInterval(2000);
    fetchPairPrice.start();
    fetchNode.setParent(this);
    fetchNode.setInterval(5000);
    fetchNode.start();
    saveWallet.setParent(this);
    saveWallet.setInterval(300000);
    saveWallet.start();

    connect(&fetchPairPrice, SIGNAL(timeout()), this, SLOT(on_FetchPairPrice()));
    connect(&fetchNode, SIGNAL(timeout()), this, SLOT(on_FetchNode()));
    connect(&saveWallet, SIGNAL(timeout()), this, SLOT(on_SaveWallet()));

}

void tickedupdates::run() {
    if(selectedNameKeyIndex != events::getSelectedNameKeyIndex() || walletNameKeyListChanged != events::getWalletNameKeyListChanged() || network != events::getNetwork()) {
        network = events::getNetwork();
        selectedNameKeyIndex = events::getSelectedNameKeyIndex();
        walletNameKeyListChanged = events::getWalletNameKeyListChanged();
        fetchNode.setInterval(2000);
        fetchNode.start();
    }
}

void tickedupdates::on_FetchPairPrice() {
    fetchPairPrice.setInterval(300000);
    QList<double>usd;
    QList<double>btc;
    /* Fetch USD on latoken.com */
    QString resp = tokenpairing::getStatic("https://api.latoken.com/v2/ticker");
    QJsonDocument jsonResponse = QJsonDocument::fromJson(resp.toUtf8());
    QJsonArray jsonArray = jsonResponse.array();
    QJsonObject obj;
    int cnt = 0;
    for(; cnt < jsonArray.count(); cnt++) {
        obj = jsonArray[cnt].toObject();
        if(obj["symbol"].toString().contains("LYR/USDT")) {
            usd.append(obj["lastPrice"].toString().toDouble());
        }
    }
    /* In case of more exchanges are pool'd */
    double avarage = usd.count() ? usd[0] : 0.0;
    for(cnt = 1; cnt < usd.count(); cnt++) {
        avarage += usd[cnt];
    }
    if(usd.count() > 1) {
        avarage /= usd.count();
    }
    if(avarage != 0.0) {
        events::setTokenPricePair("LYR_USD", avarage);
    }
    /* Fetch BTC on finexbox */
    resp = tokenpairing::getStatic("https://xapi.finexbox.com/v1/ticker?market=lyr_btc");
    jsonResponse = QJsonDocument::fromJson(resp.toUtf8());
    obj = jsonResponse.object();
    QJsonObject o = obj["result"].toObject();
    if(o.count()) {
        btc.append(o["price"].toString().toDouble());
    }
    /* In case of more exchanges are pool'd */
    avarage = btc.count() ? btc[0] : 0.0;
    for(cnt = 1; cnt < usd.count(); cnt++) {
        avarage += btc[cnt];
    }
    if(btc.count() > 1) {
        avarage /= btc.count();
    }
    if(avarage != 0.0) {
        events::setTokenPricePair("LYR_BTC", avarage);
    }
    populate::refreshAll();
}

void tickedupdates::on_FetchNode() {
    fetchNode.setInterval(60000);
    int index = events::getSelectedNameKeyIndex();
    QList<QPair<QString, QString>> pair = events::getWalletNameKeyList();
    if(index < 0) {
        if(pair.count() != 0) {
            index = 0;
        } else {
            return;
        }
    }
    if(pair.count()) {
        bool unreceived = false;
        int height = 0;
        walletbalance::balance(signatures::getAccountIdFromPrivateKey(pair[index].second), &height, &unreceived);
        events::setUnreceivedBallance(unreceived ? _tr("Yes") : _tr("No"));
        if(height != wallethistory::getCount(pair[index].first)) {
            wallethistory::updateWallet(pair[index].first, signatures::getAccountIdFromPrivateKey(pair[index].second));
            populate::refreshAll();
            events::setWalletHistoryChanged();
            events::setUpdateHistory();
        }
    }
    populate::refreshAll();
}

void tickedupdates::on_SaveWallet() {
    walletfile::save(events::getWalletUserPassword().first, events::getWalletUserPassword().second);
}

