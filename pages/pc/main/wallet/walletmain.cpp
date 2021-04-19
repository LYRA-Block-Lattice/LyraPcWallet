#include "walletmain.h"

#include <QHeaderView>
#include <QClipboard>
#include <QApplication>
#include <QThread>

#include "language/translate.h"
#include "wallet/events.h"
#include "pages/pc/textformating.h"
#include "crypto/signatures.h"
#include "wallet/rpc/walletbalance.h"
#include "pages/pc/populate.h"
#include "wallet/rpc/wallethistory.h"
#include "storage/walletfile.h"

#define s(s) _scale(s)

walletmain::walletmain() {

}

walletmain::~walletmain() {

}

void walletmain::setVars(QMdiArea *mdiArea, QMdiSubWindow *parentWindow) {
    parent = parentWindow;

    myWalletLabel = new QLabel(mdiArea);

    walletBalanceLabel = new QLabel(mdiArea);
    walletBalanceValueLabel = new QLabel(mdiArea);
    walletBalanceValueTotalLabel = new QLabel(mdiArea);

    unreceivedBalanceLabel = new QLabel(mdiArea);
    unreceivedBalanceValueLabel = new QLabel(mdiArea);
    unreceivedBalanceValueTotalLabel = new QLabel(mdiArea);

    syncButton = new QPushButton(mdiArea);

    creationDateLabel = new QLabel(mdiArea);
    creationDateDateLabel = new QLabel(mdiArea);
    creationDateHourLabel = new QLabel(mdiArea);

    numberOfTransactionsLabel = new QLabel(mdiArea);
    numberOfTransactionsCountLabel = new QLabel(mdiArea);
    numberOfTransactionsCountWeekLabel = new QLabel(mdiArea);

    walletAddressLabel = new QLabel(mdiArea);
    walletAddressIdLabel = new QLabel(mdiArea);

    startBalanceDateEdit = new QDateEdit(mdiArea);
    endBalanceDateEdit = new QDateEdit(mdiArea);

    startValueDateEdit = new QDateEdit(mdiArea);
    endValueDateEdit = new QDateEdit(mdiArea);

    btcUsdLabel = new QLabel(mdiArea);
    btcUsdSwitchLabel = new QLabel(mdiArea);

    walletBalanceChartLabel = new QLabel(mdiArea);
    walletValueChartLabel = new QLabel(mdiArea);
/********************************************************/
    myWalletLabel->setStyleSheet("color: #555;");
    myWalletLabel->setAlignment(Qt::AlignLeft);
    myWalletLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    walletBalanceLabel->setStyleSheet("color: #aaa;");
    walletBalanceLabel->setAlignment(Qt::AlignLeft);
    walletBalanceLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    walletBalanceValueLabel->setStyleSheet("color: #333;");
    walletBalanceValueLabel->setAlignment(Qt::AlignLeft);
    walletBalanceValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    walletBalanceValueTotalLabel->setStyleSheet("color: #50E3C2;");
    walletBalanceValueTotalLabel->setAlignment(Qt::AlignLeft);
    walletBalanceValueTotalLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    unreceivedBalanceLabel->setStyleSheet("color: #aaa;");
    unreceivedBalanceLabel->setAlignment(Qt::AlignLeft);
    unreceivedBalanceLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    unreceivedBalanceValueLabel->setStyleSheet("color: #333;");
    unreceivedBalanceValueLabel->setAlignment(Qt::AlignLeft);
    unreceivedBalanceValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    unreceivedBalanceValueTotalLabel->setStyleSheet("color: #50E3C2;");
    unreceivedBalanceValueTotalLabel->setAlignment(Qt::AlignLeft);
    unreceivedBalanceValueTotalLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    syncButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/sync.png); border-radius: 1px; color: #eee; ");
    syncButton->setFlat(true);
    syncButton->setCursor(Qt::PointingHandCursor);
    connect(syncButton, SIGNAL(clicked()),this, SLOT(on_Sync_ButtonPressed()));

    creationDateLabel->setStyleSheet("color: #aaa;");
    creationDateLabel->setAlignment(Qt::AlignLeft);
    creationDateLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    creationDateDateLabel->setStyleSheet("color: #333;");
    creationDateDateLabel->setAlignment(Qt::AlignLeft);
    creationDateDateLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    creationDateHourLabel->setStyleSheet("color: #50E3C2;");
    creationDateHourLabel->setAlignment(Qt::AlignLeft);
    creationDateHourLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    numberOfTransactionsLabel->setStyleSheet("color: #aaa;");
    numberOfTransactionsLabel->setAlignment(Qt::AlignLeft);
    numberOfTransactionsLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    numberOfTransactionsCountLabel->setStyleSheet("color: #333;");
    numberOfTransactionsCountLabel->setAlignment(Qt::AlignLeft);
    numberOfTransactionsCountLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    numberOfTransactionsCountWeekLabel->setStyleSheet("color: #50E3C2;");
    numberOfTransactionsCountWeekLabel->setAlignment(Qt::AlignLeft);
    numberOfTransactionsCountWeekLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    walletAddressLabel->setStyleSheet("color: #333;");
    walletAddressLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    walletAddressLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    walletAddressLabel->installEventFilter(this);

    walletAddressIdLabel->setStyleSheet("color: #A0B9CE;");
    walletAddressIdLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    walletAddressIdLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    walletAddressIdLabel->setCursor(Qt::PointingHandCursor);
    walletAddressIdLabel->installEventFilter(this);


    btcUsdLabel->setStyleSheet("color: #333;");
    btcUsdLabel->setAlignment(Qt::AlignCenter);
    btcUsdLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    btcUsdSwitchLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    btcUsdSwitchLabel->setCursor(Qt::PointingHandCursor);
    btcUsdSwitchLabel->setScaledContents(true);
    btcUsdSwitchLabel->installEventFilter(this);
    QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchRight.png");
    btcUsdSwitchLabel->setPixmap(image2);
/********************************************************/
    startBalanceDateEdit->setStyleSheet(""
                           "QDateEdit"
                           "{"
                           "color: #777;"
                           "border : 0px solid white;"
                           "background : transparent;"
                           "padding : 5px;"
                           "}"
                           "QDateEdit::drop-down {"
                           "border-style: solid;"
                           "}"
                           "QDateEdit QAbstractItemView:enabled {"
                           "color: #999; "
                           "background-color: white; "
                           "selection-background-color: #999; "
                           "selection-color: white; "
                           "}"
                           "QDateEdit QWidget#qt_calendar_navigationbar {"
                           "background-color: grey"
                           "}"
                           );
    startBalanceDateEdit->setCursor(Qt::PointingHandCursor);
    startBalanceDateEdit->setCalendarPopup(true);
    startBalanceDateEdit->setDisplayFormat("dd/MM/yyyy");
    startBalanceDateEdit->setDate(QDate::currentDate());
    connect(startBalanceDateEdit, &QDateEdit::editingFinished,this, &walletmain::on_StartDateBalance_EditFinished);
/*------------------------------------------------------*/
    endBalanceDateEdit->setStyleSheet(""
                           "QDateEdit"
                           "{"
                           "color: #777;"
                           "border : 0px solid white;"
                           "background : transparent;"
                           "padding : 5px;"
                           "}"
                           "QDateEdit::drop-down {"
                           "border-style: solid;"
                           "}"
                           "QDateEdit QAbstractItemView:enabled {"
                           "color: #999; "
                           "background-color: white; "
                           "selection-background-color: #999; "
                           "selection-color: white; "
                           "}"
                           "QDateEdit QWidget#qt_calendar_navigationbar {"
                           "background-color: grey"
                           "}"
                           );
    endBalanceDateEdit->setCursor(Qt::PointingHandCursor);
    endBalanceDateEdit->setCalendarPopup(true);
    endBalanceDateEdit->setDisplayFormat("dd/MM/yyyy");
    endBalanceDateEdit->setDate(QDate::currentDate());
    connect(endBalanceDateEdit, &QDateEdit::editingFinished,this, &walletmain::on_EndDateBalance_EditFinished);
/*------------------------------------------------------*/
    startValueDateEdit->setStyleSheet(""
                           "QDateEdit"
                           "{"
                           "color: #777;"
                           "border : 0px solid white;"
                           "background : transparent;"
                           "padding : 5px;"
                           "}"
                           "QDateEdit::drop-down {"
                           "border-style: solid;"
                           "}"
                           "QDateEdit QAbstractItemView:enabled {"
                           "color: #999; "
                           "background-color: white; "
                           "selection-background-color: #999; "
                           "selection-color: white; "
                           "}"
                           "QDateEdit QWidget#qt_calendar_navigationbar {"
                           "background-color: grey"
                           "}"
                           );
    startValueDateEdit->setCursor(Qt::PointingHandCursor);
    startValueDateEdit->setCalendarPopup(true);
    startValueDateEdit->setDisplayFormat("dd/MM/yyyy");
    startValueDateEdit->setDate(QDate::currentDate());
    connect(startValueDateEdit, &QDateEdit::editingFinished,this, &walletmain::on_StartDateValue_EditFinished);
/*------------------------------------------------------*/
    endValueDateEdit->setStyleSheet(""
                           "QDateEdit"
                           "{"
                           "color: #777;"
                           "border : 0px solid white;"
                           "background : transparent;"
                           "padding : 5px;"
                           "}"
                           "QDateEdit::drop-down {"
                           "border-style: solid;"
                           "}"
                           "QDateEdit QAbstractItemView:enabled {"
                           "color: #999; "
                           "background-color: white; "
                           "selection-background-color: #999; "
                           "selection-color: white; "
                           "}"
                           "QDateEdit QWidget#qt_calendar_navigationbar {"
                           "background-color: grey"
                           "}"
                           );
    endValueDateEdit->setCursor(Qt::PointingHandCursor);
    endValueDateEdit->setCalendarPopup(true);
    endValueDateEdit->setDisplayFormat("dd/MM/yyyy");
    endValueDateEdit->setDate(QDate::currentDate());
    connect(endValueDateEdit, &QDateEdit::editingFinished,this, &walletmain::on_EndDateValue_EditFinished);
/*------------------------------------------------------*/
/********************************************************/

    walletBalanceChartLabel->setStyleSheet("color: #333;");
    walletBalanceChartLabel->setAlignment(Qt::AlignLeft);
    walletBalanceChartLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    walletValueChartLabel->setStyleSheet("color: #333;");
    walletValueChartLabel->setAlignment(Qt::AlignLeft);
    walletValueChartLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    myWalletBalanceSeries = new QLineSeries(mdiArea);
    mdiArea->setContentsMargins(0, 0, 0, 0);
    myWalletBalanceChart = new QChart();
    myWalletBalanceChart->legend()->hide();
    myWalletBalanceChart->addSeries(myWalletBalanceSeries);

    myWalletBalanceAxisX = new QDateTimeAxis();
    myWalletBalanceAxisX->setTickCount(5);
    myWalletBalanceAxisX->setFormat("dd/MM/yy");
    myWalletBalanceAxisX->setRange(QDateTime(QDate(2021, 01, 01), QTime(0, 0, 0, 0)), QDateTime(QDate(2022, 01, 01), QTime(0, 0, 0, 0)));
    myWalletBalanceChart->addAxis(myWalletBalanceAxisX, Qt::AlignBottom);
    myWalletBalanceSeries->attachAxis(myWalletBalanceAxisX);

    myWalletBalanceAxisY = new QValueAxis();
    myWalletBalanceAxisY->setLabelFormat("%f");
    myWalletBalanceAxisY->setTickCount(11);
    //myWalletBalanceAxisY->setGridLineVisible(false);
    myWalletBalanceChart->addAxis(myWalletBalanceAxisY, Qt::AlignLeft);
    QList<QtCharts::QAbstractAxis *> abstract = myWalletBalanceChart->axes(Qt::Vertical);
    abstract.first()->setRange(0, 100);
    myWalletBalanceSeries->attachAxis(myWalletBalanceAxisY);

    myWalletBalanceChart->setBackgroundVisible(false);
    myWalletBalanceChartView = new QChartView();
    myWalletBalanceChartView->setChart(myWalletBalanceChart);
    myWalletBalanceChartView->setParent(mdiArea);
    myWalletBalanceChartView->setStyleSheet("background-color: white; ");
    myWalletBalanceChartView->setRenderHint(QPainter::Antialiasing);
    //myWalletBalanceChartView->setAutoFillBackground(false);
    //myWalletBalanceChartView->setWindowFlags(Qt::FramelessWindowHint);
    myWalletBalanceChartView->setAttribute(Qt::WA_NoSystemBackground);
    //myWalletBalanceChartView->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletBalanceChartView->setAttribute(Qt::WA_TransparentForMouseEvents);

    myWalletValueSeries = new QLineSeries(mdiArea);
    mdiArea->setContentsMargins(0, 0, 0, 0);
    myWalletValueChart = new QChart();
    myWalletValueChart->legend()->hide();
    myWalletValueChart->addSeries(myWalletValueSeries);

    myWalletValueAxisX = new QDateTimeAxis();
    myWalletValueAxisX->setTickCount(5);
    myWalletValueAxisX->setFormat("dd/MM/yy");
    myWalletValueAxisX->setRange(QDateTime(QDate(2021, 01, 01), QTime(0, 0, 0, 0)), QDateTime(QDate(2022, 01, 01), QTime(0, 0, 0, 0)));
    myWalletValueChart->addAxis(myWalletValueAxisX, Qt::AlignBottom);
    myWalletValueSeries->attachAxis(myWalletValueAxisX);

    myWalletValueAxisY = new QValueAxis();
    myWalletValueAxisY->setLabelFormat("%f");
    myWalletValueAxisY->setTickCount(11);
    //myWalletValueAxisY->setGridLineVisible(false);
    myWalletValueChart->addAxis(myWalletValueAxisY, Qt::AlignLeft);
    abstract = myWalletValueChart->axes(Qt::Vertical);
    abstract.first()->setRange(0, 100);
    myWalletValueSeries->attachAxis(myWalletValueAxisY);

    myWalletValueChart->setBackgroundVisible(false);
    myWalletValueChartView = new QChartView();
    myWalletValueChartView->setChart(myWalletValueChart);
    myWalletValueChartView->setParent(mdiArea);
    myWalletValueChartView->setStyleSheet("background-color: white; ");
    myWalletValueChartView->setRenderHint(QPainter::Antialiasing);
    //chartView->setAutoFillBackground(false);
    //chartView->setWindowFlags(Qt::FramelessWindowHint);
    myWalletValueChartView->setAttribute(Qt::WA_NoSystemBackground);
    //chartView->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletValueChartView->setAttribute(Qt::WA_TransparentForMouseEvents);

    refreshSize();
    refreshLanguage();
}

void walletmain::refreshFonts() {
    myWalletLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));

    walletBalanceLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    walletBalanceValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    walletBalanceValueTotalLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    unreceivedBalanceLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    unreceivedBalanceValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    unreceivedBalanceValueTotalLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    syncButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));

    creationDateLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    creationDateDateLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    creationDateHourLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    numberOfTransactionsLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    numberOfTransactionsCountLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    numberOfTransactionsCountWeekLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    walletAddressLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    walletAddressIdLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));

    btcUsdLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));

    walletBalanceChartLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    walletValueChartLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));

    startBalanceDateEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));
    endBalanceDateEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));

    startValueDateEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));
    endValueDateEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));

    myWalletBalanceAxisX->setLabelsFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.5)));
    myWalletBalanceAxisX->setTitleFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight()));
    myWalletBalanceAxisY->setLabelsFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.5)));
    myWalletBalanceAxisY->setTitleFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight()));

    myWalletValueAxisX->setLabelsFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.5)));
    myWalletValueAxisX->setTitleFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight()));
    myWalletValueAxisY->setLabelsFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.5)));
    myWalletValueAxisY->setTitleFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight()));
}

void walletmain::refreshSize() {
    myWalletLabel->setGeometry(s(28), s(15), s(370), s(30));

    walletBalanceLabel->setGeometry(s(57), s(69), s(370), s(20));
    walletBalanceValueLabel->setGeometry(s(57), s(88), s(370), s(25));
    walletBalanceValueTotalLabel->setGeometry(s(57), s(119), s(370), s(20));

    unreceivedBalanceLabel->setGeometry(s(330), s(69), s(370), s(20));
    unreceivedBalanceValueLabel->setGeometry(s(330), s(88), s(370), s(25));
    unreceivedBalanceValueTotalLabel->setGeometry(s(330), s(119), s(370), s(20));

    syncButton->setGeometry(s(500), s(88), s(48), s(27));

    creationDateLabel->setGeometry(s(604), s(69), s(370), s(20));
    creationDateDateLabel->setGeometry(s(604), s(88), s(370), s(25));
    creationDateHourLabel->setGeometry(s(604), s(119), s(370), s(20));

    numberOfTransactionsLabel->setGeometry(s(877), s(69), s(370), s(20));
    numberOfTransactionsCountLabel->setGeometry(s(877), s(88), s(370), s(25));
    numberOfTransactionsCountWeekLabel->setGeometry(s(877), s(119), s(370), s(20));

    walletAddressLabel->setGeometry(s(0), s(195), s(200), s(30));
    walletAddressIdLabel->setGeometry(s(220), s(193), s(870), s(30));

    startBalanceDateEdit->setGeometry(s(262), s(355), s(127), s(25));
    endBalanceDateEdit->setGeometry(s(407), s(355), s(127), s(25));

    startValueDateEdit->setGeometry(s(805), s(355), s(127), s(25));
    endValueDateEdit->setGeometry(s(950), s(355), s(127), s(25));

    btcUsdLabel->setGeometry(s(880), s(410), s(160), s(20));
    btcUsdSwitchLabel->setGeometry(s(950), s(415), s(24), s(12));

    walletBalanceChartLabel->setGeometry(s(60), s(410), s(220), s(20));
    walletValueChartLabel->setGeometry(s(605), s(410), s(220), s(20));

    myWalletBalanceChartView->setGeometry(s(40) - 35, s(450) - 35, s(520) + 65, s(360) + 65);
    myWalletValueChartView->setGeometry(s(585) - 35, s(450) - 35, s(520) + 65, s(360) + 65);
    refreshFonts();
}

void walletmain::refreshLanguage() {
    walletBalanceValueTotalLabel->setText(textformating::toPercentage(percentageOfTotalSupply) + " " + _tr("of Total Supply"));
    numberOfTransactionsCountWeekLabel->setText(QString::number(nrOfTransactionsLastWeek) + " " + _tr("in last week"));

    myWalletLabel->setText(_tr("My Wallet"));

    walletBalanceLabel->setText(_tr("ACCOUNT BALANCE"));

    unreceivedBalanceLabel->setText(_tr("UNRECEIVED BALLANCE"));

    syncButton->setText(_tr("SYNC"));

    creationDateLabel->setText(_tr("CREATION DATE"));

    numberOfTransactionsLabel->setText(_tr("NUMBER OF TRANSACTIONS"));

    walletAddressLabel->setText(_tr("Your wallet's address ID") + ":");

    btcUsdLabel->setText("BTC            USD");

    walletBalanceChartLabel->setText(_tr("My Account Balance") + " - LYR");
    walletValueChartLabel->setText(_tr("My Account Value") + " - LYR");

    unreceivedBalanceValueLabel->setText(_tr(events::getUnreceivedBallance()));


    refreshFonts();
}

void walletmain::run() {
    if(pastScale != events::getScale()) {
        refreshSize();
        pastScale = events::getScale();
    }
    if(creationDateTime != events::getCreationDateTime() || pastLanguage.compare(translate::getCurrentLang())) {
        creationDateTime = events::getCreationDateTime();
        if(creationDateTime) {
            creationDateDateLabel->setText(textformating::toDate(creationDateTime));
            creationDateHourLabel->setText(textformating::toTime(creationDateTime));
        } else {
            creationDateDateLabel->setText("EMPTY");
            creationDateHourLabel->setText("");
        }
    }
    if(pastLanguage.compare(translate::getCurrentLang())) {
        refreshLanguage();
        pastLanguage = translate::getCurrentLang();
    }
    if(ballance != events::getBallance()) {
        ballance = events::getBallance();
        walletBalanceValueLabel->setText(textformating::toValue(ballance) + " LYR");
    }
    if(percentageOfTotalSupply != events::getPercentageOfTotalSupply()) {
        percentageOfTotalSupply = events::getPercentageOfTotalSupply();
        walletBalanceValueTotalLabel->setText(textformating::toPercentage(percentageOfTotalSupply) + " " + _tr("of Total Supply"));
    }
    if(unreceivedBallance.compare(events::getUnreceivedBallance())) {
        unreceivedBallance = events::getUnreceivedBallance();
        unreceivedBalanceValueLabel->setText(_tr(unreceivedBallance));
    }
    /*if(lastSyncDateTime != events::getLastSyncDateTime()) {
        lastSyncDateTime = events::getLastSyncDateTime();
        unreceivedBalanceValueTotalLabel.setText(_tr("Last sync") + " " + textformating::toDate(lastSyncDateTime));
    }*/
    if(nrOfTransactions != events::getNumberOfTransactions()) {
        nrOfTransactions = events::getNumberOfTransactions();
        numberOfTransactionsCountLabel->setText(QString::number(nrOfTransactions));
    }
    if(nrOfTransactionsLastWeek != events::getNumberOfTransactionsLastWeek()) {
        nrOfTransactionsLastWeek = events::getNumberOfTransactionsLastWeek();
        numberOfTransactionsCountWeekLabel->setText(QString::number(nrOfTransactionsLastWeek) + " " + _tr("in last week"));
    }
    if(minimumDateTime != events::getMinimumDateTime()) {
        minimumDateTime = events::getMinimumDateTime();
        QDateTime date = QDateTime::fromMSecsSinceEpoch(minimumDateTime);
        startBalanceDateEdit->setDate(QDate(date.toString("yyyy").toInt(), date.toString("M").toInt(), date.toString("dd").toInt()));
        startBalanceDateEdit->setMinimumDate(QDate(date.toString("yyyy").toInt(), date.toString("M").toInt(), date.toString("dd").toInt()));
        startBalanceDateEdit->setMaximumDate(QDate::currentDate());
        endBalanceDateEdit->setDate(QDate::currentDate());
        endBalanceDateEdit->setMinimumDate(QDate(date.toString("yyyy").toInt(), date.toString("M").toInt(), date.toString("dd").toInt()));
        endBalanceDateEdit->setMaximumDate(QDate::currentDate());
        startValueDateEdit->setDate(QDate(date.toString("yyyy").toInt(), date.toString("M").toInt(), date.toString("dd").toInt()));
        startValueDateEdit->setMinimumDate(QDate(date.toString("yyyy").toInt(), date.toString("M").toInt(), date.toString("dd").toInt()));
        startValueDateEdit->setMaximumDate(QDate::currentDate());
        endValueDateEdit->setDate(QDate::currentDate());
        endValueDateEdit->setMinimumDate(QDate(date.toString("yyyy").toInt(), date.toString("M").toInt(), date.toString("dd").toInt()));
        endValueDateEdit->setMaximumDate(QDate::currentDate());
        myWalletBalanceAxisX->setRange(QDateTime(QDate(startBalanceDateEdit->date()), QTime(0, 0, 0, 0)), QDateTime(QDate(endBalanceDateEdit->date()), QTime(0, 0, 0, 0)));
        myWalletValueAxisX->setRange(QDateTime(QDate(startValueDateEdit->date()), QTime(0, 0, 0, 0)), QDateTime(QDate(endValueDateEdit->date()), QTime(0, 0, 0, 0)));
        parent->repaint();
    }
    if(dateRangeModifyedCnt != events::getDateRangeModifyedCnt()) {
        dateRangeModifyedCnt = events::getDateRangeModifyedCnt();
        startBalanceDateEdit->setMinimumDate(events::getMinimumDateRange());
        startBalanceDateEdit->setMaximumDate(events::getMaximumDateRange().addDays(1));
        startBalanceDateEdit->setDate(events::getMinimumDateRange());

        endBalanceDateEdit->setMinimumDate(events::getMinimumDateRange());
        endBalanceDateEdit->setMaximumDate(events::getMaximumDateRange().addDays(1));
        endBalanceDateEdit->setDate(events::getMaximumDateRange());

        startValueDateEdit->setMinimumDate(events::getMinimumDateRange());
        startValueDateEdit->setMaximumDate(events::getMaximumDateRange().addDays(1));
        startValueDateEdit->setDate(events::getMinimumDateRange());

        endValueDateEdit->setMinimumDate(events::getMinimumDateRange());
        endValueDateEdit->setMaximumDate(events::getMaximumDateRange().addDays(1));
        endValueDateEdit->setDate(events::getMaximumDateRange());
        myWalletBalanceAxisX->setRange(QDateTime(QDate(startBalanceDateEdit->date()), QTime(0, 0, 0, 0)), QDateTime(QDate(endBalanceDateEdit->date()), QTime(0, 0, 0, 0)).addDays(1));
        myWalletValueAxisX->setRange(QDateTime(QDate(startValueDateEdit->date()), QTime(0, 0, 0, 0)), QDateTime(QDate(endValueDateEdit->date()), QTime(0, 0, 0, 0)).addDays(1));
        parent->repaint();
    }
    if(myWalletBalanceChartModifyedCnt != events::getMyWalletBalanceChartModifyedCnt()) {
        myWalletBalanceChartModifyedCnt = events::getMyWalletBalanceChartModifyedCnt();
        QPair<qint64, double> tmp;
        myWalletBalanceSeries->clear();
        double minimum = std::numeric_limits<double>::max();
        double maximum = std::numeric_limits<double>::min();
        QList<QPair<qint64, double>> list = events::getMyWalletBalanceChart();
        foreach(tmp, list) {
            double value = tmp.second;
            if(value < minimum)
                minimum = value;
            if(value > maximum)
                maximum = value;
            myWalletBalanceSeries->append(tmp.first, value);
        }
        if(list.count()) {
            QDate startDate = QDateTime::fromMSecsSinceEpoch(list[0].first).date();
            QDate endDate = QDateTime::fromMSecsSinceEpoch(list[list.count() - 1].first).date();
            events::setDateRange(startDate, endDate);
        }
        QList<QtCharts::QAbstractAxis *> abstract = myWalletBalanceChart->axes(Qt::Vertical);
        if(list.count() != 0) {
            abstract.first()->setRange(minimum, maximum);
        } else {
            abstract.first()->setRange(0, 1);
            events::setDateRange(QDate::currentDate(), QDate::currentDate());
        }
    }
    if(myWalletValueChartModifyedCnt != events::getMyWalletValueChartModifyedCnt() || usdSelected != events::getBtcUsdSelect()) {
        myWalletValueChartModifyedCnt = events::getMyWalletValueChartModifyedCnt();
        QPair<qint64, double> tmp;
        myWalletValueSeries->clear();
        double minimum = std::numeric_limits<double>::max();
        double maximum = std::numeric_limits<double>::min();
        QList<QPair<qint64, double>> list = events::getMyWalletValueChart();
        foreach(tmp, list) {
            double value = (events::getBtcUsdSelect() ? events::getTokenPricePair("LYR_USD") : events::getTokenPricePair("LYR_BTC")) * tmp.second;
            if(value < minimum)
                minimum = value;
            if(value > maximum)
                maximum = value;
            myWalletValueSeries->append(tmp.first, value);
        }
        if(list.count()) {
            QDate startDate = QDateTime::fromMSecsSinceEpoch(list[0].first).date();
            QDate endDate = QDateTime::fromMSecsSinceEpoch(list[list.count() - 1].first).date();
            events::setDateRange(startDate, endDate);
        }
        QList<QtCharts::QAbstractAxis *> abstract = myWalletValueChart->axes(Qt::Vertical);
        abstract.first()->setRange(minimum, maximum);
        if(list.count() != 0) {
            abstract.first()->setRange(minimum, maximum);
        } else {
            abstract.first()->setRange(0, 1);
            events::setDateRange(QDate::currentDate(), QDate::currentDate());
        }
        parent->repaint();
    }
    if(usdSelected != events::getBtcUsdSelect()) {
        usdSelected = events::getBtcUsdSelect();
        if(events::getBtcUsdSelect()) {
            QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchRight.png");
            btcUsdSwitchLabel->setPixmap(image2);
            walletValueChartLabel->setText(_tr("My Account Value") + " - USD");
        } else  {
            QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchLeft.png");
            btcUsdSwitchLabel->setPixmap(image2);
            walletValueChartLabel->setText(_tr("My Account Value") + " - BTC");
        }
        parent->repaint();
    }
    if(idModifyedCnt != events::getIdModifyedCnt()) {
        idModifyedCnt = events::getIdModifyedCnt();
        walletAddressIdLabel->setText(events::getId());
        events::setUnreceivedBallance("Please wait");
    }
}

bool walletmain::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonRelease) {
        if(mouseEvent->button() == Qt::LeftButton) {
            if (obj == btcUsdSwitchLabel) {
                events::setBtcUsdSelect(!events::getBtcUsdSelect());
                return true;
            } else if(obj == walletAddressIdLabel) {
                QClipboard* clipboard = QApplication::clipboard();
                clipboard->setText(walletAddressIdLabel->text());
            }
        }
    }
    return false;
}

void walletmain::on_StartDateBalance_EditFinished() {
    endBalanceDateEdit->setMinimumDate(startBalanceDateEdit->date().addDays(1));
    QDateTime tmp;
    tmp.setDate(startBalanceDateEdit->date());
    myWalletBalanceAxisX->setMin(tmp);

    endValueDateEdit->setMinimumDate(startBalanceDateEdit->date().addDays(1));
    myWalletValueAxisX->setMin(tmp);

    startValueDateEdit->setDate(startBalanceDateEdit->date());
}

void walletmain::on_EndDateBalance_EditFinished() {
    startBalanceDateEdit->setMaximumDate(endBalanceDateEdit->date().addDays(-1));
    QDateTime tmp;
    tmp.setDate(endBalanceDateEdit->date());
    myWalletBalanceAxisX->setMax(tmp);

    startValueDateEdit->setMaximumDate(endBalanceDateEdit->date().addDays(-1));
    myWalletValueAxisX->setMax(tmp);

    endValueDateEdit->setDate(endBalanceDateEdit->date());
}

void walletmain::on_StartDateValue_EditFinished() {
    endBalanceDateEdit->setMinimumDate(startValueDateEdit->date().addDays(1));
    QDateTime tmp;
    tmp.setDate(startValueDateEdit->date());
    myWalletBalanceAxisX->setMin(tmp);

    endValueDateEdit->setMinimumDate(startValueDateEdit->date().addDays(1));
    myWalletValueAxisX->setMin(tmp);

    startBalanceDateEdit->setDate(startValueDateEdit->date());
}

void walletmain::on_EndDateValue_EditFinished() {
    startBalanceDateEdit->setMaximumDate(endValueDateEdit->date().addDays(-1));
    QDateTime tmp;
    tmp.setDate(endValueDateEdit->date());
    myWalletBalanceAxisX->setMax(tmp);

    startValueDateEdit->setMaximumDate(endValueDateEdit->date().addDays(-1));
    myWalletValueAxisX->setMax(tmp);

    endBalanceDateEdit->setDate(endValueDateEdit->date());
}

void walletmain::on_Sync_ButtonPressed() {
    if(userInputSemaphore)
        return;
    userInputSemaphore = true;
    wallet::sync();
    userInputSemaphore = false;
}


