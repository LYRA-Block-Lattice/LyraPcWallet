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

    QWidget *parent;
    QMdiArea *thisMdiArea;
    QWidget *thisWidget;
    QMdiSubWindow *thisWindow;

    QLabel *titleName;

    QLineEdit *privateKey;

    QPushButton *okButton;

    runMode_e currentMode = NONE;
    runMode_e pastMode = NONE;

    double pastScale = 0.0;
    QString pastLanguage = "en";

private slots:
    void on_Ok_ButtonPressed();

};

#endif // SETTINGSSHOWPRIVKEY_H
