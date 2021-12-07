#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QMainWindow>
#include <QWidget>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "dashmain.h"

#include "config.h"
#include "wallet/events.h"

class dashboardpage : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        STATE_NONE = 0,
        STATE_MAIN,
        STATE_RECENT_TRANSACTIONS,
        STATE_ASSETS,
    }state_e;

    dashboardpage();
    ~dashboardpage();
    void setVars(QMdiSubWindow *window = nullptr, QWidget *parent = nullptr);
    //~dashboard();
    void run();
    void setState(state_e state);
    QMdiSubWindow *window = nullptr;
private:
    /* Window */
    QWidget *parent = nullptr;
    QWidget *widget = nullptr;
    //QMdiArea *mdiArea;
    QMdiArea *mdiAreaDashboard = nullptr;
    QMdiSubWindow *windowDashboard = nullptr;

    dashmain *dashMain = nullptr;


    state_e currentState = STATE_MAIN;
    state_e pastState = STATE_MAIN;
    double pastScale = 0.0;
    QString pastLanguage = "";
signals:

};

#endif // DASHBOARDPAGE_H
