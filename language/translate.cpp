#include "translate.h"

#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

#include "config.h"
#include "wallet/events.h"

QJsonObject languagemap;
QString language = "en";

bool translate::loadLanguage(QString lan) {
    QStringList trList;
    //Q_INIT_RESOURCE(Resources);
    QFile trFile(":/resource/lang/" + lan + ".txt");
    if (!trFile.open(QIODevice::ReadOnly))
        return false;
    QString tmpTrFile = trFile.readAll();
    /*trList = tmpTrFile.remove('{').remove('}').remove(',').remove('\"').split("\r\n");
    trList.removeAll("");
    for(int cnt = 0; cnt < trList.count(); cnt++) {
        trList[cnt] = trList[cnt].simplified();
    }
    foreach(QString str, trList) {
        QStringList data = str.split(": ");
        data[1] = data[1].remove(0,1);
        if(data.count() > 2) {
            for(int cnt = 2; cnt < data.count(); cnt++) {
                data[1].append(":" + data[cnt]);
            }
        }
        data[1] = data[1].replace("\\n", "\n").replace("\\r", "\r");
        languagemap.insert(data[0], data[1]);
    }*/
    QJsonDocument jsonResponse = QJsonDocument::fromJson(tmpTrFile.toUtf8());
    languagemap = jsonResponse.object();

    language = lan;
    return true;
}

QString translate::getCurrentLang() {
    return language;
}

QString translate::getCurrentFontLight() {
    if(language == "cn") {
        return ":/resource/font/SimplifiedChinese/SourceHanSerifSC-Light.otf";
    } else {
        return ":/resource/font/roboto/Roboto-Light.ttf";
    }
}

int translate::getNumberFontSize(double scale) {
    QList<QScreen *> primaryScreen = QGuiApplication::screens();
    return _scale(DEFAULT_FONT_SIZE, scale / events::getOsWindowScale() / FONT_REDUCTION * ((double)primaryScreen[events::getScreenNumber()]->geometry().height() / (double)WINDOW_HEIGHT / 2));
}

int translate::getCurrentFontSizeLight(double scale) {
    QList<QScreen *> primaryScreen = QGuiApplication::screens();
    if(language == "cn") {
        return _scale(DEFAULT_FONT_SIZE + 4, scale / events::getOsWindowScale() / FONT_REDUCTION * ((double)primaryScreen[events::getScreenNumber()]->geometry().height() / (double)WINDOW_HEIGHT / 2));
    } else {
        return _scale(DEFAULT_FONT_SIZE, scale / events::getOsWindowScale() / FONT_REDUCTION * ((double)primaryScreen[events::getScreenNumber()]->geometry().height() / (double)WINDOW_HEIGHT / 2));
    }
}

QString translate::getCurrentFontMedium() {
    if(language == "cn") {
        return ":/resource/font/SimplifiedChinese/SourceHanSerifSC-Light.otf";
    } else {
        return ":/resource/font/roboto/Roboto-Medium.ttf";
    }
}

int translate::getCurrentFontSizeMedium(double scale) {
    QList<QScreen *> primaryScreen = QGuiApplication::screens();
    if(language == "cn") {
        return _scale(DEFAULT_FONT_SIZE + 4, scale / events::getOsWindowScale() / FONT_REDUCTION * ((double)primaryScreen[events::getScreenNumber()]->geometry().height() / (double)WINDOW_HEIGHT / 2));
    } else {
        return _scale(DEFAULT_FONT_SIZE, scale / events::getOsWindowScale() / FONT_REDUCTION * ((double)primaryScreen[events::getScreenNumber()]->geometry().height() / (double)WINDOW_HEIGHT / 2));
    }
}

QString translate::getCurrentFontRegular() {
    if(language == "cn") {
        return ":/resource/font/SimplifiedChinese/SourceHanSerifSC-Light.otf";
    } else {
        return ":/resource/font/roboto/Roboto-Regular.ttf";
    }
}

int translate::getCurrentFontSizeRegular(double scale) {
    QList<QScreen *> primaryScreen = QGuiApplication::screens();
    if(language == "cn") {
        return _scale(DEFAULT_FONT_SIZE + 4, scale / events::getOsWindowScale() / FONT_REDUCTION * ((double)primaryScreen[events::getScreenNumber()]->geometry().height() / (double)WINDOW_HEIGHT / 2));
    } else {
        return _scale(DEFAULT_FONT_SIZE, scale / events::getOsWindowScale() / FONT_REDUCTION * ((double)primaryScreen[events::getScreenNumber()]->geometry().height() / (double)WINDOW_HEIGHT / 2));
    }
}


QString _tr(QString str) {
    /*QMap<QString, QString>::iterator i = languagemap.find(str);
    while (i != languagemap.end() && i.key() == str) {
        return i.value();
        //++i;
    }*/
    if(languagemap.contains(str)) {
        return languagemap[str].toString().replace("<__VERSION__>", APP_VERSION);
    }
    return "?" + str;
}

int _scale(int value, double extra) {
    double tmp = (double)value * events::getScale() * extra;
    return (int)tmp;
}

