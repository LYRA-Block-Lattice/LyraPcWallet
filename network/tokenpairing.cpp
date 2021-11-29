#include "tokenpairing.h"

#include <QVariant>
#include <QApplication>
#include <QTimer>

#include "wallet/events.h"

tokenpairing *tokenPairing = nullptr;

tokenpairing::tokenpairing() {
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
}

tokenpairing::~tokenpairing() {

}

void tokenpairing::init(QWidget *parent, tokenpairing *instance) {
    this->setParent(parent);
    tokenPairing = instance;
}

tokenpairing *tokenpairing::getInstance() {
    return tokenPairing;
}

QString tokenpairing::getStatic(QString url) {
    return tokenPairing->get(url);
}

QString tokenpairing::get(QString url) {
    QTimer timer;
    manager.setParent(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(replyFinished(QNetworkReply*)));
    reply = manager.get(QNetworkRequest(QUrl(url)));
    connect(reply, SIGNAL(readyRead()),
        this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(finished()),
        this, SLOT(httpDownloadFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
           this, SLOT(slotError(QNetworkReply::NetworkError)));
    read.clear();
    status.clear();
    busy = true;
    error = false;
    timedOut = false;
    QTimer::singleShot(WEB_MESSAGE_RESPONSE_TIMEOUT, this, &tokenpairing::timedOutFunc);
    while(1) {
#if VORBOSE_LEVEL >= 5
        qDebug() << "TOKENPAIRING 4 :Loop ";
#endif
        if(!busy || error || events::getAppClosing() || timedOut) {
            timedOut = true;
            return read;
        }
        QApplication::processEvents();
    }
}

void tokenpairing::replyFinished(QNetworkReply *resp){
    QVariant status_code = resp->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(status_code.isValid()){
        // Print or catch the status code
        status = status_code.toString(); // or status_code.toInt();
        //qDebug() << "TOKENPAIRING 1: " << status;
    }
}

void tokenpairing::httpReadyRead() {
    read.append(reply->readAll());
}

void tokenpairing::httpDownloadFinished() {
    //qDebug() << read;
    busy = false;
}

void tokenpairing::slotError(QNetworkReply::NetworkError err) {
    error = true;
    qDebug() << "TOKENPAIRING 1: ";
    qDebug() << "TOKENPAIRING 2: " << err;
}

void tokenpairing::timedOutFunc() {
    if(!timedOut) {
        timedOut = true;
        qDebug() << "TOKENPAIRING 3: TimedOut";
    }
}
