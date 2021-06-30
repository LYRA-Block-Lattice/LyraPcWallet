#ifndef SETTINGSSHOWPRIVKEY_H
#define SETTINGSSHOWPRIVKEY_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class settingsshowprivkey : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        NONE = 0,
        RUN
    }runMode_e;
    settingsshowprivkey();
    ~settingsshowprivkey();
    void init(QMdiArea *mdiArea = nullptr);
    void run();
    runMode_e getState();
    void setState(runMode_e state, QString walletName = "", QString privKey = "");

private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();

    QWidget *parent = nullptr;
    QMdiArea *thisMdiArea = nullptr;
    QWidget *thisWidget = nullptr;
    QMdiSubWindow *thisWindow = nullptr;

    QLabel *titleName = nullptr;

    QLineEdit *privateKey = nullptr;

    QPushButton *okButton = nullptr;

    runMode_e currentMode = NONE;
    runMode_e pastMode = NONE;

    double pastScale = 0.0;
    QString pastLanguage = "";

private slots:
    void on_Ok_ButtonPressed();

};

#endif // SETTINGSSHOWPRIVKEY_H
