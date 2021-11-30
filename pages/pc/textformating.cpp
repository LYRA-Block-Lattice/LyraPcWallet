#include "textformating.h"

#include <QVariant>
#include <QDate>

#include "language/translate.h"

QString textformating::toValue(double sum, int precision, bool noComma) {
    QString tmp = QString::number(sum,'f', precision).remove('-');//QVariant{sum}.toString();
    if(!noComma) {
        int loc = tmp.indexOf('.');
        if(loc != -1) {
            /*if(tmp.length() - loc > 10)
                tmp.insert(loc + 10, ',');
            if(tmp.length() - loc > 7)
                tmp.insert(loc + 7, ',');
            if(tmp.length() - loc > 4)
                tmp.insert(loc + 4, ',');*/
            if(loc > 12) {
                tmp.insert(loc - 3, ',');
                tmp.insert(loc - 6, ',');
                tmp.insert(loc - 9, ',');
                tmp.insert(loc - 12, ',');
            } else if(loc > 9) {
                tmp.insert(loc - 3, ',');
                tmp.insert(loc - 6, ',');
                tmp.insert(loc - 9, ',');
            } else if(loc > 6) {
                tmp.insert(loc - 3, ',');
                tmp.insert(loc - 6, ',');
            } else if(loc > 3)
                tmp.insert(loc - 3, ',');
        } else {
            int len = tmp.length();
            if(len > 12) {
                tmp.insert(len - 3, ',');
                tmp.insert(len - 6, ',');
                tmp.insert(len - 9, ',');
                tmp.insert(len - 12, ',');
            } else if(len > 9) {
                tmp.insert(len - 3, ',');
                tmp.insert(len - 6, ',');
                tmp.insert(len - 9, ',');
            } else if(len > 6) {
                tmp.insert(len - 3, ',');
                tmp.insert(len - 6, ',');
            } else if(len > 3)
                tmp.insert(len - 3, ',');
        }
    }
    return tmp;
}

QString textformating::toValue(QString value, int precision) {
    return textformating::toValue(value.toDouble(), precision);
}

QString textformating::toPercentage(double sum) {
    return QString::asprintf("%3.4f %%", sum);
}

QString textformating::toTranslatedMonth(int month) {
    switch(month) {
    case 1:
        return QString(_tr("January"));
    case 2:
        return QString(_tr("February"));
    case 3:
        return QString(_tr("March"));
    case 4:
        return QString(_tr("April"));
    case 5:
        return QString(_tr("May"));
    case 6:
        return QString(_tr("June"));
    case 7:
        return QString(_tr("July"));
    case 8:
        return QString(_tr("August"));
    case 9:
        return QString(_tr("September"));
    case 10:
        return QString(_tr("October"));
    case 11:
        return QString(_tr("November"));
    case 12:
        return QString(_tr("December"));
    default:
        return QString(_tr("Month Out Of Range"));
    }
}

QString textformating::toDate(qint64 dateTime) {
    QDateTime date = QDateTime::fromMSecsSinceEpoch(dateTime);
    return QString(date.toString("dd") + "/" + toTranslatedMonth(date.toString("M").toInt()) + "/" + date.toString("yyyy"));
}

QString textformating::toTime(qint64 dateTime) {
    QDateTime date = QDateTime::fromMSecsSinceEpoch(dateTime);
    return date.toString("hh:mm:ss");
}




