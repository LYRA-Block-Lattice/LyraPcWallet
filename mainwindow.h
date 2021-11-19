#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMdiSubWindow>
#include <QWidget>
#include <QStyle>
#include <QPixmap>
#include <QGraphicsBlurEffect>

#include "keyrevealer.h"

#include "config.h"
#include "language/translate.h"
#include "pages/pc/login/login.h"

#include "pages/pc/main/def.h"
#include "pages/pc/main/mainboard.h"

#include "network/rpc.h"
#include "network/rpcapi.h"
#include "network/tokenpairing.h"

#include "wallet/tickedupdates.h"


typedef enum {
    APP_STATE_LOGIN = 0,
    APP_STATE_MAIN
}appState_e;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QString getPassword();
    static void setPassword(QString pass, bool persistent = false);
    static QWidget *getParent();
private:
    void windowToMain();
    void windowToLogin();

    Ui::MainWindow *ui;

    tokenpairing tokenPairing;
    tickedupdates tickedUpdates;

    QPoint cursorPressPos;
    bool cursorIsMoving{false};
    bool windowClosed = false;
    bool windowNoWarnOnClose = false;
    double blurRadius = 5.0;


    QTimer timerAppStart;
    QTimer timerLoop;

    QPixmap mainPixMap;

    bool deblurLoginPage = true;

    login *loginPage;

    mainWindowState_e mainWindowState = mainWindowState_e::MAIN_WINDOW_STATE_DASHBOARD;
    mainboard mainboardWindow;

    appState_e appState = APP_STATE_LOGIN;

    rpc *rpcConnection = nullptr;
    rpcapi *rpcApi = nullptr;
    double windowScale = -1;
    login::command_e loginCommand = login::command_e::CMD_NONE;
    bool pastNetworkConnection = false;
    int screenAt = 0;

protected:
    virtual void resizeEvent(QResizeEvent *) override;
    //void mousePressEvent(QMouseEvent* event) override;
    //void mouseMoveEvent(QMouseEvent* event) override;
    //void mouseReleaseEvent(QMouseEvent* /*event*/) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;
private slots:
    void appStart();
    void appMain();

    void on_closePushButton_clicked();
    void on_minimisePushButton_clicked();

    void passClear();
signals:
    //void logicalDotsPerInchChanged(qreal dpi);

};
#endif // MAINWINDOW_H
