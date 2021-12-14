
#include <QList>
#include <QPair>
#include <QVariant>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QApplication>

#include "tickedupdates.h"
#include "network/tokenpairing.h"
#include "wallet/events.h"
#include "wallet/rpc/wallethistory.h"
#include "wallet/rpc/walletbalance.h"
#include "pages/pc/populate.h"
#include "crypto/signatures.h"
#include "storage/walletfile.h"
#include "language/translate.h"
#include "api/nebula.h"

#include "config.h"

QTimer *fetchNode;
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
    fetchNode = new QTimer(this);
    fetchNode->setInterval(5000);
    fetchNode->start();
    fetchNodeUnreceived.setParent(this);
    fetchNodeUnreceived.setInterval(5000);
    fetchNodeUnreceived.start();
    saveWallet.setParent(this);
    saveWallet.setInterval(300000);
    saveWallet.start();

    connect(&fetchPairPrice, SIGNAL(timeout()), this, SLOT(on_FetchPairPrice()));
    connect(fetchNode, SIGNAL(timeout()), this, SLOT(on_FetchNode()));
    connect(&fetchNodeUnreceived, SIGNAL(timeout()), this, SLOT(on_FetchNodeUnreceived()));
    connect(&saveWallet, SIGNAL(timeout()), this, SLOT(on_SaveWallet()));
}

void tickedupdates::triggerAccountRefresh() {
    fetchNode->setInterval(1);
    fetchNode->start();
}

void tickedupdates::run() {
    if(selectedNameKeyIndex != events::getSelectedNameKeyIndex() ||
            walletNameKeyListChanged != events::getAccountNameKeyListChanged() ||
            network != events::getNetwork()) {
        network = events::getNetwork();
        selectedNameKeyIndex = events::getSelectedNameKeyIndex();
        walletNameKeyListChanged = events::getAccountNameKeyListChanged();
        fetchNode->setInterval(2000);
        fetchNode->start();
    }
    if(events::getTriggerNodeFetch()) {
        fetchNodeUnreceived.setInterval(2000);
        fetchNodeUnreceived.start();
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
    fetchNode->setInterval(60000);

#if VORBOSE_LEVEL >= 3
    qDebug() << "TICKEDUPDATES 1: Start fetch nebula for network data.";
#endif
    /* Fetch data from nebula */
    double teamTotal = 0.0;
    double circulate = 0.0;
    double burned = 0.0;
    qint64 height = 0;
    if(nebula::getCirculation(&teamTotal, &circulate, &burned)) {
        events::setBurnedSupply(burned);
        events::setTeamLockedReserved(teamTotal);
        events::setCirculatingSupply(circulate);
#if VORBOSE_LEVEL >= 3
        qDebug() << "TICKEDUPDATES 2: Fetch";
#endif
    } else {
#if VORBOSE_LEVEL >= 3
        qDebug() << "TICKEDUPDATES 3: Failed";
#endif
    }
#if VORBOSE_LEVEL >= 3
    qDebug() << "TICKEDUPDATES 4: End fetch nebula for network data.";
    qDebug() << "TICKEDUPDATES 5: Start fetch node for data.";
#endif
    if(nebula::getBlockHeight(&height)) {
        events::setTotalBlockCount(height);
#if VORBOSE_LEVEL >= 3
        qDebug() << "TICKEDUPDATES 6: Fetch";
#endif
    } else {
        events::setTotalBlockCount(0);
#if VORBOSE_LEVEL >= 3
        qDebug() << "TICKEDUPDATES 7: Failed";
#endif
    }

    // Check if current account have unreceived balance.
    int index = events::getSelectedNameKeyIndex();
    QString accId = events::getAccountId(index);
    /*if(!accId.length())
        return;*/
    QStringList names = events::getAccountNameList();
    if(index < 0) {
        if(names.count() != 0) {
            index = 0;
        } else {
            return;
        }
    }
    if(names.count()) {
        bool unreceived = false;
        int height = 0;
        walletbalance::balance(accId, &height, &unreceived);
        events::setUnreceivedBallance(unreceived ? "Yes" : "No");
        if(height != wallethistory::getCount(names[index])) {
            wallethistory::updateAccount(names[index], accId);
            //populate::refreshAll(index);
            events::setWalletHistoryChanged();
            events::setUpdateHistory();
        }
    }

    populate::refreshAll();
#if VORBOSE_LEVEL >= 3
    qDebug() << "TICKEDUPDATES 8: End fetch node for data.";
#endif
}

void tickedupdates::on_FetchNodeUnreceived() {
    fetchNodeUnreceived.setInterval(60000);
#if VORBOSE_LEVEL >= 3
    qDebug() << "TICKEDUPDATES 9: Start fetch node for unreceived balance.";
#endif
    // Check if there is unreceived balance in each account.
    QList<QPair<QString, QString>> nameAccIdList = events::getAccountNameIdList();
    QPair<QString, QString>nameAccId;
    QList<QPair<QString, int>> tmpListNewRegistered;
    foreach(nameAccId, nameAccIdList) {
        QApplication::processEvents();
        bool unreceived = false;
        int height = 0;
        walletbalance::balance(nameAccId.second, &height, &unreceived);
        if(unreceived) {
            tmpListNewRegistered.append(QPair<QString, int>(nameAccId.first + ":" + nameAccId.second, events::ringEvent_e::RING_EVENT_UNRECEIVED_BALANCE));
            //events::addRingEvent(nameAccId.first + ":" + nameAccId.second, events::ringEvent_e::RING_EVENT_UNRECEIVED_BALANCE);
        }
    }
    QList<QPair<QString, int>> tmpListAlreadyRegistered(events::getRingEventList());
    QPair<QString, int>tmpItem;
    // Remove the events that no longer active.
    foreach(tmpItem, events::getRingEventList()) {
        if(!tmpListNewRegistered.contains(tmpItem)) {
            tmpListAlreadyRegistered.removeAll(tmpItem);
        }
    }
    // Insert the new events into position 0, most recent first.
    foreach(tmpItem, tmpListNewRegistered) {
        if(!tmpListAlreadyRegistered.contains(tmpItem)) {
            tmpListAlreadyRegistered.insert(0, tmpItem);
        }
    }
    events::clearRingEvents();
    events::addRingEvents(tmpListAlreadyRegistered);
    events::setBells(events::getRingEventCount());
#if VORBOSE_LEVEL >= 3
    qDebug() << "TICKEDUPDATES 10: End fetch node for unreceived balance.";
#endif
}

void tickedupdates::on_SaveWallet()  {
    walletfile::save(events::getWalletUserPassword().first, events::getWalletUserPassword().second);
}

