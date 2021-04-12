#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include "config.h"

class login : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        STATE_NONE = 0,
        STATE_LOGIN_PAGE,
        STATE_LOGIN_WALLET,
        STATE_CREATE_MANAGER,
        STATE_CREATE_WALLET,
        STATE_OPEN_WALLET,
        STATE_RESTORE_WALLET
    }state_e;

    typedef enum {
        CMD_NONE = 0,
        CMD_LOGIN_WALLET,
        CMD_CREATE_MANAGER,
        CMD_CREATE_WALLET,
        CMD_OPEN_WALLET,
        CMD_RESTORE_WALLET
    }command_e;

    login(QMdiArea *mdiArea = nullptr, QWidget *parent = nullptr);
    ~login();
    //void setVars();
    void run();
    command_e getCommand();
    void setState(state_e state);
private:
    /* Window */
    QWidget *parent;
    QMdiArea *mdiArea;
    QMdiSubWindow *window;
    QWidget *widget;

    QLabel *titleText;

    QLineEdit *privKeyLineEdit;
    QLineEdit *nameLineEdit;
    QLineEdit *pass1LineEdit;
    QLineEdit *pass2LineEdit;

    QPushButton *createNewUser;
    //QPushButton *createNewWallet;
    //QPushButton *openWallet;
    //QPushButton *restoreWallet;
    QPushButton *loginManager;
    QPushButton *getStarted;
    QPushButton *back;
    QPushButton *info;
    QLabel *infoMessage;

    //QLabel *loginManagerUser;
    QLabel *lyraInc;

    QString privKey;
    QString name;
    QString pass1;
    QString pass2;

    state_e currentState = STATE_NONE;
    state_e pastState = STATE_NONE;
    double pastScale = DEFAULT_SCALE;
    QString pastLanguage = "en";
    bool infoSwitch = false;
    command_e command = CMD_NONE;

    bool actionSemaphore = false;

signals:

private slots:
    void on_privateKeyLineEdit_textChanged(const QString &);
    void on_nameNameLineEdit_textChanged(const QString &);
    void on_pass1LineEdit_textChanged(const QString &);
    void on_pass2LineEdit_textChanged(const QString &);
    void on_createNewWallet_pushed();
    void on_openWallet_pushed();
    void on_restoreWallet_pushed();
    void on_login_pushed();
    void on_getStarted_pushed();
    void on_back_pushed();
    void on_info_pushed();

};

#endif // LOGIN_H
