#ifndef SETTINGSDELETEACCOUNT_H
#define SETTINGSDELETEACCOUNT_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class settingsdeleteaccount : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        NONE = 0,
        RUN,
        CANCEL,
        OK,
    }runMode_e;
    typedef enum {
        RETURN_NONE = 0,
        RETURN_CANCEL,
        RETURN_OK
    }return_e;
    settingsdeleteaccount();
    ~settingsdeleteaccount();
    void init(QMdiArea *mdiArea = nullptr);
    return_e run();
    runMode_e getState();
    void setState(runMode_e state, QString walletName = "");

private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();

    QWidget *parent = nullptr;
    QMdiArea *thisMdiArea = nullptr;
    QWidget *thisWidget = nullptr;
    QMdiSubWindow *thisWindow = nullptr;

    QLabel *titleName = nullptr;

    QLineEdit *passwordConfirmationLineEdit = nullptr;

    QPushButton *cancelButton = nullptr;
    QPushButton *okButton = nullptr;

    runMode_e currentMode = NONE;
    runMode_e pastMode = NONE;
    return_e choose = RETURN_NONE;

    QString walletName;

    double pastScale = 0.0;
    QString pastLanguage = "";

private slots:
    void on_Cancel_ButtonPressed();
    void on_Ok_ButtonPressed();
    void on_passwordConfirmationLineEdit_textChanged(const QString &arg1);
};

#endif // SETTINGSDELETEACCOUNT_H
