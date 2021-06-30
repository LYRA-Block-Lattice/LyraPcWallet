#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>

class searchwindow : public QMainWindow {
    Q_OBJECT
public:
    typedef enum {
        STATE_NONE = 0,
        STATE_SEARCH
    }state_e;

    searchwindow();
    ~searchwindow();

    void setVars(QMdiSubWindow *window = nullptr, QWidget *parent = nullptr);
    void run();
    void setState(state_e state);
private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();

    QWidget *parent = nullptr;
    QMdiSubWindow *window = nullptr;

    QMdiArea *mdiAreaSearch = nullptr;
    QWidget *widgetSearch = nullptr;
    QMdiSubWindow *windowSearch = nullptr;

    QPushButton *searchPushButton = nullptr;

    state_e currentState = STATE_NONE;
    state_e pastState = STATE_SEARCH;
    double pastScale = 0.0;
    QString pastLanguage = "";
private slots:
    void on_openNebula_ButtonPressed();

};

#endif // SEARCHWINDOW_H
