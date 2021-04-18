#ifndef SETTINGSEDITACCOUNT_H
#define SETTINGSEDITACCOUNT_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class settingseditaccount : public QMainWindow
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
    settingseditaccount();
    ~settingseditaccount();
    void init(QMdiArea *mdiArea = nullptr);
    return_e run();
    runMode_e getState();
    void setState(runMode_e state, QString walletName = "");

private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();

    QWidget *parent;
    QMdiArea *thisMdiArea;
    QWidget *thisWidget;
    QMdiSubWindow *thisWindow;

    QLabel *titleName;

    QLineEdit *nameLineEdit;

    QLineEdit *passwordConfirmationLineEdit;

    QPushButton *cancelButton;
    QPushButton *okButton;

    runMode_e currentMode = NONE;
    runMode_e pastMode = NONE;
    return_e choose = RETURN_NONE;

    QString walletName;

    double pastScale = 0.0;
    QString pastLanguage = "en";

private slots:
    void on_Cancel_ButtonPressed();
    void on_Ok_ButtonPressed();
};

#endif // SETTINGSEDITACCOUNT_H
