#include "tokenpairing.h"

#include <QVariant>
#include <QApplication>

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
    manager.setParent(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(replyFinished(QNetworkReply*)));
    reply = manager.get(QNetworkRequest(QUrl(url)));
    connect(reply, SIGNAL(readyRead()),
        this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(finished()),
        this, SLOT(httpDownloadFinished()));
    read.clear();
    status.clear();
    busy = true;
    while(1) {
        if(!busy)
            return read;
        QApplication::processEvents();
        if(events::getAppClosing())
            return read;
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
