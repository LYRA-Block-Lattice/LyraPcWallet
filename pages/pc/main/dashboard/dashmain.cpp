#include "dashmain.h"

#include <QClipboard>
#include <QApplication>
#include <QHeaderView>
#include <limits>

#include "language/translate.h"
#include "wallet/events.h"
#include "pages/pc/textformating.h"
#include "pages/pc/populate.h"

#include "pages/pc/main/mainboard.h"

#define s(s) _scale(s)

dashmain::dashmain(QMdiArea *mdiArea, QMdiSubWindow *parentWindow) : QMainWindow() {
    parent = parentWindow;
    titleLabel = new QLabel(mdiArea);

    totalSupplyLabel = new QLabel(mdiArea);
    totalSupplyValueLabel = new QLabel(mdiArea);

    teamLockedResetvedLabel = new QLabel(mdiArea);
    teamLockedResetvedValueLabel = new QLabel(mdiArea);
    teamLockedResetvedValueTotalLabel = new QLabel(mdiArea);

    circulatingSupplyLabel = new QLabel(mdiArea);
    circulatingSupplyValueLabel = new QLabel(mdiArea);
    circulatingSupplyValueTotalLabel = new QLabel(mdiArea);

    totalBlockLabel = new QLabel(mdiArea);
    totalBlockCountLabel = new QLabel(mdiArea);
    //totalBlockPercentageLabel = new QLabel(mdiArea);

    myWalletValueLabel = new QLabel(mdiArea);
    btcUsdLabel = new QLabel(mdiArea);
    btcUsdSwitchLabel = new QLabel(mdiArea);
    chooseRangeComboBox = new QComboBox(mdiArea);
    orLabel = new QLabel(mdiArea);
    startLabel = new QLabel(mdiArea);
    startDateEdit = new QDateEdit(mdiArea);
    endLabel = new QLabel(mdiArea);
    endDateEdit = new QDateEdit(mdiArea);

    recentTransactionLabel = new QLabel(mdiArea);
    viewAllTransactionsButton = new QPushButton(mdiArea);

    assetsLabel = new QLabel(mdiArea);
    viewAllAssetsButton = new QPushButton(mdiArea);

    recentTransactionsTableView = new QTableView(mdiArea);
    recentTransactionsItemModel = new QStandardItemModel();
    assetsTableView = new QTableView(mdiArea);
    assetsItemModel = new QStandardItemModel();

    titleLabel->setStyleSheet("color: #555;");
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalSupplyLabel->setStyleSheet("color: #aaa;");
    totalSupplyLabel->setAlignment(Qt::AlignLeft);
    totalSupplyLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalSupplyValueLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    totalSupplyValueLabel->setAlignment(Qt::AlignLeft);
    totalSupplyValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    teamLockedResetvedLabel->setStyleSheet("color: #aaa;");
    teamLockedResetvedLabel->setAlignment(Qt::AlignLeft);
    teamLockedResetvedLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    teamLockedResetvedValueLabel->setStyleSheet("color: #555;");
    teamLockedResetvedValueLabel->setAlignment(Qt::AlignLeft);
    teamLockedResetvedValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    teamLockedResetvedValueTotalLabel->setStyleSheet("color: #50E3C2;");
    teamLockedResetvedValueTotalLabel->setAlignment(Qt::AlignLeft);
    teamLockedResetvedValueTotalLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    circulatingSupplyLabel->setStyleSheet("color: #aaa;");
    circulatingSupplyLabel->setAlignment(Qt::AlignLeft);
    circulatingSupplyLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    circulatingSupplyValueLabel->setStyleSheet("color: #555;");
    circulatingSupplyValueLabel->setAlignment(Qt::AlignLeft);
    circulatingSupplyValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    circulatingSupplyValueTotalLabel->setStyleSheet("color: #50E3C2;");
    circulatingSupplyValueTotalLabel->setAlignment(Qt::AlignLeft);
    circulatingSupplyValueTotalLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalBlockLabel->setStyleSheet("color: #aaa;");
    totalBlockLabel->setAlignment(Qt::AlignLeft);
    totalBlockLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalBlockCountLabel->setStyleSheet("color: #555;");
    totalBlockCountLabel->setAlignment(Qt::AlignLeft);
    totalBlockCountLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    /*totalBlockPercentageLabel->setStyleSheet("color: #50E3C2;");
    totalBlockPercentageLabel->setAlignment(Qt::AlignLeft);
    totalBlockPercentageLabel->setAttribute(Qt::WA_TranslucentBackground, true);*/

    myWalletValueLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    myWalletValueLabel->setAlignment(Qt::AlignLeft);
    myWalletValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletValueLabel->installEventFilter(this);

    btcUsdLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    btcUsdLabel->setAlignment(Qt::AlignCenter);
    btcUsdLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    btcUsdSwitchLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    btcUsdSwitchLabel->setCursor(Qt::PointingHandCursor);
    btcUsdSwitchLabel->setScaledContents(true);
    btcUsdSwitchLabel->installEventFilter(this);
    QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchRight.png");
    btcUsdSwitchLabel->setPixmap(image2);

    chooseRangeComboBox->setCurrentIndex(0);
    chooseRangeComboBox->setAutoFillBackground(false);
    chooseRangeComboBox->setCursor(Qt::PointingHandCursor);
    chooseRangeComboBox->setVisible(false);

    orLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    orLabel->setAlignment(Qt::AlignLeft);
    orLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    orLabel->setVisible(false);

    startLabel->setStyleSheet("color: #999;");
    startLabel->setAlignment(Qt::AlignVCenter);
    startLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    startDateEdit->setAttribute(Qt::WA_NoSystemBackground);
    startDateEdit->setAttribute(Qt::WA_TranslucentBackground, true);

    startDateEdit->setCursor(Qt::PointingHandCursor);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");
    startDateEdit->setDate(QDate::currentDate().addMonths(-1));
    connect(startDateEdit, &QDateEdit::editingFinished,this, &dashmain::on_StartDate_EditFinished);

    endLabel->setStyleSheet("color: #999;");
    endLabel->setAlignment(Qt::AlignVCenter);
    endLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    endDateEdit->setCursor(Qt::PointingHandCursor);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");
    endDateEdit->setDate(QDate::currentDate());
    connect(endDateEdit, &QDateEdit::editingFinished,this, &dashmain::on_EndDate_EditFinished);


    myWalletValueSeries = new QLineSeries();
    myWalletValueSeries->setParent(mdiArea);
    mdiArea->setContentsMargins(0, 0, 0, 0);
    myWalletValueChart = new QChart();
    myWalletValueChart->legend()->hide();
    myWalletValueChart->addSeries(myWalletValueSeries);

    myWalletValueAxisX = new QDateTimeAxis();
    myWalletValueAxisX->setTickCount(13);
    myWalletValueAxisX->setFormat("dd/MM/yy");
    myWalletValueAxisX->setRange(QDateTime(QDate(2021, 01, 01), QTime(0, 0, 0, 0)), QDateTime(QDate(2022, 01, 01), QTime(0, 0, 0, 0)));
    myWalletValueChart->addAxis(myWalletValueAxisX, Qt::AlignBottom);
    myWalletValueSeries->attachAxis(myWalletValueAxisX);

    myWalletValueAxisY = new QValueAxis();
    myWalletValueAxisY->setLabelFormat("%f");
    myWalletValueAxisY->setTickCount(11);
    myWalletValueChart->addAxis(myWalletValueAxisY, Qt::AlignLeft);
    QList<QtCharts::QAbstractAxis *> abstract = myWalletValueChart->axes(Qt::Vertical);
    abstract.first()->setRange(0, 100);
    myWalletValueSeries->attachAxis(myWalletValueAxisY);

    myWalletValueChart->setBackgroundVisible(false);
    myWalletValueChartView = new QChartView();
    myWalletValueChartView->setChart(myWalletValueChart);
    myWalletValueChartView->setParent(mdiArea);
    myWalletValueChartView->setStyleSheet("background-color: white; ");
    myWalletValueChartView->setRenderHint(QPainter::Antialiasing);
    myWalletValueChartView->setAttribute(Qt::WA_NoSystemBackground);
    myWalletValueChartView->setAttribute(Qt::WA_TransparentForMouseEvents);

    recentTransactionLabel->setStyleSheet("color: #777;");
    recentTransactionLabel->setAlignment(Qt::AlignLeft);
    recentTransactionLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    viewAllTransactionsButton->setStyleSheet("border-radius: 3px; color: #777; border: 1px solid #eee; ; ");
    viewAllTransactionsButton->setFlat(true);
    viewAllTransactionsButton->setCursor(Qt::PointingHandCursor);
    //viewAllTransactionsButton->setAlignment(Qt::AlignCenter);
    viewAllTransactionsButton->setAttribute(Qt::WA_TranslucentBackground, true);
    connect(viewAllTransactionsButton, &QPushButton::clicked,this, &dashmain::on_ViewAllTransactions_ButtonPushed);

    assetsLabel->setStyleSheet("color: #777;");
    assetsLabel->setAlignment(Qt::AlignLeft);
    assetsLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    viewAllAssetsButton->setStyleSheet("border-radius: 3px; color: #777; border: 1px solid #eee; ; ");
    viewAllAssetsButton->setFlat(true);
    viewAllAssetsButton->setCursor(Qt::PointingHandCursor);
    //viewAllAssetsButton->setAlignment(Qt::AlignCenter);
    viewAllAssetsButton->setAttribute(Qt::WA_TranslucentBackground, true);
    //myWalletValueLabel->setAlignment(Qt::AlignCenter);
   // myWalletValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    connect(viewAllAssetsButton, &QPushButton::clicked,this, &dashmain::on_ViewAllAssets_ButtonPushed);

    updateLastTransactionsTables();
    updateAssetsTables();
    refreshStyle();
    refreshSize();
    refreshLanguage();
}

dashmain::~dashmain() {

}

void dashmain::refreshFonts() {
    titleLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));

    totalSupplyLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    totalSupplyValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));

    teamLockedResetvedLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    teamLockedResetvedValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    teamLockedResetvedValueTotalLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));

    circulatingSupplyLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    circulatingSupplyValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    circulatingSupplyValueTotalLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));

    totalBlockLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    totalBlockCountLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    //totalBlockPercentageLabel.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));

    myWalletValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    btcUsdLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));
    chooseRangeComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    orLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    startLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));
    startDateEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));
    endLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));
    endDateEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));

    myWalletValueAxisX->setLabelsFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.5)));
    myWalletValueAxisX->setTitleFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight()));
    myWalletValueAxisY->setLabelsFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.5)));
    myWalletValueAxisY->setTitleFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight()));

    recentTransactionLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    viewAllTransactionsButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    assetsLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    viewAllAssetsButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    recentTransactionsTableView->horizontalHeader()->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    recentTransactionsTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    recentTransactionsTableView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    QStandardItem *tmp;
    for(int cnt = 0; cnt < recentTransactionsItemModel->rowCount(); cnt++) {
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 0));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 1));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.6)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 2));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 3));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.6)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 4));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.6)));
    }

    for(int cnt = 0; cnt < assetsItemModel->rowCount(); cnt++) {
        tmp = assetsItemModel->itemFromIndex(assetsItemModel->index(cnt, 0));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.6)));
        tmp = assetsItemModel->itemFromIndex(assetsItemModel->index(cnt, 1));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.6)));
        tmp = assetsItemModel->itemFromIndex(assetsItemModel->index(cnt, 2));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.6)));
    }
}

void dashmain::refreshSize() {
    titleLabel->setGeometry(s(28), s(15), s(370), s(30));

    totalSupplyLabel->setGeometry(s(57), s(69), s(370), s(20));
    totalSupplyValueLabel->setGeometry(s(57), s(88), s(370), s(25));

    teamLockedResetvedLabel->setGeometry(s(330), s(69), s(370), s(20));
    teamLockedResetvedValueLabel->setGeometry(s(330), s(88), s(370), s(25));
    teamLockedResetvedValueTotalLabel->setGeometry(s(330), s(119), s(370), s(20));

    circulatingSupplyLabel->setGeometry(s(604), s(69), s(370), s(20));
    circulatingSupplyValueLabel->setGeometry(s(604), s(88), s(370), s(25));
    circulatingSupplyValueTotalLabel->setGeometry(s(604), s(119), s(370), s(20));

    totalBlockLabel->setGeometry(s(877), s(69), s(370), s(20));
    totalBlockCountLabel->setGeometry(s(877), s(88), s(370), s(25));
    //totalBlockPercentageLabel->setGeometry(s(877), s(119), s(370), s(20));

    myWalletValueLabel->setGeometry(s(59), s(185), s(270), s(20));
    btcUsdLabel->setGeometry(s(345), s(181), s(160), s(20));
    btcUsdSwitchLabel->setGeometry(s(413), s(186), s(24), s(12));
    chooseRangeComboBox->setGeometry(s(570), s(173), s(165), s(40));
    orLabel->setGeometry(s(760), s(186), s(50), s(20));

    startLabel->setGeometry(s(800), s(175), s(50), s(10));
    startDateEdit->setGeometry(s(795), s(183), s(127), s(30));

    endLabel->setGeometry(s(945), s(175), s(50), s(10));
    endDateEdit->setGeometry(s(940), s(183), s(127), s(30));

    myWalletValueChartView->setGeometry(s(40) - 35, s(230) - 35, s(1053) + 65, s(315) + 65);

    recentTransactionLabel->setGeometry(s(57), s(620), s(300), s(20));
    viewAllTransactionsButton->setGeometry(s(577), s(610), s(128), s(37));

    assetsLabel->setGeometry(s(820), s(620), s(100), s(20));
    viewAllAssetsButton->setGeometry(s(943), s(610), s(128), s(37));

    recentTransactionsTableView->setGeometry(s(57), s(660), s(645), s(175));
    recentTransactionsTableView->setColumnWidth(0, s(125));
    recentTransactionsTableView->setColumnWidth(1, s(140));
    recentTransactionsTableView->setColumnWidth(2, s(90));
    recentTransactionsTableView->setColumnWidth(3, s(145));
    recentTransactionsTableView->setColumnWidth(4, s(145));
    QHeaderView* header = recentTransactionsTableView->verticalHeader();
    header->setDefaultSectionSize(s(31));

    assetsTableView->setGeometry(s(783), s(655), s(305), s(175));
    assetsTableView->setColumnWidth(0, s(95));
    assetsTableView->setColumnWidth(1, s(65));
    assetsTableView->setColumnWidth(2, s(125));
    header = assetsTableView->verticalHeader();
    header->setDefaultSectionSize(s(31));
    assetsTableView->repaint();

    refreshStyle();
    refreshFonts();
}

void dashmain::refreshStyle() {
    chooseRangeComboBox->setStyleSheet(""
        "QComboBox {   "
               "combobox-popup: 1;"
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(3)) + "px;"
               "padding: 1px " + QString::number((int)s(18)) + "px 1px " + QString::number((int)s(3)) + "px;"
               "text-align: center;"
               ";}"
        "QComboBox::drop-down {border-width: 1px;} "
        "QComboBox::down-arrow {image: url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/rangeComboBox.png);}"
        "QComboBox QAbstractItemView {"
               "border: 2px solid darkgray;"
               "color: #aaa;"
               "padding: 1px 1px 1px 1px;"
               "selection-background-color: darkgray;"
               "}"
    );
    startDateEdit->setStyleSheet(""
                           "QDateEdit"
                           "{"
                           "color: #777;"
                           "border : 0px solid white;"
                           "background : transparent;"
                           "padding : " + QString::number((int)s(5)) + "px;"
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
    endDateEdit->setStyleSheet(""
                           "QDateEdit"
                           "{"
                           "color: #777;"
                           "border : 0px solid white;"
                           "background : transparent;"
                           "padding : " + QString::number((int)s(5)) + "px;"
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
}

void dashmain::refreshLanguage() {
    titleLabel->setText(_tr("Dashboard"));

    totalSupplyLabel->setText(_tr("TOTAL SUPPLY"));

    teamLockedResetvedLabel->setText(_tr("TEAM/LOCKED/RESERVED"));

    circulatingSupplyLabel->setText(_tr("CIRCULATING SUPPLY"));

    totalBlockLabel->setText(_tr("TOTAL BLOCK COUNT"));

    myWalletValueLabel->setText(_tr("My Wallet Value") + " - LYR");
    btcUsdLabel->setText("BTC            USD");
    chooseRangeComboBox->setPlaceholderText(_tr("Choose range"));
    orLabel->setText(_tr("or"));
    startLabel->setText(_tr("Start"));
    endLabel->setText(_tr("End"));

    recentTransactionLabel->setText(_tr("Recent Transactions"));
    viewAllTransactionsButton->setText(_tr("VIEW ALL"));

    assetsLabel->setText(_tr("Assets"));
    viewAllAssetsButton->setText(_tr("VIEW ALL"));

    refreshFonts();
}

void dashmain::updateLastTransactionsTables() {
    recentTransactionsItemModel->clear();
    recentTransactionsItemModel->setColumnCount(5);
    recentTransactionsItemModel->setRowCount(0);

    recentTransactionsItemModel->setHeaderData(0, Qt::Horizontal, _tr("ACCOUNT"));
    recentTransactionsItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(1, Qt::Horizontal, _tr("DATE TRANSACTION"));
    recentTransactionsItemModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(2, Qt::Horizontal, _tr("TYPE"));
    recentTransactionsItemModel->setHeaderData(2, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(3, Qt::Horizontal, _tr("QUANTITY"));
    recentTransactionsItemModel->setHeaderData(3, Qt::Horizontal, Qt::AlignRight, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(4, Qt::Horizontal, _tr("BALANCES"));
    recentTransactionsItemModel->setHeaderData(4, Qt::Horizontal, Qt::AlignRight, Qt::TextAlignmentRole);

    recentTransactionsTableView->setStyleSheet(
                                   "QTableView { border: none;"
                                   "background-color: transparent;"
                                   "selection-background-color: transparent}");
    recentTransactionsTableView->setShowGrid(false);
    recentTransactionsTableView->verticalHeader()->setVisible(false);
    recentTransactionsTableView->horizontalHeader()->setSectionsClickable(false);
    recentTransactionsTableView->horizontalHeader()->setStyleSheet("color: " COLOR_GREY_MID "; ");
    recentTransactionsTableView->horizontalHeader()->setEnabled(false);
    recentTransactionsTableView->setAlternatingRowColors(true);
    recentTransactionsTableView->setModel(recentTransactionsItemModel);
    recentTransactionsTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    recentTransactionsTableView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);


    QStandardItem *tmp;
    QList<QStringList> list = events::getRecentTransactions();
    for(int cnt = 0; cnt < list.count(); cnt++) {
        QList<QStandardItem *> item = QList<QStandardItem *>();
        recentTransactionsTableView->setRowHidden(cnt, false);
        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEditable(false);
        tmp->setSelectable(false);
        tmp->setText(list[cnt][0]);
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEditable(false);
        tmp->setSelectable(false);
        tmp->setText(list[cnt][1]);
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEditable(false);
        tmp->setSelectable(false);
        tmp->setText(_tr(list[cnt][2]));
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEditable(false);
        tmp->setSelectable(false);
        tmp->setText(list[cnt][3]);
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEditable(false);
        tmp->setSelectable(false);
        tmp->setText(list[cnt][4]);
        item.append(tmp);
        recentTransactionsItemModel->appendRow(item);
    }
    refreshSize();
}

void dashmain::updateAssetsTables() {
    assetsItemModel->clear();
    assetsItemModel->setColumnCount(3);
    assetsItemModel->setRowCount(0);
    refreshSize();

    assetsItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    assetsItemModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    assetsItemModel->setHeaderData(2, Qt::Horizontal, Qt::AlignRight, Qt::TextAlignmentRole);

    assetsTableView->setStyleSheet(
                                   "QTableView { border: none;"
                                   "background-color: transparent;"
                                   "selection-background-color: transparent}");
    assetsTableView->setShowGrid(false);
    assetsTableView->verticalHeader()->setVisible(false);
    assetsTableView->horizontalHeader()->setVisible(false);
    assetsTableView->setAlternatingRowColors(true);
    assetsTableView->setModel(assetsItemModel);
    assetsTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    assetsTableView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    QList<QStringList> list = events::getAssets();
    QStandardItem *tmp;

    QIcon arrowUp(":/resource/ico/" + events::getStyle() + "/mainDashBoard/dashboard/main/arrowUp.png");
    QIcon arrowDown(":/resource/ico/" + events::getStyle() + "/mainDashBoard/dashboard/main/arrowDown.png");
    QIcon arrowNone("");
    /*for(int cnt = 0; cnt < list.count(); cnt++) {
        assetsTableView->setRowHidden(cnt, true);
        tmp = assetsItemModel->itemFromIndex(assetsItemModel->index(cnt, 0));
        tmp->setText("");
        tmp = assetsItemModel->itemFromIndex(assetsItemModel->index(cnt, 1));
        tmp->setIcon(arrowNone);
        tmp->setText("");
        tmp = assetsItemModel->itemFromIndex(assetsItemModel->index(cnt, 2));
        tmp->setText("");
    }*/
    for(int cnt = 0; cnt < list.count(); cnt++) {
        QList<QStandardItem *> item = QList<QStandardItem *>();
        if(cnt >= list.count())
            break;
        assetsTableView->setRowHidden(cnt, false);
        tmp = new QStandardItem();
        tmp->setText(list[cnt][0]);
        item.append(tmp);
        tmp = new QStandardItem();
        if(list[cnt][1].toDouble() == 0.0)
            tmp->setIcon(arrowNone);
        else if(list[cnt][1].toDouble() > 0.0)
            tmp->setIcon(arrowUp);
        else
            tmp->setIcon(arrowDown);
        tmp->setText(list[cnt][1] + " %");
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setText(list[cnt][2]);
        item.append(tmp);
        assetsItemModel->appendRow(item);
    }
    for(int cnt = 0;  cnt < list.count(); cnt++) {
        tmp = assetsItemModel->itemFromIndex(assetsItemModel->index(cnt, 0));
        tmp->setTextAlignment(Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEditable(false);
        tmp->setSelectable(false);
        tmp = assetsItemModel->itemFromIndex(assetsItemModel->index(cnt, 1));
        tmp->setTextAlignment(Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEditable(false);
        tmp->setSelectable(false);
        tmp = assetsItemModel->itemFromIndex(assetsItemModel->index(cnt, 2));
        tmp->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEditable(false);
        tmp->setSelectable(false);
    }
    refreshSize();
}

void dashmain::run() {
    if(pastScale != events::getScale()) {
        pastScale = events::getScale();
        updateAssetsTables();
        updateLastTransactionsTables();
    }
    if(pastLanguage.compare(translate::getCurrentLang())) {
        refreshLanguage();
        populate::refreshAll();
        pastLanguage = translate::getCurrentLang();
        updateAssetsTables();
        updateLastTransactionsTables();
        teamLockedResetvedValueTotalLabel->setText(textformating::toPercentage(events::getTeamLockedPercentageOfTotalSupply()) + " " + _tr("of Total Supply"));
        circulatingSupplyValueTotalLabel->setText(textformating::toPercentage(events::getCirculatingSupplyPercentage()) + " " + _tr("of Total Supply"));
    }
    if(totalSupply != events::getTotalSupply()) {
        totalSupply = events::getTotalSupply();
        totalSupplyValueLabel->setText(textformating::toValue(totalSupply) + " LYR");
    }
    if(teamLockedReserved != events::getTeamLockedReserved()) {
        teamLockedReserved = events::getTeamLockedReserved();
        teamLockedResetvedValueLabel->setText(textformating::toValue(teamLockedReserved) + " LYR");
        teamLockedResetvedValueTotalLabel->setText(textformating::toPercentage(events::getTeamLockedPercentageOfTotalSupply()) + " " + _tr("of Total Supply"));
    }
    if(circulatingSupply != events::getCirculatingSupply()) {
        circulatingSupply = events::getCirculatingSupply();
        circulatingSupplyValueLabel->setText(textformating::toValue(circulatingSupply) + " LYR");
        circulatingSupplyValueTotalLabel->setText(textformating::toPercentage(events::getCirculatingSupplyPercentage()) + " " + _tr("of Total Supply"));
    }
    if(totalBlockCount != events::getTotalBlockCount()) {
        totalBlockCount = events::getTotalBlockCount();
        if(totalBlockCount) {
            totalBlockCountLabel->setText(QString::number(totalBlockCount));
        } else {
            totalBlockCountLabel->setText(_tr("WAIT"));
        }
    }
    if(minimumDateTime != events::getMinimumDateTime()) {
        minimumDateTime = events::getMinimumDateTime();
        QDateTime date = QDateTime::fromMSecsSinceEpoch(minimumDateTime);
        startDateEdit->setDate(QDate(date.toString("yyyy").toInt(), date.toString("M").toInt(), date.toString("dd").toInt()));
        startDateEdit->setMinimumDate(QDate(date.toString("yyyy").toInt(), date.toString("M").toInt(), date.toString("dd").toInt()));
        startDateEdit->setMaximumDate(QDate::currentDate());
        endDateEdit->setDate(QDate::currentDate());
        endDateEdit->setMinimumDate(QDate(date.toString("yyyy").toInt(), date.toString("M").toInt(), date.toString("dd").toInt()));
        endDateEdit->setMaximumDate(QDate::currentDate());
        myWalletValueAxisX->setRange(QDateTime(QDate(startDateEdit->date()), QTime(0, 0, 0, 0)), QDateTime(QDate(endDateEdit->date()), QTime(0, 0, 0, 0)));
    }
    if(dateRangeModifyedCnt != dateRangeModifyedCntN/*events::getDateRangeModifyedCnt()*/) {
        dateRangeModifyedCnt = dateRangeModifyedCntN/*events::getDateRangeModifyedCnt()*/;
        //startDateEdit->setMinimumDate(events::getMinimumDateRange());
        //startDateEdit->setMaximumDate(events::getMaximumDateRange().addDays(1));
        //startDateEdit->setDate(events::getMinimumDateRange());
        //endDateEdit->setMinimumDate(events::getMinimumDateRange());
        //endDateEdit->setMaximumDate(events::getMaximumDateRange().addDays(1));
        //endDateEdit->setDate(events::getMaximumDateRange());

        startDateEdit->setMinimumDate(minimumDateRange);
        startDateEdit->setMaximumDate(maximumDateRange.addDays(1));
        startDateEdit->setDate(minimumDateRange);
        endDateEdit->setMinimumDate(minimumDateRange);
        endDateEdit->setMaximumDate(maximumDateRange.addDays(1));
        endDateEdit->setDate(maximumDateRange);
        myWalletValueAxisX->setRange(QDateTime(QDate(startDateEdit->date()), QTime(0, 0, 0, 0)), QDateTime(QDate(endDateEdit->date()), QTime(0, 0, 0, 0)).addDays(1));
    }
    if(myWalletValueAllChartModifyedCnt != events::getMyAccountValueAllChartModifyedCnt() || usdSelected != events::getBtcUsdSelect()) {
        myWalletValueAllChartModifyedCnt = events::getMyAccountValueAllChartModifyedCnt();
        QPair<qint64, double> tmp;
        myWalletValueSeries->clear();
        double minimum = std::numeric_limits<double>::max();
        double maximum = std::numeric_limits<double>::min();
        QList<QPair<qint64, double>> list = events::getMyAccountValueAllChart();
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
            minimumDateRange = startDate;
            maximumDateRange = endDate;
            dateRangeModifyedCntN++;
            //events::setDateRange(startDate, endDate);
        }
        QList<QtCharts::QAbstractAxis *> abstract = myWalletValueChart->axes(Qt::Vertical);
        if(list.count() != 0) {
            abstract.first()->setRange(minimum, maximum);
        } else {
            abstract.first()->setRange(0, 1);
            minimumDateRange = QDate::currentDate();
            maximumDateRange = QDate::currentDate();
            dateRangeModifyedCntN++;
            //events::setDateRange(QDate::currentDate(), QDate::currentDate());
        }
    }
    if(recentTransactionsListModifyedCnt != events::getRecentTransactionsModifyedCnt()) {
        recentTransactionsListModifyedCnt = events::getRecentTransactionsModifyedCnt();
        updateLastTransactionsTables();
    }
    if(assetsListModifyedCnt != events::getAssetsModifyedCnt()) {
        assetsListModifyedCnt = events::getAssetsModifyedCnt();
        updateAssetsTables();
    }
    if(usdSelected != events::getBtcUsdSelect()) {
        usdSelected = events::getBtcUsdSelect();
        if(events::getBtcUsdSelect()) {
            QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchRight.png");
            btcUsdSwitchLabel->setPixmap(image2);
        } else  {
            QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchLeft.png");
            btcUsdSwitchLabel->setPixmap(image2);
        }
    }
}

bool dashmain::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonRelease) {
        if(mouseEvent->button() == Qt::LeftButton) {
            if (obj == btcUsdSwitchLabel) {
                events::setBtcUsdSelect(!events::getBtcUsdSelect());
                return true;
            } else if(obj == myWalletValueLabel) {
                QClipboard* clipboard = QApplication::clipboard();
                clipboard->setText(myWalletValueLabel->text());
            }
        }
    }
    return false;
}

void dashmain::on_StartDate_EditFinished() {
    endDateEdit->setMinimumDate(startDateEdit->date().addDays(1));
    QDateTime tmp;
    tmp.setDate(startDateEdit->date());
    myWalletValueAxisX->setMin(tmp);
}

void dashmain::on_EndDate_EditFinished() {
    startDateEdit->setMaximumDate(endDateEdit->date().addDays(-1));
    QDateTime tmp;
    tmp.setDate(endDateEdit->date());
    myWalletValueAxisX->setMax(tmp);
}

void dashmain::on_ViewAllTransactions_ButtonPushed() {
    events::setShowTransitionsWindow();
}

void dashmain::on_ViewAllAssets_ButtonPushed() {
    events::setShowTransitionsWindow();
}



