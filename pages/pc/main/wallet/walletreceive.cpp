#include "walletreceive.h"

#include <QClipboard>
#include <QApplication>
#include <QHeaderView>
#include <QMouseEvent>
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

walletreceive::walletreceive() {

}

walletreceive::~walletreceive() {
    parent->setVisible(false);
}

void walletreceive::setVars(QMdiArea *mdiArea, QMdiSubWindow *parentWindow) {
    parent = parentWindow;
    this->mdiArea = mdiArea;

    myWalletLabel = new QLabel(mdiArea);

    walletBalanceLabel = new QLabel(mdiArea);
    walletBalanceValueLabel = new QLabel(mdiArea);
    walletBalanceValueTotalLabel = new QLabel(mdiArea);

    unreceivedBalanceLabel = new QLabel(mdiArea);
    unreceivedBalanceValueLabel = new QLabel(mdiArea);
    unreceivedBalanceValueTotalLabel = new QLabel(mdiArea);

    syncButton = new QPushButton(mdiArea);

    totalLyrReceivedLabel = new QLabel(mdiArea);
    totalLyrReceivedValueLabel = new QLabel(mdiArea);
    totalLyrReceivedLastWeekLabel = new QLabel(mdiArea);

    lastReceivedLabel = new QLabel(mdiArea);
    lastReceivedDateLabel = new QLabel(mdiArea);
    lastReceivedValueLabel = new QLabel(mdiArea);

    walletAddressLabel = new QLabel(mdiArea);
    walletAddressIdLabel = new QLabel(mdiArea);

    requestToWalletIdLabel = new QLabel(mdiArea);
    requestToWalletIdLineEdit = new QLineEdit(mdiArea);
    requestToWalletIdStatusLabel = new QLabel(mdiArea);

    ammountLabel = new QLabel(mdiArea);
    tokenComboBox = new QComboBox(mdiArea); //
    //lyrTextLabel = new QLabel(mdiArea);
    ammountValueStatusLabel = new QLabel(mdiArea); //
    ammountApproximateLabel = new QLabel(mdiArea);
    ammountLineEdit = new QLineEdit(mdiArea);
    btcUsdTextLabel = new QLabel(mdiArea);
    btcUsdLineEdit = new QLineEdit(mdiArea);
    btcUsdStatusLabel = new QLabel(mdiArea);//

    qrCodeLabel = new QLabel(mdiArea);
    qrCodeImageLabel = new QLabel(mdiArea);
    requestButton = new QPushButton(mdiArea);

    btcUsdLabel = new QLabel(mdiArea);
    btcUsdSwitchLabel = new QLabel(mdiArea);

    lastReceivedTransitionsArrowLabel = new QLabel(mdiArea);
    lastReceivedTransitionsLabel = new QLabel(mdiArea);

    recentTransactionsTableView = new QTableView(mdiArea);
    recentTransactionsItemModel = new QStandardItemModel();

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

    totalLyrReceivedLabel->setStyleSheet("color: #aaa;");
    totalLyrReceivedLabel->setAlignment(Qt::AlignLeft);
    totalLyrReceivedLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalLyrReceivedValueLabel->setStyleSheet("color: #333;");
    totalLyrReceivedValueLabel->setAlignment(Qt::AlignLeft);
    totalLyrReceivedValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalLyrReceivedLastWeekLabel->setStyleSheet("color: #50E3C2;");
    totalLyrReceivedLastWeekLabel->setAlignment(Qt::AlignLeft);
    totalLyrReceivedLastWeekLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    lastReceivedLabel->setStyleSheet("color: #aaa;");
    lastReceivedLabel->setAlignment(Qt::AlignLeft);
    lastReceivedLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    lastReceivedDateLabel->setStyleSheet("color: #333;");
    lastReceivedDateLabel->setAlignment(Qt::AlignLeft);
    lastReceivedDateLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    lastReceivedValueLabel->setStyleSheet("color: #50E3C2;");
    lastReceivedValueLabel->setAlignment(Qt::AlignLeft);
    lastReceivedValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    walletAddressLabel->setStyleSheet("color: #333;");
    walletAddressLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    walletAddressLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    walletAddressLabel->installEventFilter(this);

    walletAddressIdLabel->setStyleSheet("color: #A0B9CE;");
    walletAddressIdLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    walletAddressIdLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    walletAddressIdLabel->setCursor(Qt::PointingHandCursor);
    walletAddressIdLabel->installEventFilter(this);

    requestToWalletIdLabel->setStyleSheet("color: #333;");
    requestToWalletIdLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    requestToWalletIdLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    requestToWalletIdLineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    connect(requestToWalletIdLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_requestToWalletIdStatusLabel_Changed(const QString &)));
    requestToWalletIdLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #A0B9CE;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");

    requestToWalletIdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    requestToWalletIdStatusLabel->setScaledContents(true);


    ammountLabel->setStyleSheet("color: #333;");
    ammountLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ammountLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    tokenComboBox->setAutoFillBackground(false);
    tokenComboBox->setStyleSheet(""
        "QComboBox {   "
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: 3px;"
               "padding: 1px 18px 1px 3px;"
               "text-align: center;"
               ";}"
        "QComboBox::drop-down {border-width: 1px;} "
        "QComboBox::down-arrow {image: url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/walletComboBoxArrow.png);}"
        "QComboBox QAbstractItemView {"
               "border: 2px solid darkgray;"
               "color: #aaa;"
               "padding: 1px 1px 1px 1px;"
               "selection-background-color: darkgray;"
               "}"
    );
    tokenComboBox->setCursor(Qt::PointingHandCursor);
    connect(tokenComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_destinationWalletIdLineEdit_Changed(const QString &)));


    /*lyrTextLabel.setStyleSheet("color: #333;");
    lyrTextLabel.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    lyrTextLabel.setAttribute(Qt::WA_TranslucentBackground, true);*/

    ammountValueStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    ammountValueStatusLabel->setScaledContents(true);

    ammountLineEdit->setPlaceholderText("0.00000000");
    ammountLineEdit->setAlignment(Qt::AlignRight);
    ammountLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #A0B9CE;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");
    connect(ammountLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_ammountLineEdit_Changed(const QString &)));

    btcUsdTextLabel->setStyleSheet("color: #333;");
    btcUsdTextLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    btcUsdTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    ammountApproximateLabel->setStyleSheet("color: #333;");
    ammountApproximateLabel->setAlignment(Qt::AlignCenter);
    ammountApproximateLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    btcUsdLineEdit->setPlaceholderText("0.00000000");
    btcUsdLineEdit->setAlignment(Qt::AlignRight);
    btcUsdLineEdit->setEnabled(false);
    btcUsdLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #A0B9CE;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");
    connect(btcUsdLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_ammountUsdBtcLineEdit_Changed(const QString &)));

    btcUsdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    btcUsdStatusLabel->setScaledContents(true);


    btcUsdLabel->setStyleSheet("color: #333;");
    btcUsdLabel->setAlignment(Qt::AlignCenter);
    btcUsdLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    btcUsdSwitchLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    btcUsdSwitchLabel->setCursor(Qt::PointingHandCursor);
    btcUsdSwitchLabel->setScaledContents(true);
    btcUsdSwitchLabel->installEventFilter(this);
    QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchRight.png");
    btcUsdSwitchLabel->setPixmap(image2);

    qrCodeLabel->setStyleSheet("color: #333;");
    qrCodeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    qrCodeLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    qrCodeImageLabel->setCursor(Qt::PointingHandCursor);
    qrCodeImageLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/receive/qr.png); border-radius: 3px;");
    qrCodeImageLabel->setScaledContents(true);
    qrCodeImageLabel->installEventFilter(this);

    requestButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/receive/request.png); border-radius: 2px; border: 1px solid #eee; color: #fff; ");
    requestButton->setFlat(true);
    requestButton->setCursor(Qt::PointingHandCursor);
    requestButton->setVisible(false);

    lastReceivedTransitionsArrowLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/receive/arrow.png); border: 0px solid #eee; color: #fff; ");
    lastReceivedTransitionsArrowLabel->setScaledContents(true);

    lastReceivedTransitionsLabel->setStyleSheet("color: #333;");
    lastReceivedTransitionsLabel->setAlignment(Qt::AlignLeft);
    lastReceivedTransitionsLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    recentTransactionsItemModel->setColumnCount(6);
    recentTransactionsItemModel->setRowCount(0);

    recentTransactionsItemModel->setHeaderData(0, Qt::Horizontal, _tr("Date / Hours"));
    recentTransactionsItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(1, Qt::Horizontal, _tr("Source"));
    recentTransactionsItemModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(2, Qt::Horizontal, _tr("Amount"));
    recentTransactionsItemModel->setHeaderData(2, Qt::Horizontal, Qt::AlignRight, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(3, Qt::Horizontal, _tr("Transaction"));
    recentTransactionsItemModel->setHeaderData(3, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(4, Qt::Horizontal, _tr("Status"));
    recentTransactionsItemModel->setHeaderData(4, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(5, Qt::Horizontal, _tr("Note"));
    recentTransactionsItemModel->setHeaderData(5, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    recentTransactionsTableView->setStyleSheet(
                                   "QTableView { border: none;"
                                   "background-color: transparent;"
                                   "selection-background-color: transparent}");
    recentTransactionsTableView->setShowGrid(false);
    recentTransactionsTableView->verticalHeader()->setVisible(false);
    recentTransactionsTableView->horizontalHeader()->setSectionsClickable(false);
    recentTransactionsTableView->horizontalHeader()->setStyleSheet("color: #777");
    recentTransactionsTableView->horizontalHeader()->setEnabled(false);
    recentTransactionsTableView->setAlternatingRowColors(true);
    recentTransactionsTableView->setModel(recentTransactionsItemModel);

    QStandardItem *tmp;
    for(int cnt = 0;  cnt < 6; cnt++) {
        QList<QStandardItem *> item = QList<QStandardItem *>();

        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);

        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);

        tmp= new QStandardItem();
        tmp->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);

        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignCenter);
        tmp->setForeground(QBrush(0xFFFFFF));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);

        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignCenter);
        tmp->setForeground(QBrush(0xF0F0F0));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);

        tmp = new QStandardItem();
        tmp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);

        recentTransactionsItemModel->appendRow(item);
    }
    QPushButton *viewDetails;
    QLabel *statusDetails;
    for(int cnt = 0;  cnt < 6; cnt++) {
        viewDetails = new QPushButton();
        viewDetails->setText(_tr("View details"));
        viewDetails->setCursor(Qt::PointingHandCursor);
        viewDetails->setStyleSheet(
                    "background-color: transparent;"
                    "color: #0093EE"
                    );
        viewDetails->setFlat(true);
        viewDetails->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        statusDetails = new QLabel();
        switch(cnt) {
        case 0:
            connect(viewDetails , SIGNAL(clicked()) , this , SLOT(on_viewDetails0_PushButton()));
            break;
        case 1:
            connect(viewDetails , SIGNAL(clicked()) , this , SLOT(on_viewDetails1_PushButton()));
            break;
        case 2:
            connect(viewDetails , SIGNAL(clicked()) , this , SLOT(on_viewDetails2_PushButton()));
            break;
        case 3:
            connect(viewDetails , SIGNAL(clicked()) , this , SLOT(on_viewDetails3_PushButton()));
            break;
        case 4:
            connect(viewDetails , SIGNAL(clicked()) , this , SLOT(on_viewDetails4_PushButton()));
            break;
        case 5:
            connect(viewDetails , SIGNAL(clicked()) , this , SLOT(on_viewDetails5_PushButton()));
            break;
        }
        recentTransactionsTableView->setIndexWidget(recentTransactionsItemModel->index(cnt, 3), viewDetails);
        recentTransactionsTableView->setIndexWidget(recentTransactionsItemModel->index(cnt, 4), statusDetails);
    }
    refreshSize();
    refreshLanguage();

    showDetailsWindow = new walletshowdetails();
    showDetailsWindow->init(mdiArea);

    refreshTotalReceivedLastWeekTimer.setInterval(5000);
    connect(&refreshTotalReceivedLastWeekTimer, SIGNAL(timeout()), this, SLOT(on_RefreshTotalReceivedLastWeekTimer()));
    refreshTotalReceivedLastWeekTimer.start();
}

void walletreceive::refreshFonts() {
    myWalletLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));

    walletBalanceLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    walletBalanceValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    walletBalanceValueTotalLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    unreceivedBalanceLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    unreceivedBalanceValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    unreceivedBalanceValueTotalLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    syncButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));

    totalLyrReceivedLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    totalLyrReceivedValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    totalLyrReceivedLastWeekLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    lastReceivedLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    lastReceivedDateLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    lastReceivedValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    walletAddressLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    walletAddressIdLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));

    requestToWalletIdLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    requestToWalletIdLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    requestToWalletIdStatusLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));

    ammountLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    tokenComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.0)));
    //lyrTextLabel.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    ammountLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));
    ammountValueStatusLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    btcUsdTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));
    ammountApproximateLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));
    btcUsdLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    btcUsdStatusLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    btcUsdLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));

    qrCodeLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));

    requestButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));

    lastReceivedTransitionsLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));


    recentTransactionsTableView->horizontalHeader()->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    QPushButton *viewDetails;
    QLabel *statusDetails;
    QStandardItem *tmp;
    for(int cnt = 0;  cnt < recentTransactionsTableView->verticalHeader()->count(); cnt++) {
        viewDetails = (QPushButton *)recentTransactionsTableView->indexWidget(recentTransactionsItemModel->index(cnt, 3));
        viewDetails->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        statusDetails = (QLabel *)recentTransactionsTableView->indexWidget(recentTransactionsItemModel->index(cnt, 4));
        statusDetails->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        statusDetails->setAlignment(Qt::AlignCenter);

        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 0));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 1));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 2));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 3));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 4));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 5));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    }
}

void walletreceive::refreshSize() {
    myWalletLabel->setGeometry(s(28), s(15), s(370), s(30));

    walletBalanceLabel->setGeometry(s(57), s(69), s(370), s(20));
    walletBalanceValueLabel->setGeometry(s(57), s(88), s(370), s(25));
    walletBalanceValueTotalLabel->setGeometry(s(57), s(119), s(370), s(20));

    unreceivedBalanceLabel->setGeometry(s(330), s(69), s(370), s(20));
    unreceivedBalanceValueLabel->setGeometry(s(330), s(88), s(370), s(25));
    unreceivedBalanceValueTotalLabel->setGeometry(s(330), s(119), s(370), s(20));

    syncButton->setGeometry(s(500), s(88), s(48), s(27));

    totalLyrReceivedLabel->setGeometry(s(604), s(69), s(370), s(20));
    totalLyrReceivedValueLabel->setGeometry(s(604), s(88), s(370), s(25));
    totalLyrReceivedLastWeekLabel->setGeometry(s(604), s(119), s(370), s(20));

    lastReceivedLabel->setGeometry(s(877), s(69), s(370), s(20));
    lastReceivedDateLabel->setGeometry(s(877), s(88), s(370), s(25));
    lastReceivedValueLabel->setGeometry(s(877), s(119), s(370), s(20));

    walletAddressLabel->setGeometry(s(0), s(165), s(200), s(30));
    walletAddressIdLabel->setGeometry(s(220), s(164), s(870), s(30));

    requestToWalletIdLabel->setGeometry(s(0), s(235), s(200), s(33));
    requestToWalletIdLineEdit->setGeometry(s(215), s(232), s(850), s(39));
    requestToWalletIdStatusLabel->setGeometry(s(1070), s(242), s(20), s(20));

    ammountLabel->setGeometry(s(0), s(280), s(200), s(39));
    tokenComboBox->setGeometry(s(215), s(280), s(200), s(39));
    ammountLineEdit->setGeometry(s(420), s(280), s(200), s(39));
    ammountValueStatusLabel->setGeometry(s(625), s(290), s(20), s(20));
    ammountApproximateLabel->setGeometry(s(650), s(280), s(39), s(39));
    btcUsdTextLabel->setGeometry(s(690), s(280), s(50), s(39));
    btcUsdLineEdit->setGeometry(s(735), s(280), s(200), s(39));
    btcUsdStatusLabel->setGeometry(s(940), s(290), s(20), s(20));

    btcUsdLabel->setGeometry(s(953), s(288), s(160), s(20));
    btcUsdSwitchLabel->setGeometry(s(1020), s(295), s(24), s(12));

    qrCodeLabel->setGeometry(s(480), s(330), s(200), s(39));
    qrCodeImageLabel->setGeometry(s(688), s(330), s(84), s(84));

    requestButton->setGeometry(s(897), s(379), s(192), s(39));

    lastReceivedTransitionsArrowLabel->setGeometry(s(55), s(470), s(15), s(15));
    lastReceivedTransitionsLabel->setGeometry(s(80), s(470), s(250), s(30));

    recentTransactionsTableView->setGeometry(s(35), s(510), s(1048), s(325));
    recentTransactionsTableView->setColumnWidth(0, s(175));
    recentTransactionsTableView->setColumnWidth(1, s(240));
    recentTransactionsTableView->setColumnWidth(2, s(190));
    recentTransactionsTableView->setColumnWidth(3, s(125));
    recentTransactionsTableView->setColumnWidth(4, s(90));
    recentTransactionsTableView->setColumnWidth(5, s(228));
    for( int cnt = 0; cnt < recentTransactionsTableView->verticalHeader()->count(); cnt++) {
        recentTransactionsTableView->setRowHeight(cnt, s(44));
    }
    refreshFonts();
    /*
     * Due to an issue with the table in QT we nee to repeat the dimension setup.
     */
    recentTransactionsTableView->setGeometry(s(35), s(510), s(1048), s(325));
    recentTransactionsTableView->setColumnWidth(0, s(175));
    recentTransactionsTableView->setColumnWidth(1, s(240));
    recentTransactionsTableView->setColumnWidth(2, s(190));
    recentTransactionsTableView->setColumnWidth(3, s(125));
    recentTransactionsTableView->setColumnWidth(4, s(90));
    recentTransactionsTableView->setColumnWidth(5, s(228));
    for( int cnt = 0; cnt < recentTransactionsTableView->verticalHeader()->count(); cnt++) {
        recentTransactionsTableView->setRowHeight(cnt, s(44));
    }

    refreshFonts();
    recentTransactionsTableView->repaint();
}

void walletreceive::refreshLanguage() {
    myWalletLabel->setText(_tr("My Wallet") +" - "+ _tr("Receive Funds"));

    walletBalanceLabel->setText(_tr("ACCOUNT BALANCE"));

    unreceivedBalanceLabel->setText(_tr("UNRECEIVED BALLANCE"));

    syncButton->setText(_tr("SYNC"));

    totalLyrReceivedLabel->setText(_tr("TOTAL LYRA RECEIVED LAST WEEK"));

    lastReceivedLabel->setText(_tr("LAST RECEIVED TRANSACTION"));

    walletAddressLabel->setText(_tr("Your wallet's address ID") + ":");

    requestToWalletIdLabel->setText(_tr("Request to wallet ID") + ":");

    ammountLabel->setText(_tr("Amount") + ":");

    ammountApproximateLabel->setText("~");

    if(events::getBtcUsdSelect())
        btcUsdTextLabel->setText("USD");
    else
        btcUsdTextLabel->setText("BTC");

    btcUsdLabel->setText("BTC            USD");

    qrCodeLabel->setText(_tr("QR Code") + ":");

    requestButton->setText(_tr("REQUEST"));

    lastReceivedTransitionsLabel->setText(_tr("LAST RECEIVED TRANSACTIONS"));

    unreceivedBalanceValueLabel->setText(_tr(events::getUnreceivedBallance()));

    recentTransactionsItemModel->setHeaderData(0, Qt::Horizontal, _tr("Date / Hours"));
    recentTransactionsItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(1, Qt::Horizontal, _tr("Source"));
    recentTransactionsItemModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(2, Qt::Horizontal, _tr("Amount"));
    recentTransactionsItemModel->setHeaderData(2, Qt::Horizontal, Qt::AlignRight, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(3, Qt::Horizontal, _tr("Transaction"));
    recentTransactionsItemModel->setHeaderData(3, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(4, Qt::Horizontal, _tr("Status"));
    recentTransactionsItemModel->setHeaderData(4, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(5, Qt::Horizontal, _tr("Note"));
    recentTransactionsItemModel->setHeaderData(5, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    QPushButton *viewDetails;
    for(int cnt = 0;  cnt < 6; cnt++) {
        viewDetails = (QPushButton *)recentTransactionsTableView->indexWidget(recentTransactionsItemModel->index(cnt, 3));
        viewDetails->setText(_tr("View details"));
    }
    refreshFonts();
}

void walletreceive::run() {
    /*if(!mdiArea->isVisible())
        showDetailsWindow.setVisible(false);*/
    if(pastScale != events::getScale()) {
        refreshSize();
        pastScale = events::getScale();
    }
    if(pastLanguage.compare(translate::getCurrentLang())) {
        refreshLanguage();
        pastLanguage = translate::getCurrentLang();
        walletBalanceValueTotalLabel->setText(textformating::toPercentage(percentageOfTotalSupply) + " " + _tr("of Total Supply"));
        totalLyrReceivedLastWeekLabel->setText( textformating::toPercentage(totalLyraPercentageReceivedLastWeek) + " " + _tr("of Total Wallet"));
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
        unreceivedBalanceValueTotalLabel->setText(_tr("Last sync") + " " + textformating::toDate(lastSyncDateTime));
    }*/
    if(totalLyraReceivedLastWeek != events::getTotalLyraReceivedLastWeek()) {
        totalLyraReceivedLastWeek = events::getTotalLyraReceivedLastWeek();
        totalLyrReceivedValueLabel->setText(textformating::toValue(totalLyraReceivedLastWeek) + " LYR");
    }
    if(totalLyraPercentageReceivedLastWeek != events::getTotalLyraPercentageReceivedLastWeekOfTotal()) {
        totalLyraPercentageReceivedLastWeek = events::getTotalLyraPercentageReceivedLastWeekOfTotal();
        totalLyrReceivedLastWeekLabel->setText( textformating::toPercentage(totalLyraPercentageReceivedLastWeek) + " " + _tr("of Total Wallet"));
    }
    if(lastReceivedTransactionsListModifyedCnt != events::getLastReceivedTransactionsListModifyedCnt()) {
        lastReceivedTransactionsListModifyedCnt = events::getLastReceivedTransactionsListModifyedCnt();
        QList<QPair<QString, QString>> rxTr = events::getLastReceivedTransactionsList();
        if(rxTr.count() != 0) {
            QPair<QString, QString> tmp = rxTr[0];
            lastReceivedDateLabel->setText(tmp.first);
            lastReceivedValueLabel->setText(tmp.second);
        } else {
            lastReceivedDateLabel->setText("");
            lastReceivedValueLabel->setText("");
        }
    }
    if(recentRxTransactionsModifyedCnt != events::getRecentRxTransactionsModifyedCnt()) {
        recentRxTransactionsModifyedCnt = events::getRecentRxTransactionsModifyedCnt();
        QList<QStringList> list = events::getRecentRxTransactions();
        QStandardItem *tmp;
        QLabel *statusDetails;
        for(int cnt = 0; cnt < 6; cnt++) {
            recentTransactionsTableView->setRowHidden(cnt, true);
            tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 0));
            tmp->setText("");
            tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 1));
            tmp->setText("");
            tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 2));
            tmp->setText("");
            tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 3));
            tmp->setText("");
            tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 4));
            tmp->setText("");
            tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 5));
            tmp->setText("");
        }
        if(list.count() != 0) {
            for(int cnt = 0; cnt < 6; cnt++) {
                if(cnt >= list.count())
                    break;
                statusDetails = (QLabel *)recentTransactionsTableView->indexWidget(recentTransactionsItemModel->index(cnt, 4));
                statusDetails->setText(list[cnt][4]);
                if(!list[cnt][4].compare(_tr("Completed"))) {
                    statusDetails->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/blue.png); border: 0px solid #eee; color: #fff; ");
                }
                recentTransactionsTableView->setRowHidden(cnt, false);
                tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 0));
                tmp->setText(list[cnt][0]);
                tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 1));
                tmp->setText(list[cnt][1]);
                tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 2));
                tmp->setText(list[cnt][2]);
                tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 3));
                tmp->setText(list[cnt][3]);
                tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 4));
                tmp->setText(list[cnt][4]);
                tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 5));
                tmp->setText(list[cnt][5]);
            }
            refreshFonts();
        }
    }
    if(idModifyedCnt != events::getIdModifyedCnt()) {
        idModifyedCnt = events::getIdModifyedCnt();
        walletAddressIdLabel->setText(events::getId());
        showDetailsWindow->setDetailsVisible(false);
        events::setUnreceivedBallance("Please wait");
        run();
        wallet::checkNewTransactions();
    }
    if(tokenListModifyedCnt != events::getTokenListModifyedCnt()) {
        tokenListModifyedCnt = events::getTokenListModifyedCnt();
        QList<QPair<QString, double>> list = events::getTokenList();
        QString tmp = tokenComboBox->currentText();
        tokenComboBox->clear();
        QPair<QString, double> pair;
        foreach(pair, list) {
            tokenComboBox->addItem(pair.first);
        }
        tokenComboBox->setCurrentText(tmp);
    }
    if(usdSelected != events::getBtcUsdSelect()) {
        usdSelected = events::getBtcUsdSelect();
        if(events::getBtcUsdSelect()) {
            QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchRight.png");
            btcUsdSwitchLabel->setPixmap(image2);
            btcUsdTextLabel->setText("USD");
        } else  {
            QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchLeft.png");
            btcUsdSwitchLabel->setPixmap(image2);
            btcUsdTextLabel->setText("BTC");
        }
        double value = (events::getBtcUsdSelect() ? events::getTokenPricePair("LYR_USD") : events::getTokenPricePair("LYR_BTC")) * ammountLineEdit->text().toDouble();
        if(value != 0.0)
            btcUsdLineEdit->setText(textformating::toValue(value, events::getBtcUsdSelect() ? 8 : 8));
        else
            btcUsdLineEdit->setText("");
    }
    showDetailsWindow->run();
}

bool walletreceive::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonRelease) {
        if(mouseEvent->button() == Qt::LeftButton) {
            if (obj == btcUsdSwitchLabel) {
                events::setBtcUsdSelect(!events::getBtcUsdSelect());
                return true;
            } else if(obj == walletAddressIdLabel) {
                QClipboard* clipboard = QApplication::clipboard();
                clipboard->setText(walletAddressIdLabel->text());
            } else if(obj == qrCodeImageLabel) {
                showDetailsWindow->show(walletAddressIdLabel->text());
            }
        }
    }
    return false;
}

void walletreceive::on_requestToWalletIdStatusLabel_Changed(const QString &str) {
    if(signatures::validateAccountId(str)) {
        requestToWalletIdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
    } else {
        requestToWalletIdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    }
}

void walletreceive::on_destinationWalletIdLineEdit_Changed(const QString &/*str*/) {

}

void walletreceive::on_tokenComboBox_TextChanget(const QString &) {

}

void walletreceive::on_ammountLineEdit_Changed(const QString &) {
    if(ammountLineEdit->text().toDouble() != 0.0 && events::getCirculatingSupply() > ammountLineEdit->text().toDouble()) {
        ammountValueStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
        btcUsdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
    } else {
        ammountValueStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
        btcUsdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    }
    double value = (events::getBtcUsdSelect() ? events::getTokenPricePair("LYR_USD") : events::getTokenPricePair("LYR_BTC")) * ammountLineEdit->text().toDouble();
    if(value != 0.0)
        btcUsdLineEdit->setText(textformating::toValue(value, events::getBtcUsdSelect() ? 8 : 8));
    else
        btcUsdLineEdit->setText("");
}

void walletreceive::on_ammountUsdBtcLineEdit_Changed(const QString &) {
    if(btcUsdLineEdit->text().toDouble() != 0.0 && events::getCirculatingSupply() > btcUsdLineEdit->text().toDouble()) {
        ammountValueStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
        btcUsdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
    } else {
        ammountValueStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
        btcUsdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    }
    //ammountLineEdit.setText(btcUsdLineEdit.text());
}

void walletreceive::on_Sync_ButtonPressed() {
    if(userInputSemaphore)
        return;
    userInputSemaphore = true;
    wallet::sync();
    userInputSemaphore = false;
}

void walletreceive::showDetails(int order) {
    QList<QStringList> list = events::getRecentRxTransactions();
    int transactionNr = list[order][6].toInt() - 1;
    int index = events::getSelectedNameKeyIndex();
    QList<QPair<QString, QString>> pair = events::getWalletNameKeyList();
    QList<QList<QMap<QString, QString>>> wallet = wallethistory::getWallet(pair[index].first);
    QList<QMap<QString, QString>> transaction = wallet[transactionNr];
    showDetailsWindow->show(transaction);
}

void walletreceive::on_viewDetails0_PushButton() {
    showDetails(0);
}

void walletreceive::on_viewDetails1_PushButton() {
    showDetails(1);
}

void walletreceive::on_viewDetails2_PushButton() {
    showDetails(2);
}

void walletreceive::on_viewDetails3_PushButton() {
    showDetails(3);
}

void walletreceive::on_viewDetails4_PushButton() {
    showDetails(4);
}

void walletreceive::on_viewDetails5_PushButton() {
    showDetails(5);
}

void walletreceive::on_RefreshTotalReceivedLastWeekTimer() {
    wallet::calculateLastWeek();
}



