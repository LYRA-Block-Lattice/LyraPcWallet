#ifndef SETTINGSEDITUSER_H
#define SETTINGSEDITUSER_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class settingsedituser : public QMainWindow
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
    settingsedituser();
    ~settingsedituser();
    void init(QMdiArea *mdiArea = nullptr);
    return_e run();
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

    QLineEdit *userLineEdit = nullptr;
    QLabel *accountNameCheck = nullptr;
    QLineEdit *oldPasswordLineEdit = nullptr;
    QLineEdit *newPassword1LineEdit = nullptr;
    QLineEdit *newPassword2LineEdit = nullptr;

    QPushButton *cancelButton = nullptr;
    QPushButton *okButton = nullptr;

    runMode_e currentMode = NONE;
    runMode_e pastMode = NONE;
    return_e choose = RETURN_NONE;

    QString walletNames;

    double pastScale = 0.0;
    QString pastLanguage = "en";

private slots:
    void on_UserNameLineEdit_Changed(const QString &);
    void on_Cancel_ButtonPressed();
    void on_Ok_ButtonPressed();
    void on_oldPasswordLineEdit_textChanged(const QString &arg1);
    void on_newPassword1LineEdit_textChanged(const QString &arg1);
    void on_newPassword2LineEdit_textChanged(const QString &arg1);
};

#endif // SETTINGSEDITUSER_H
