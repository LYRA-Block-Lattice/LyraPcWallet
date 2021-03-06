#ifndef WALLETPAGE_H
#define WALLETPAGE_H

#include <QMainWindow>
#include <QWidget>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "walletmain.h"
#include "walletreceive.h"
#include "walletsend.h"

#include "config.h"
#include "wallet/events.h"
#include "walletshowdetails.h"

class walletpage : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        STATE_NONE = 0,
        STATE_MAIN,
        STATE_SEND,
        STATE_RECEIVE,
    }state_e;

    walletpage();
    ~walletpage();
    void setVars(QMdiSubWindow *window = nullptr, QWidget *parent = nullptr);
    void run();
    void setState(state_e state);
    QMdiSubWindow *window;
    QMdiSubWindow *windowWallet = nullptr;
    QMdiSubWindow *windowSend = nullptr;
    QMdiSubWindow *windowReceive = nullptr;
private:
    /* Window */
    QWidget *parent;

    QMdiArea *mdiAreaWallet = nullptr;
    QWidget *widgetWallet = nullptr;
    QMdiArea *mdiAreaSend = nullptr;
    QWidget *widgetSend = nullptr;
    QMdiArea *mdiAreaReceive = nullptr;
    QWidget *widgetReceive = nullptr;


    walletmain *walletMain = nullptr;
    walletreceive *walletReceive = nullptr;
    walletsend *walletSend = nullptr;


    state_e currentState = STATE_MAIN;
    state_e pastState = STATE_MAIN;
    double pastScale = 0.0;
    QString pastLanguage = "";
signals:

};

#endif // WALLETPAGE_H
