#include "events.h"

#include "configlyra.h"

#include <QDateTime>

#include "crypto/signatures.h"
#include "keyrevealer.h"

bool appClosing = false;
bool networkConnected = false;
QList<QPair<QString, QPair<QString, QString>>> walletNameKeyList;
int walletNameKeyListChanged = 0;
int selectedNameKeyIndex = 0;
int bellsNr = 0;
int messagesNr = 0;
rpc::network_e network = events::network_e::DEFAULT_NETWORK;
rpc::network_e networkRpc = events::network_e::DEFAULT_NETWORK;
QList<QPair<QString, bool>> rpcNodeList[3] = {RPC_TESTNET_IP_LIST, RPC_MAINNET_IP_LIST, RPC_DEV_IP_LIST};
double ballance = 0.0;
QString unreceivedBallance;
double totalSupply = LYRA_MAX_SUPPLY;
double burnedSupply = 0.0;
qint64 creationDateTime = 0;
qint64 lastSyncDateTime = 0;
int nrOfTransactions = 0;
int nrOfTransactionsLastWeek = 0;
qint64 minimumDateTime = 0;
QList<QPair<qint64, double>> myWalletBalanceChart = QList<QPair<qint64, double>>();
int myWalletBalanceChartModifyedCnt = 0;
QList<QPair<qint64, double>> myWalletValueAllChart = QList<QPair<qint64, double>>();
int myWalletValueAllChartModifyedCnt = 0;
QList<QPair<qint64, double>> myWalletValueChart = QList<QPair<qint64, double>>();
int myWalletValueChartModifyedCnt = 0;
bool udsSelected = false;
double teamLockedReserved = 9432576934;
double circulatingSupply = 567423066;
double totalBlockCount = 38254;
QList<QStringList> recentTransactionsList = QList<QStringList>();
int recentTransactionsListModifyedCnt = 0;
QList<QStringList> recentRxTransactionsList = QList<QStringList>();
int recentRxTransactionsListModifyedCnt = 0;
QList<QStringList> recentTxTransactionsList = QList<QStringList>();
int recentTxTransactionsListModifyedCnt = 0;
QList<QStringList> assetsList = QList<QStringList>();
int assetsListModifyedCnt = 0;
QDate minimumDateRange;
QDate maximumDateRange;
int dateRangeModifyedCnt = 0;
QString id;
int idModifyedCnt = 0;
QList<QPair<QString, double>> tokenList;
int tokenListModifyedCnt;
QList<QPair<QString, QString>> lastReceivedTransactionsList = QList<QPair<QString, QString>>();
int lastReceivedTransactionsListModifyedCnt = 0;
QList<QPair<QString, QString>> lastSendedTransactionsList = QList<QPair<QString, QString>>();
int lastSendedTransactionsListModifyedCnt = 0;
QMap<QString, double> pricePair;
bool walletLoaded = false;
QString style = DEFAULT_THEME;
int styleCnt = 0;
QPair<QString, QString> walletUserPassword;
int walletUserPasswordCnt = 0;
double scale = 0;
double scaleLater = 0;
bool showTransitionsWindow = false;
bool walletHistoryChanged = false;
bool updateHistory = false;
double totalLyraSendedLastWeek = 0.0;
double totalLyraReceivedLastWeek = 0.0;
double osWindowScale = 1.0;
int screenNumber = 0;
QString customIp[3];
int customIpChanged = 0;
bool walletUnlocked = true;

bool events::getAppClosing() {
    return appClosing;
}

void events::setAppClosing(bool closing) {
    appClosing = closing;
}

void events::setNetworkConnected(bool connected) {
    networkConnected = connected;
}

bool events::getNetworkConnected() {
    return networkConnected;
}

void events::addAccountNameKeyList(QPair<QString, QString> account) {
    QPair<QString, QPair<QString, QString>>tmp2;
    tmp2.first = account.first;
    tmp2.second.first = account.second;
    tmp2.second.second = signatures::getAccountIdFromPrivateKey(account.second);
    walletNameKeyList.append(tmp2);
    walletNameKeyListChanged++;
}

void events::removeAccountNameKeyList(QString accountName) {
    for (int cnt = 0; cnt < walletNameKeyList.count(); cnt++) {
        if(!walletNameKeyList[cnt].first.compare(accountName)) {
            walletNameKeyList.removeAt(cnt);
            break;
        }
        walletNameKeyListChanged++;
    }
}

void events::replaceAccountNameKeyList(QString oldName, QString newName) {
    for (int cnt = 0; cnt < walletNameKeyList.count(); cnt++) {
        if(!walletNameKeyList[cnt].first.compare(oldName)) {
            walletNameKeyList.replace(cnt, QPair<QString, QPair<QString, QString>>(newName, walletNameKeyList[cnt].second));
            walletNameKeyListChanged++;
            break;
        }
    }
}

void events::setAccountNameKeyList(QList<QPair<QString, QString>> accountList) {
    walletNameKeyList.clear();
    QPair<QString, QString> tmp;
    foreach(tmp, accountList) {
        QPair<QString, QPair<QString, QString>>tmp2;
        tmp2.first = tmp.first;
        tmp2.second.first = tmp.second;
        tmp2.second.second = signatures::getAccountIdFromPrivateKey(tmp.second);
        walletNameKeyList.append(tmp2);
    }
    walletNameKeyListChanged++;
}

QList<QPair<QString, QString>> events::getAccountNameIdList() {
    QPair<QString, QPair<QString, QString>>tmp;
    QList<QPair<QString, QString>>tmp3;
    foreach(tmp, walletNameKeyList) {
        QPair<QString, QString>tmp2;
        tmp2.first = tmp.first;
        tmp2.second = tmp.second.second;
        tmp3.append(tmp2);
    }
    return tmp3;
}

QString events::getAccountId(int idNr) {
    if(idNr < walletNameKeyList.count())
        return walletNameKeyList[idNr].second.second;
    return "";
}

QStringList events::getAccountNameList() {
    QStringList walletList;
    QPair<QString, QPair<QString, QString>>pair;
    foreach(pair, walletNameKeyList)
        walletList.append(pair.first);
    return walletList;
}

QString events::getAccountKey(int keyNr, bool immediate, bool persistent) {
    if(keyNr < walletNameKeyList.count()) {
        keyrevealer keyRevealer(keyNr, immediate, persistent);
        return keyRevealer.getKey();
    }
    return "";
}

QString events::getAccountKeyNoP(int keyNr) {
    if(keyNr < walletNameKeyList.count())
        return walletNameKeyList[keyNr].second.first;
    return "";
}

QString events::getAccountKeyNoP(QString name) {
    QMap<QString, QString> pair;
    QPair<QString, QPair<QString, QString>> _pair;
    foreach(_pair, walletNameKeyList)
        pair.insert(_pair.first, _pair.second.first);
    return pair.value(name);
}

int events::getAccountNameKeyListChanged() {
    return walletNameKeyListChanged;
}

int events::getSelectedNameKeyIndex() {
    if(selectedNameKeyIndex < 0)
        return 0;
    else
        return selectedNameKeyIndex;
}

void events::setSelectedNameKeyIndex(int index) {
    selectedNameKeyIndex = index;
}

void events::setBells(int bells) {
    bellsNr = bells;
}

int events::getBells() {
    return bellsNr;
}

void events::setMessages(int messages) {
    messagesNr = messages;
}

int events::getMessages() {
    return messagesNr;
}

void events::setNetwork(network_e net) {
    customIpChanged++;
    network = net;
}

events::network_e events::getNetwork() {
    return network;
}

void events::setRpcNetwork(network_e net) {
    //customIpChanged++;
    networkRpc = net;
}

events::network_e events::getRpcNetwork() {
    return networkRpc;
}

QList<QPair<QString, bool>> events::getNodeList() {
    return rpcNodeList[network];
}

void events::setNodeList(QList<QPair<QString, bool>> nodes) {
    rpcNodeList[network] = nodes;
}

double events::getBallance() {
    return ballance;
}

void events::setBallance(double bal) {
    ballance = bal;
}

QString events::getUnreceivedBallance() {
    return unreceivedBallance;
}

void events::setUnreceivedBallance(QString bal) {
    unreceivedBallance = bal;
}

double events::getTotalSupply() {
    return totalSupply - burnedSupply;
}

void events::setTotalSupply(double bal) {
    totalSupply = bal;
}

double events::getPercentageOfTotalSupply() {
    return (ballance / (totalSupply - burnedSupply)) * 100;
}

double events::getTeamLockedPercentageOfTotalSupply() {
    return (teamLockedReserved / (totalSupply - burnedSupply)) * 100;
}

double events::getBurnedSupply() {
    return burnedSupply;
}

void events::setBurnedSupply(double bal) {
    burnedSupply = bal;
}

qint64 events::getCreationDateTime() {
    return creationDateTime;
}

void events::setCreationDateTime(qint64 dateTime) {
    creationDateTime = dateTime;
}

qint64 events::getLastSyncDateTime() {
    return lastSyncDateTime;
}

void events::setLastSyncDateTime(qint64 dateTime) {
    lastSyncDateTime = dateTime;
}

int events::getNumberOfTransactions() {
    return nrOfTransactions;
}

void events::setNumberOfTransactions(int trans) {
    nrOfTransactions = trans;
}

int events::getNumberOfTransactionsLastWeek() {
    return nrOfTransactionsLastWeek;
}

void events::setNumberOfTransactionsLastWeek(int trans) {
    nrOfTransactionsLastWeek = trans;
}

qint64 events::getMinimumDateTime() {
    return minimumDateTime;
}

void events::setMinimumDateTime(qint64 dateTime) {
    minimumDateTime = dateTime;
}

QList<QPair<qint64, double>> events::getMyAccountBalanceChart() {
    return myWalletBalanceChart;
}

int events::getMyAccountBalanceChartModifyedCnt() {
    return myWalletBalanceChartModifyedCnt;
}

QList<QPair<qint64, double>> events::getMyAccountValueAllChart() {
    return myWalletValueAllChart;
}

int events::getMyAccountValueAllChartModifyedCnt() {
    return myWalletValueAllChartModifyedCnt;
}

QList<QPair<qint64, double>> events::getMyAccountValueChart() {
    return myWalletValueChart;
}

int events::getMyAccountValueChartModifyedCnt() {
    return myWalletValueChartModifyedCnt;
}

void events::setMyAccountBalanceChart(QList<QPair<qint64, double>> list) {
    myWalletBalanceChart = list;
    myWalletBalanceChartModifyedCnt++;
}

void events::setMyAccountValueAllChart(QList<QPair<qint64, double>> list) {
    myWalletValueAllChart = list;
    myWalletValueAllChartModifyedCnt++;
}

void events::setMyAccountValueChart(QList<QPair<qint64, double>> list) {
    myWalletValueChart = list;
    myWalletValueChartModifyedCnt++;
}

bool events::getBtcUsdSelect() {
    return udsSelected;
}

void events::setBtcUsdSelect(bool sel) {
    udsSelected = sel;
}

double events::getTeamLockedReserved() {
    return teamLockedReserved;
}

double events::getCirculatingSupply() {
    return circulatingSupply;
}

double events::getCirculatingSupplyPercentage() {
    return (circulatingSupply / (totalSupply - burnedSupply)) * 100;
}

qint64 events::getTotalBlockCount() {
    return totalBlockCount;
}

void events::setTeamLockedReserved(double value) {
    teamLockedReserved = value;
}

void events::setCirculatingSupply(double value) {
    circulatingSupply = value;
}

void events::setTotalBlockCount(qint64 value) {
    totalBlockCount = value;
}

QList<QStringList> events::getRecentTransactions() {
    return recentTransactionsList;
}

int events::getRecentTransactionsModifyedCnt() {
    return recentTransactionsListModifyedCnt;
}

void events::setRecentTransactions(QList<QStringList> list) {
    recentTransactionsList = list;
    recentTransactionsListModifyedCnt++;
}

QList<QStringList> events::getRecentRxTransactions() {
    return recentRxTransactionsList;
}

int events::getRecentRxTransactionsModifyedCnt() {
    return recentRxTransactionsListModifyedCnt;
}

void events::setRecentRxTransactions(QList<QStringList> list) {
    recentRxTransactionsList = list;
    recentRxTransactionsListModifyedCnt++;
}

QList<QStringList> events::getRecentTxTransactions() {
    return recentTxTransactionsList;
}

int events::getRecentTxTransactionsModifyedCnt() {
    return recentTxTransactionsListModifyedCnt;
}

void events::setRecentTxTransactions(QList<QStringList> list) {
    recentTxTransactionsList = list;
    recentTxTransactionsListModifyedCnt++;
}


QList<QStringList> events::getAssets() {
    return assetsList;
}

int events::getAssetsModifyedCnt() {
    return assetsListModifyedCnt;
}

void events::setAssets(QList<QStringList> list) {
    assetsList = list;
    assetsListModifyedCnt++;
}

QDate events::getMinimumDateRange() {
    return minimumDateRange;
}

QDate events::getMaximumDateRange() {
    return maximumDateRange;
}

int events::getDateRangeModifyedCnt() {
    return dateRangeModifyedCnt;
}

void events::setDateRange(QDate dateMin, QDate dateMax) {
    minimumDateRange = dateMin;
    maximumDateRange = dateMax;
    dateRangeModifyedCnt++;
}

QString events::getId() {
    return id;
}

int events::getIdModifyedCnt() {
    return idModifyedCnt;
}

void events::setId(QString idStr) {
    id = idStr;
    idModifyedCnt++;
}

QList<QPair<QString, double>> events::getTokenList() {
    return tokenList;
}


int events::getTokenListModifyedCnt() {
    return tokenListModifyedCnt;
}


void events::setTokenList(QList<QPair<QString, double>> list) {
    tokenList = list;
    tokenListModifyedCnt++;
}

void events::setTotalLyraSendedLastWeek(double balance) {
    totalLyraSendedLastWeek = balance;
}

double events::getTotalLyraSendedLastWeek() {
    return totalLyraSendedLastWeek;
}

double events::getTotalLyraPercentageSendedLastWeekOfTotal() {
    return 0;
}

void events::setTotalLyraReceivedLastWeek(double balance) {
    totalLyraReceivedLastWeek = balance;
}

double events::getTotalLyraReceivedLastWeek() {
    return totalLyraReceivedLastWeek;
}

double events::getTotalLyraPercentageReceivedLastWeekOfTotal() {
    return 0;
}

QList<QPair<QString, QString>> events::getLastReceivedTransactionsList() {
    return lastReceivedTransactionsList;
}

int events::getLastReceivedTransactionsListModifyedCnt() {
    return lastReceivedTransactionsListModifyedCnt;
}

QList<QPair<QString, QString>> events::getLastSendedTransactionsList() {
    return lastSendedTransactionsList;
}

int events::getLastSendedTransactionsListModifyedCnt() {
    return lastSendedTransactionsListModifyedCnt;
}

void events::setLastReceivedTransactionsList(QList<QPair<QString, QString>> list) {
    lastReceivedTransactionsList = list;
    lastReceivedTransactionsListModifyedCnt++;
}

void events::setLastSendedTransactionsList(QList<QPair<QString, QString>> list) {
    lastSendedTransactionsList = list;
    lastSendedTransactionsListModifyedCnt++;
}

QMap<QString, double> events::getTokenPricePairs() {
    return pricePair;
}

double events::getTokenPricePair(QString tokenPair) {
    return pricePair.value(tokenPair);
}

void events::setTokenPricePair(QString tokenPair, double price) {
    if(pricePair.contains(tokenPair)) {
        pricePair.remove(tokenPair);
    }
    pricePair.insert(tokenPair, price);
}

bool events::getWalletLoaded() {
    return walletLoaded;
}

void events::setWalletLoaded(bool loaded) {
    walletLoaded = loaded;
}

QString events::getStyle() {
    return style;
}

int events::getStyleChangedCnt() {
    return styleCnt;
}

void events::setStyle(QString st) {
    style = st;
    styleCnt++;
}

QPair<QString, QString> events::getWalletUserPassword() {
    return walletUserPassword;
}

int events::getWalletUserPasswordChangedCnt() {
    return walletUserPasswordCnt;
}

void events::setWalletUserPassword(QPair<QString, QString> usrPass) {
    walletUserPassword = usrPass;
    walletUserPasswordCnt++;
}

void events::setScale(double s) {
    scale = s;
    scaleLater = s;
}

double events::getScale() {
#if USE_WINDOWS_SCALING
    return QString::asprintf("%1.1f", scale * osWindowScale).toDouble();
#else
    return QString::asprintf("%1.1f", scale ).toDouble();
#endif
}

double events::getScaleStore() {
    return scale;
}

void events::setScaleLater(double s) {
    scaleLater = s;
}

double events::getScaleLater() {
    return scaleLater;
}

void events::setOsWindowScale(double s) {
    osWindowScale = s;
}

double events::getOsWindowScale() {
#if USE_WINDOWS_SCALING
    return osWindowScale;
#else
    return 1.0;
#endif
}

bool events::getShowTransitionsWindow() {
    bool tmp = showTransitionsWindow;
    showTransitionsWindow = false;
    return tmp;
}

void events::setShowTransitionsWindow() {
    showTransitionsWindow = true;
}

bool events::getWalletHistoryChanged() {
    bool tmp = walletHistoryChanged;
    walletHistoryChanged = false;
    return tmp;
}

bool events::getWalletHistoryChangedUnmodify() {
    return walletHistoryChanged;
}

void events::setWalletHistoryChanged() {
    walletHistoryChanged = true;
}

bool events::getUpdateHistory() {
    bool tmp = updateHistory;
    updateHistory = false;
    return tmp;
}

void events::setUpdateHistory() {
    updateHistory = true;
}

void events::setScreenNumber(int screen) {
    screenNumber = screen;
}

int events::getScreenNumber() {
    return screenNumber;
}

void events::setCustomIp(QString ip, events::network_e net) {
    customIpChanged++;
    if(net == events::network_e::NETWORK_NONE) {
        if(network == events::network_e::NETWORK_TESTNET)
            customIp[0] = ip;
        else if(network == events::network_e::NETWORK_MAINNET)
            customIp[1] = ip;
        else if(network == events::network_e::NETWORK_DEV)
            customIp[2] = ip;
    } else {
        if(net == events::network_e::NETWORK_TESTNET)
            customIp[0] = ip;
        else if(net == events::network_e::NETWORK_MAINNET)
            customIp[1] = ip;
        else if(net == events::network_e::NETWORK_DEV)
            customIp[2] = ip;
    }
}

int events::getCustomIpChanged() {
    return customIpChanged;
}

QString events::getCustomIp(events::network_e net) {
    if(net == events::network_e::NETWORK_NONE) {
        if(network == events::network_e::NETWORK_TESTNET)
            return customIp[0];
        else if(network == events::network_e::NETWORK_MAINNET)
            return customIp[1];
        else if(network == events::network_e::NETWORK_DEV)
            return customIp[2];
        else
            return "";
    } else {
        if(net == events::network_e::NETWORK_TESTNET)
            return customIp[0];
        else if(net == events::network_e::NETWORK_MAINNET)
            return customIp[1];
        else if(net == events::network_e::NETWORK_DEV)
            return customIp[2];
        else
            return "";
    }
}

void events::setWalletUnlocked(bool unlocked) {
    walletUnlocked = unlocked;
}

bool events::getWalletUnlocked() {
    return walletUnlocked;
}



