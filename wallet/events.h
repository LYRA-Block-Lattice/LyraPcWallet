#ifndef EVENTS_H
#define EVENTS_H

#include <QStringList>
#include <QPair>
#include <QList>

#include "network/rpc.h"


class events {
public:
    typedef rpc::network_e network_e;
    static bool getNetworkConnected();
    static QList<QPair<QString, QString>>  getWalletNameKeyList();
    static QStringList getWalletNameList();
    static QStringList getWalletKeyList();
    static QString getWalletKey(QString name);
    static int getWalletNameKeyListChanged();
    static int getSelectedNameKeyIndex();
    static int getBells();
    static int getMessages();
    static rpc::network_e getNetwork();
    static QList<QPair<QString, bool>> getNodeList();
    static double getBallance();
    static QString getUnreceivedBallance();
    static double getTotalSupply();
    static double getPercentageOfTotalSupply();
    static double getTeamLockedPercentageOfTotalSupply();
    static qint64 getCreationDateTime();
    static qint64 getLastSyncDateTime();
    static int getNumberOfTransactions();
    static int getNumberOfTransactionsLastWeek();
    static qint64 getMinimumDateTime();
    static QList<QPair<qint64, double>> getMyWalletBalanceChart();
    static int getMyWalletBalanceChartModifyedCnt();
    static QList<QPair<qint64, double>>getMyWalletValueAllChart();
    static int getMyWalletValueAllChartModifyedCnt();
    static QList<QPair<qint64, double>> getMyWalletValueChart();
    static int getMyWalletValueChartModifyedCnt();
    static bool getBtcUsdSelect();
    static double getTeamLockedReserved();
    static double getCirculatingSupply();
    static double getCirculatingSupplyPercentage();
    static qint64 getTotalBlockCount();
    static QList<QStringList> getRecentTransactions();
    static int getRecentTransactionsModifyedCnt();
    static QList<QStringList> getRecentRxTransactions();
    static int getRecentRxTransactionsModifyedCnt();
    static QList<QStringList> getRecentTxTransactions();
    static int getRecentTxTransactionsModifyedCnt();
    static QList<QStringList> getAssets();
    static int getAssetsModifyedCnt();
    static QDate getMinimumDateRange();
    static QDate getMaximumDateRange();
    static int getDateRangeModifyedCnt();
    static QString getId();
    static int getIdModifyedCnt();
    static QList<QPair<QString, double>> getTokenList();
    static int getTokenListModifyedCnt();
    static double getTotalLyraSendedLastWeek();
    static double getTotalLyraReceivedLastWeek();
    static double getTotalLyraPercentageSendedLastWeekOfTotal();
    static double getTotalLyraPercentageReceivedLastWeekOfTotal();
    static QList<QPair<QString, QString>> getLastReceivedTransactionsList();
    static int getLastReceivedTransactionsListModifyedCnt();
    static QList<QPair<QString, QString>> getLastSendedTransactionsList();
    static int getLastSendedTransactionsListModifyedCnt();
    static QMap<QString, double> getTokenPricePairs();
    static double getTokenPricePair(QString tokenPair);
    static bool getWalletLoaded();
    static QString getStyle();
    static int getStyleChangedCnt();
    static QPair<QString, QString> getWalletUserPassword();
    static int getWalletUserPasswordChangedCnt();
    static double getScale();
    static double getScaleStore();
    static double getScaleLater();
    static double getOsWindowScale();
    static bool getShowTransitionsWindow();
    static bool getWalletHistoryChanged();
    static bool getWalletHistoryChangedUnmodify();
    static bool getUpdateHistory();


    static void setNetworkConnected(bool connected);
    static void addWalletNameKeyList(QPair<QString, QString> wallet);
    static void removeWalletNameKeyList(QString wallet);
    static void replaceWalletNameKeyList(QString oldName, QString newName);
    static void setWalletNameKeyList(QList<QPair<QString, QString>> walletList);
    static void setSelectedNameKeyIndex(int index);
    static void setBells(int bells);
    static void setMessages(int messages);
    static void setNetwork(rpc::network_e net);
    static void setNodeList(QList<QPair<QString, bool>> nodes);
    static void setBallance(double bal);
    static void setUnreceivedBallance(QString bal);
    static void setTotalSupply(double bal);
    static void setCreationDateTime(qint64 dateTime);
    static void setLastSyncDateTime(qint64 dateTime);
    static void setNumberOfTransactions(int trans);
    static void setNumberOfTransactionsLastWeek(int trans);
    static void setMinimumDateTime(qint64 dateTime);
    static void setMyWalletBalanceChart(QList<QPair<qint64, double>> list);
    static void setMyWalletValueAllChart(QList<QPair<qint64, double>> list);
    static void setMyWalletValueChart(QList<QPair<qint64, double>> list);
    static void setBtcUsdSelect(bool sel);
    static void setTeamLockedReserved(double value);
    static void setCirculatingSupply(double value);
    static void setTotalBlockCount(qint64 value);
    static void setRecentTransactions(QList<QStringList> list);
    static void setRecentRxTransactions(QList<QStringList> list);
    static void setRecentTxTransactions(QList<QStringList> list);
    static void setAssets(QList<QStringList> list);
    static void setDateRange(QDate dateMin, QDate dateMax);
    static void setId(QString idStr);
    static void setTokenList(QList<QPair<QString, double>> list);
    static void setTotalLyraSendedLastWeek(double balance);
    static void setTotalLyraReceivedLastWeek(double balance);
    static void setLastReceivedTransactionsList(QList<QPair<QString, QString>> list);
    static void setLastSendedTransactionsList(QList<QPair<QString, QString>> list);
    static void setTokenPricePair(QString tokenPair, double price);
    static void setWalletLoaded(bool loaded);
    static void setStyle(QString style);
    static void setWalletUserPassword(QPair<QString, QString> usrPass);
    static void setScale(double s);
    static void setScaleLater(double s);
    static void setOsWindowScale(double s);
    static void setShowTransitionsWindow();
    static void setWalletHistoryChanged();
    static void setUpdateHistory();
};

#endif // EVENTS_H
