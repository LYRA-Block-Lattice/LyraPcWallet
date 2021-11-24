#include "walletfile.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QList>
#include <QPair>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

#include "config.h"
#include "wallet/events.h"
#include "wallet/rpc/wallethistory.h"
#include "crypto/signatures.h"
#include "crypto/aes.h"
#include "pages/pc/populate.h"
#include "language/translate.h"
#include "network/rpc.h"

QList<QPair<QString, QString>> accounts;

bool walletfile::load(QString fileName, QString password) {
    QString path = QString(USER_HOME) + "/" WALLET_PATH "/" + fileName + "." + DEFAULT_WALLET_EXTENSION;
    QFile trFile(path);
    if (!trFile.open(QIODevice::ReadOnly))
        return false;
    QByteArray walletFileEnc = trFile.readAll();
    QByteArray dec = aes::aesDecrypt(walletFileEnc, password);
    /*QFile dumpFile(QString(USER_HOME) + "/" WALLET_PATH "/dump.txt");
    if (!dumpFile.open(QIODevice::WriteOnly))
        return false;
    dumpFile.write(dec);
    dumpFile.close();*/
    trFile.close();
    QString walletFile(dec);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(walletFile.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    if(!jsonObject.contains("accounts")) {
        return false;
    }
    QJsonObject listPairsObject = jsonObject["lastPairs"].toObject();
    foreach(QString pair, listPairsObject.keys()) {
        events::setTokenPricePair(pair, listPairsObject.value(pair).toString().toDouble());
    }
    if(jsonObject.contains("accounts")) {
        events::setWalletLoaded(true);
    }
    QJsonObject accountsObject = jsonObject["accounts"].toObject();
    accounts.clear();
    foreach(QString account, accountsObject.keys()) {
        QJsonObject accountObject = accountsObject[account].toObject();
        accounts.append({account, accountObject["key"].toString()});

        QJsonObject testnetObject = accountObject["testnet"].toObject();
        QJsonArray historyArray = testnetObject["history"].toArray();
        wallethistory::addAccount(account, historyArray, 0);

        QJsonObject mainnetObject = accountObject["mainnet"].toObject();
        historyArray = mainnetObject["history"].toArray();
        wallethistory::addAccount(account, historyArray, 1);
    }
    events::setAccountNameKeyList(accounts);
    populate::refreshAll();
    return true;
}

bool walletfile::save(QString fileName, QString password, QString directory) {
    if(!events::getWalletHistoryChanged())
        return true;
    if(fileName.length() == 0)
        return false;
    QJsonObject objectFile;
    QJsonObject objectAccounts;
    QJsonObject lastPairs;
    QString pairName;
    QMap<QString, double> pairList = events::getTokenPricePairs();
    QStringList keyList = pairList.keys();
    foreach(pairName, keyList) {
        lastPairs.insert(pairName, QString::number(pairList.value(pairName)));
    }

    QMap<QString, QList<QList<QMap<QString, QString>>>> accounts = wallethistory::getAccounts(0);
    QJsonObject accountObject;
    QStringList accountNameList = accounts.keys();
    foreach(QString accountName, accountNameList) {
        accountObject.insert("key", events::getAccountKeyNoP(accountName));
        QJsonArray historyArrayTestnet = wallethistory::getAccountHistoryJson(accountName, 0);
        QJsonObject historyTestnet;
        historyTestnet.insert("history", historyArrayTestnet);
        accountObject.insert("testnet", historyTestnet);
        QJsonObject historyMainnet;
        QJsonArray historyArrayMainnet = wallethistory::getAccountHistoryJson(accountName, 1);
        historyTestnet.insert("history", historyArrayMainnet);
        accountObject.insert("mainnet", historyTestnet);
        objectAccounts.insert(accountName, accountObject);
    }
    objectFile.insert("accounts", objectAccounts);
    objectFile.insert("lastPairs", lastPairs);
    objectFile.insert("version", 1);
    QJsonDocument doc(objectFile);
    QString fileData = QString((doc.toJson(QJsonDocument::Indented)));
    QByteArray enc = aes::aesEncrypt(fileData.toUtf8(), password);
    QString path;
    if(directory.length() == 0) {
        QDir dir = QDir(QString(USER_HOME) + "/" WALLET_PATH "/");
        if(!dir.exists()) {
            dir.mkdir(QString(USER_HOME) + "/" WALLET_PATH "/");
        }
        QString dateTime = QDateTime::currentDateTimeUtc().toString("yyyy_MM_dd_hh_mm_ss");
        QFile::rename(QString(USER_HOME) + "/" WALLET_PATH "/" + events::getWalletUserPassword().first + "." + DEFAULT_WALLET_EXTENSION,
                      QString(USER_HOME) + "/" WALLET_PATH "/" + events::getWalletUserPassword().first + "_" + dateTime + "." + DEFAULT_WALLET_EXTENSION);
        path = QString(USER_HOME) + "/" WALLET_PATH "/" + fileName + "." + DEFAULT_WALLET_EXTENSION;
    } else {
        //QFileInfo file(directory);
        //path = file.completeBaseName() + "." + DEFAULT_WALLET_EXTENSION;
        path = directory;
    }
    QFile trFile(path);
    if (!trFile.open(QIODevice::WriteOnly))
        return false;
    trFile.write(enc);
    trFile.close();

    return true;
}

bool walletfile::exists(QString fileName) {
    return QFile::exists(QString(USER_HOME) + "/" WALLET_PATH "/" + fileName + "." + DEFAULT_WALLET_EXTENSION);
}

bool walletfile::loadSettings() {
    QString path = QString(USER_HOME) + "/" WALLET_PATH "/settings.dtb";
    QFile trFile(path);
    trFile.open(QIODevice::ReadOnly);
    QString settingsFile = trFile.readAll();
    trFile.close();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(settingsFile.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject settingsObject = jsonObject["settings"].toObject();
    if(settingsObject.contains("language")) {
        translate::loadLanguage(settingsObject["language"].toString());
    } else {
        translate::loadLanguage(DEFAULT_LANGUAGE);
    }
    if(settingsObject.contains("style")) {
        events::setStyle(settingsObject["style"].toString());
    } else {
        events::setStyle(DEFAULT_THEME);
    }
    if(settingsObject.contains("def_network")) {
        if(!settingsObject["def_network"].toString().compare("testnet")) {
            events::setNetwork(rpc::network_e::NETWORK_TESTNET);
        } else if(!settingsObject["def_network"].toString().compare("mainnet")) {
            events::setNetwork(rpc::network_e::NETWORK_MAINNET);
        }
    } else {
        events::setNetwork(rpc::network_e::NETWORK_MAINNET);
    }
    if(settingsObject.contains("window_scale")) {
        events::setScale(settingsObject["window_scale"].toDouble());
    } else {
        events::setScale(DEFAULT_SCALE);
    }
    if(settingsObject.contains("lastHeight")) {
        events::setTotalBlockCount(settingsObject["lastHeight"].toVariant().toLongLong());
    } else {
        events::setTotalBlockCount(0);
    }
    if(settingsObject.contains("def_value")) {
        if(!settingsObject["def_value"].toString().compare("BTC")) {
            events::setBtcUsdSelect(false);
        } else if(!settingsObject["def_value"].toString().compare("USD")) {
            events::setBtcUsdSelect(true);
        } else {
            events::setBtcUsdSelect(false);
        }
    }
    if(settingsObject.contains("custom_nede_ip_testnet")) {
        events::setCustomIp(settingsObject["custom_nede_ip_testnet"].toString(), events::network_e::NETWORK_TESTNET);
    } else {
        events::setCustomIp("", events::network_e::NETWORK_TESTNET);
    }
    if(settingsObject.contains("custom_nede_ip_mainnet")) {
        events::setCustomIp(settingsObject["custom_nede_ip_mainnet"].toString(), events::network_e::NETWORK_MAINNET);
    } else {
        events::setCustomIp("", events::network_e::NETWORK_MAINNET);
    }
    return true;
}

bool walletfile::saveSettings() {
    QJsonObject settingsObject;
    settingsObject.insert("language", translate::getCurrentLang());
    settingsObject.insert("style", events::getStyle());
    if(events::getNetwork() == rpc::network_e::NETWORK_TESTNET) {
        settingsObject.insert("def_network", "testnet");
    } else if(events::getNetwork() == rpc::network_e::NETWORK_MAINNET) {
        settingsObject.insert("def_network", "mainnet");
    }
    if(events::getBtcUsdSelect()) {
        settingsObject.insert("def_value", "USD");
    } else {
        settingsObject.insert("def_value", "BTC");
    }
    settingsObject.insert("window_scale", events::getScaleStore());
    settingsObject.insert("lastHeight", events::getTotalBlockCount());
    settingsObject.insert("custom_nede_ip_testnet", events::getCustomIp(events::network_e::NETWORK_TESTNET));
    settingsObject.insert("custom_nede_ip_mainnet", events::getCustomIp(events::network_e::NETWORK_MAINNET));
    QJsonObject objectFile;
    objectFile.insert("settings", settingsObject);
    QDir directory = QDir(QString(USER_HOME) + "/" WALLET_PATH "/");
    if(!directory.exists()) {
        directory.mkdir(QString(USER_HOME) + "/" WALLET_PATH "/");
    }
    QString path = QString(USER_HOME) + "/" WALLET_PATH "/settings.dtb";
    QFile trFile(path);
    if (!trFile.open(QIODevice::WriteOnly))
        return false;
    QJsonDocument doc(objectFile);
    QString fileData = QString((doc.toJson(QJsonDocument::Indented)));
    trFile.write(fileData.toLocal8Bit());
    trFile.close();
    return true;
}

