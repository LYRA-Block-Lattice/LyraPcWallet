#ifndef TOKENPAIRING_H
#define TOKENPAIRING_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class tokenpairing : public QWidget
{
    Q_OBJECT
public:
    tokenpairing();
    ~tokenpairing();
    static tokenpairing *getInstance();
    void init(QWidget *parent = nullptr, tokenpairing *instance = nullptr);
    QString get(QString url);
    static QString getStatic(QString url);

    QString status;
private:
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QString read;
    bool busy = true;
signals:
private slots:
    void replyFinished(QNetworkReply *resp);
    void httpReadyRead();
    void httpDownloadFinished();
};

#endif // TOKENPAIRING_H
