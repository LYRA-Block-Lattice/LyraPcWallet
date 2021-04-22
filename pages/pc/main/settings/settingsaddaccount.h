#ifndef SETTINGSADDACCOUNT_H
#define SETTINGSADDACCOUNT_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class settingsaddaccount : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        RETURN_NONE = 0,
        RETURN_CANCEL,
        RETURN_OK
    }return_e;
    typedef enum {
        NONE = 0,
        CHOOSE,
        NEW,
        RECOVER,
        CANCEL,
        OK
    }runMode_e;
    settingsaddaccount();
    ~settingsaddaccount();
    void init(QMdiArea *mdiArea = nullptr);
    return_e run();
    QString getAccountName();
    QString getPrivateKey();
    runMode_e getState();
    void setState(runMode_e state);
private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();

    QWidget *parent = nullptr;
    QMdiArea *thisMdiArea = nullptr;
    QWidget *thisWidget = nullptr;
    QMdiSubWindow *thisWindow = nullptr;

    QLabel *titleName = nullptr;

    QPushButton *newAccountButton = nullptr;
    QPushButton *recoverAccountButton = nullptr;

    QLineEdit *accountNameLineEdit = nullptr;
    QLabel *accountNameCheck = nullptr;
    QLineEdit *privateKeyLineEdit = nullptr;
    QLabel *privateKeyCheck = nullptr;

    QLineEdit *passwordConfirmationLineEdit = nullptr;

    QPushButton *cancelButton = nullptr;
    QPushButton *okButton = nullptr;

    QString accountName;
    QString privateKey;

    runMode_e currentMode = NONE;
    runMode_e pastMode = NONE;
    return_e choose = RETURN_NONE;

    QString walletName;

    double pastScale = 0.0;
    QString pastLanguage = "en";
private slots:
    void on_New_ButtonPressed();
    void on_Recover_ButtonPressed();
    void on_Cancel_ButtonPressed();
    void on_Ok_ButtonPressed();

    void on_AccountNameLineEdit_Changed(const QString &);
    void on_PrivateKeyLineEdit_Changed(const QString &);

};

#endif // SETTINGSADDACCOUNT_H
