#include <QResizeEvent>
#include <QScreen>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QPainter>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QCursor>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "language/translate.h"

#include "crypto/base58Encoding.h"
#include "crypto/signatures.h"
#include "wallet/rpc/walletbalance.h"
#include "wallet/rpc/wallethistory.h"
#include "wallet/events.h"
#include "pages/pc/populate.h"

#include "storage/walletfile.h"

/*#define MAIN_WINDOW_SIZE_X      (1000)
#define MAIN_WINDOW_SIZE_Y      (712 + TITLEBAR_HEIGHT)
#define LOGIN_WINDOW_SIZE_X      (2700)
#define LOGIN_WINDOW_SIZE_Y      (1920 + TITLEBAR_HEIGHT)

#define WINDOW_DIMENSION_CHANGE_SPEED   100.0*/


#define s(s) _scale(s)


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {


    walletfile::loadSettings();


    ui->setupUi(this);
    this->repaint();
    QApplication::processEvents();



    tokenPairing.init(this, &tokenPairing);
    tickedUpdates.init(this, &tickedUpdates);

    if(TITLEBAR_HEIGHT != 0) {
        ui->closePushButton->setVisible(false);
        ui->minimisePushButton->setVisible(false);
    }

    this->installEventFilter(this);
#if (TITLEBAR_HEIGHT == 0)
    this->setWindowFlags( Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
#else
    this->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
#endif
    //this->setWindowFlag(Qt::FramelessWindowHint, true);
    ui->closePushButton->setWindowFlag(Qt::FramelessWindowHint, true);
    ui->minimisePushButton->setWindowFlag(Qt::FramelessWindowHint, true);
#ifdef Q_OS_OSX
    ui->minimisePushButton->setVisible(false);
#endif
    mainPixMap.fill(Qt::white);

    ui->mainMdiArea->setParent(this);
    ui->mainMdiArea->setBackground(QBrush(mainPixMap));
    //ui->mainMdiArea->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_Light.png)");

    //ui->mainMdiArea->setAttribute(Qt::WA_NoBackground);
    //ui->mainMdiArea->setAttribute(Qt::WA_NoSystemBackground);
    ui->mainMdiArea->setAttribute(Qt::WA_TranslucentBackground);
    //ui->mainMdiArea->setAttribute(Qt::WA_DeleteOnClose);
    //ui->mainMdiArea->setProperty("windowOpacity", 0.1);

    QSize size = this->size();
    ui->mainMdiArea->setGeometry(0, s(TITLEBAR_HEIGHT), size.rwidth(), size.rheight());
    ui->mainMdiArea->repaint();

    //ui->mainMdiArea->setGraphicsEffect(blurEffect);
    ui->centralwidget->update();
    ui->mainMdiArea->update();
    timerAppStart.setInterval(500);
    connect(&timerAppStart, SIGNAL(timeout()), this, SLOT(appStart()));

    timerLoop.setInterval(10);
    connect(&timerLoop, SIGNAL(timeout()), this, SLOT(appMain()));

    loginPage = new login(ui->mainMdiArea, ui->centralwidget);
    mainboardWindow.setVars(ui->mainMdiArea, ui->centralwidget);
    //loginPage->setVars(ui->mainMdiArea, ui->centralwidget);

    rpcConnection = new rpc(this);
    rpcApi = new rpcapi(rpcConnection);


    timerAppStart.start();
    timerLoop.start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    /*QSize size = event->size();
    ui->mainMdiArea->setGeometry(0, s(TITLEBAR_HEIGHT), size.rwidth(), size.rheight() - s(TITLEBAR_HEIGHT));
    ui->mainMdiArea->repaint();
    ui->closePushButton->setGeometry(this->width() - s(41), s(ui->closePushButton->y()), s(41), s(TITLEBAR_HEIGHT));
    ui->minimisePushButton->setGeometry(this->width() - s(82), s(ui->minimisePushButton->y()), s(41), s(TITLEBAR_HEIGHT));*/
    event->accept();
}

/*void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && event->pos().y() < s(TITLEBAR_HEIGHT)){
        cursorPressPos = event->pos();
        cursorIsMoving = true;
        event->accept();
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if(cursorIsMoving){
        QPoint diff = event->pos() - cursorPressPos;
        window()->move(window()->pos() + diff);
        event->accept();
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    cursorIsMoving = false;
    event->accept();
}*/

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == this) {
        if(!windowClosed) {
            if(blurRadius) {
                //QGraphicsBlurEffect *blurEffectCentralWidget = new QGraphicsBlurEffect();
                //blurEffectCentralWidget->setBlurRadius(blurRadius);
                //ui->centralwidget->setGraphicsEffect(blurEffectCentralWidget);
                //QGraphicsBlurEffect *blurEffectMainMdiArea = new QGraphicsBlurEffect();
                //blurEffectMainMdiArea->setBlurRadius(blurRadius);
                //ui->mainMdiArea->setGraphicsEffect(blurEffectMainMdiArea);
            }
            else {
                ui->centralwidget->setGraphicsEffect(nullptr);
                ui->mainMdiArea->setGraphicsEffect(nullptr);
            }
        }
        if (event->type() == QEvent::Close && !windowNoWarnOnClose) {
            QMessageBox::StandardButton resBtn = QMessageBox::question( this, this->windowTitle(),
                                                                        _tr("Are you sure you want to exit?"),
                                                                        QMessageBox::No | QMessageBox::Yes,
                                                                        QMessageBox::No);
            if (resBtn != QMessageBox::Yes) {
                event->ignore();
            } else {
                timerLoop.stop();
                /*if(mainboardWindow)
                    mainboardWindow->close();*/
                walletfile::save(events::getWalletUserPassword().first, events::getWalletUserPassword().second);
                walletfile::saveSettings();
                event->accept();
            }
            return true;
        } else {
            return QMainWindow::eventFilter(obj, event);
        }
        return QMainWindow::eventFilter(obj, event);
    } else {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, event);
    }
}

void MainWindow::on_closePushButton_clicked() {
    this->close();
}

void MainWindow::on_minimisePushButton_clicked() {
#ifdef Q_OS_OSX
    /*setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & (~Qt::WindowTitleHint));
    showMinimized();
    setWindowFlags(windowFlags() & (~Qt::CustomContextMenu & ~Qt::WindowTitleHint) | Qt::FramelessWindowHint);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    show();*/
    this->showMinimized();
#else
    this->showMinimized();
#endif
}

void MainWindow::windowToMain() {
#if 0
    ui->closePushButton->setVisible(false);
    ui->minimisePushButton->setVisible(false);
    ui->languageComboBox->setVisible(false);
    double stepsX = ((double)MAIN_WINDOW_SIZE_X - (double)LOGIN_WINDOW_SIZE_X) / WINDOW_DIMENSION_CHANGE_SPEED;
    double stepsY = ((double)MAIN_WINDOW_SIZE_Y - (double)LOGIN_WINDOW_SIZE_Y) / WINDOW_DIMENSION_CHANGE_SPEED;
    double left = (double)this->x();
    double up = (double)this->y();
    double right = (double)this->x() + (double)LOGIN_WINDOW_SIZE_X;
    double down = (double)this->y() + (double)LOGIN_WINDOW_SIZE_Y;
    double radius = 20.0;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    while(1) {
        /*QEventLoop loop;
        QTimer::singleShot(10, &loop, SLOT(quit()));
        loop.exec();*/
        if(left > 0.0 && right < (double)screenGeometry.width()) {
            left -= stepsX;
            right += stepsX;
        } else if(left < 0.0) {
            right += stepsX * 2.0;
        } else {
            left -= stepsX * 2.0;
        }
        if(up > 0.0 && down < (double)screenGeometry.height()) {
            up -= stepsY;
            down += stepsY;
        } else if(up < 0.0) {
            down += stepsY * 2.0;
        } else {
            up -= stepsY * 2.0;
        }
        if(right - left >= (double)MAIN_WINDOW_SIZE_X || down - up >= (double)MAIN_WINDOW_SIZE_Y)
            break;
        this->setGeometry(left, up, right - left, down - up);

        radius -= 20.0 / (WINDOW_DIMENSION_CHANGE_SPEED / 2);
        QGraphicsBlurEffect *blurEffectCentralWidget = new QGraphicsBlurEffect();
        blurEffectCentralWidget->setBlurRadius(radius);
        ui->mainMdiArea->setGraphicsEffect(blurEffectCentralWidget);

        QGraphicsBlurEffect *blurEffectLabel = new QGraphicsBlurEffect();
        blurEffectLabel->setBlurRadius(radius);
        ui->label->setGraphicsEffect(blurEffectLabel);

        QApplication::processEvents();
    }
    this->setGeometry(left, up, MAIN_WINDOW_SIZE_X, MAIN_WINDOW_SIZE_Y);
    QGraphicsBlurEffect *blurEffectCentralWidget = new QGraphicsBlurEffect();
    blurEffectCentralWidget->setBlurRadius(0);
    ui->mainMdiArea->setGraphicsEffect(blurEffectCentralWidget);
    ui->closePushButton->setVisible(true);
    ui->minimisePushButton->setVisible(true);
    ui->languageComboBox->setVisible(true);

    mainboardWindow->setVisible(true);
#endif
}

void MainWindow::windowToLogin() {
#if 0
    mainboardWindow->setVisible(false);

    ui->closePushButton->setVisible(false);
    ui->minimisePushButton->setVisible(false);
    ui->languageComboBox->setVisible(false);
    double stepsX = ((double)MAIN_WINDOW_SIZE_X - (double)LOGIN_WINDOW_SIZE_X) / WINDOW_DIMENSION_CHANGE_SPEED;
    double stepsY = ((double)MAIN_WINDOW_SIZE_Y - (double)LOGIN_WINDOW_SIZE_Y) / WINDOW_DIMENSION_CHANGE_SPEED;
    double left = (double)this->x();
    double up = (double)this->y();
    double right = (double)this->x() + (double)MAIN_WINDOW_SIZE_X;
    double down = (double)this->y() + (double)MAIN_WINDOW_SIZE_Y;
    double radius = 0.0;
    while(1) {
        /*QEventLoop loop;
        QTimer::singleShot(10, &loop, SLOT(quit()));
        loop.exec();*/
        left += stepsX;
        right -= stepsX;
        up += stepsY;
        down -= stepsY;
        if(right - left <= (double)LOGIN_WINDOW_SIZE_X)
            break;
        this->setGeometry(left, up, right - left, down - up);

        radius += 20.0 / (WINDOW_DIMENSION_CHANGE_SPEED / 2);
        QGraphicsBlurEffect *blurEffectCentralWidget = new QGraphicsBlurEffect();
        blurEffectCentralWidget->setBlurRadius(radius);
        ui->mainMdiArea->setGraphicsEffect(blurEffectCentralWidget);

        QGraphicsBlurEffect *blurEffectLabel = new QGraphicsBlurEffect();
        blurEffectLabel->setBlurRadius(radius);
        ui->label->setGraphicsEffect(blurEffectLabel);

        QApplication::processEvents();
    }
    this->setGeometry(left, up, LOGIN_WINDOW_SIZE_X, LOGIN_WINDOW_SIZE_Y);
    QGraphicsBlurEffect *blurEffectCentralWidget = new QGraphicsBlurEffect();
    blurEffectCentralWidget->setBlurRadius(0);
    ui->mainMdiArea->setGraphicsEffect(blurEffectCentralWidget);
    ui->closePushButton->setVisible(true);
    ui->minimisePushButton->setVisible(true);
    ui->languageComboBox->setVisible(true);

    loginPage->setVisible(true);
    //timerLoopShowLoginWindow.start();
#endif
}

void MainWindow::appMain() {
    if(loginPage)
        loginPage->run();
    if(loginCommand == login::CMD_NONE) {
        if(loginPage)
            loginCommand = loginPage->getCommand();
        if(loginCommand == login::CMD_LOGIN_WALLET) {
            mainboardWindow.run();
            mainboardWindow.setState(mainboard::STATE_DASHBOARD);
        }
    }
    if(loginCommand == login::CMD_LOGIN_WALLET) {
        mainboardWindow.run();
        tickedUpdates.run();
    }
/*
 *  There is an issue moving window from one screen to another with different resolutions, this partially fix the window size.
 *  There remain a flicker when transitioning from one window to another until the mouse is released.
 */
    if(this->width() != s(WINDOW_WIDTH) || this->height() != s(WINDOW_HEIGHT)) {
        this->resize(s(WINDOW_WIDTH), s(WINDOW_HEIGHT) + s(TITLEBAR_HEIGHT));
        this->setMinimumSize(s(WINDOW_WIDTH), s(WINDOW_HEIGHT) + s(TITLEBAR_HEIGHT));
        this->setMaximumSize(s(WINDOW_WIDTH), s(WINDOW_HEIGHT) + s(TITLEBAR_HEIGHT));
    }
    if(windowScale != events::getScale()) {
        windowScale = events::getScale();
        qDebug() << "Scale := " << windowScale;
        this->resize(s(WINDOW_WIDTH), s(WINDOW_HEIGHT) + s(TITLEBAR_HEIGHT));
        this->setMinimumSize(s(WINDOW_WIDTH), s(WINDOW_HEIGHT) + s(TITLEBAR_HEIGHT));
        this->setMaximumSize(s(WINDOW_WIDTH), s(WINDOW_HEIGHT) + s(TITLEBAR_HEIGHT));
        ui->mainMdiArea->setGeometry(0, s(TITLEBAR_HEIGHT), s(WINDOW_WIDTH), s(WINDOW_HEIGHT));
        ui->closePushButton->setGeometry(this->width() - s(41), s(ui->closePushButton->y()), s(41), s(TITLEBAR_HEIGHT));
        ui->minimisePushButton->setGeometry(this->width() - s(82), s(ui->minimisePushButton->y()), s(41), s(TITLEBAR_HEIGHT));
        this->update();
    }
    if(pastNetworkConnection != events::getNetworkConnected()) {
        pastNetworkConnection = events::getNetworkConnected();
        if(pastNetworkConnection) {
            wallethistory::updateWallets();
            populate::refreshAll();
        }
    }
    int tmp = QApplication::desktop()->screenNumber(this);
    events::setScreenNumber(tmp);
    QList<QScreen *> screens = QGuiApplication::screens();
    double osWindowScale = 0;
#ifdef Q_OS_OSX
    osWindowScale = ((screens[tmp]->logicalDotsPerInch() / 72) * ((double)screens[tmp]->geometry().height() / (double)WINDOW_HEIGHT / 2) / FONT_REDUCTION);
#elif defined (Q_OS_WIN32)
    osWindowScale = ((screens[tmp]->logicalDotsPerInch() / 96) * ((double)screens[tmp]->geometry().height() / (double)WINDOW_HEIGHT / 2) / FONT_REDUCTION);
#else
    osWindowScale = ((screens[tmp]->logicalDotsPerInch() / 96) * ((double)screens[tmp]->geometry().height() / (double)WINDOW_HEIGHT / 2) / FONT_REDUCTION);
#endif
    if(screens[tmp] != nullptr && events::getOsWindowScale() != osWindowScale) {
        events::setOsWindowScale(osWindowScale);
        qDebug() << "osWindowScale" << osWindowScale;
        if(screenAt > tmp) {
            QCursor::setPos(QPoint(QCursor::pos().x() - (screens[tmp]->logicalDotsPerInch() * 2), QCursor::pos().y()));
            cursorPressPos.setX(cursorPressPos.x() + (screens[tmp]->logicalDotsPerInch() * 2));
        }
        if(screenAt < tmp) {
            QCursor::setPos(QPoint(QCursor::pos().x() + (screens[screenAt]->logicalDotsPerInch() * 2), QCursor::pos().y()));
            cursorPressPos.setX(cursorPressPos.x() - (screens[screenAt]->logicalDotsPerInch() * 2));
        }
    }
    screenAt = tmp;
}

void MainWindow::appStart() {
    timerAppStart.stop();
    if(loginPage)
        loginPage->setState(login::state_e::STATE_LOGIN_PAGE);
    this->setWindowOpacity(1.0);
}

/*void MainWindow::logicalDotsPerInchChanged(qreal dpi) {
    //qDebug() << dpi;
}*/

