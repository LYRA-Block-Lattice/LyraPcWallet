#ifndef WALLETSHOWDETAILS_H
#define WALLETSHOWDETAILS_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QList>
#include <QMap>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>

class walletshowdetails : public QMainWindow
{
    Q_OBJECT
public:
    walletshowdetails();
    ~walletshowdetails();
    void init(QMdiArea *mdiArea = nullptr);
    void run();
    void show(QList<QMap<QString, QString>> transaction);
    void show(QString text);
    void setDetailsVisible(bool visible);

private:
    QWidget *parent;
    QMdiArea *thisMdiArea;
    QWidget *thisWidget;
    QMdiSubWindow *thisWindow;

    QTextEdit *detailTextEdit;
    QPushButton *backPushButton;
    QPushButton *savePushButton;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;

    double pastScale = 0.0;
    QString pastLanguage = "en";
    bool back = false;
    int size = 0;
private slots:
    void on_Back_ButtonPressed();

};

#endif // WALLETSHOWDETAILS_H
