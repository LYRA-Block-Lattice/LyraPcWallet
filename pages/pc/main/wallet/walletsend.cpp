#include "walletsend.h"

#include <QClipboard>
#include <QApplication>
#include <QHeaderView>
#include <QMouseEvent>
#include <QThread>
#include <QMessageBox>
#include <QListView>

#include "language/translate.h"
#include "wallet/events.h"
#include "crypto/signatures.h"
#include "pages/pc/textformating.h"
#include "crypto/signatures.h"
#include "wallet/rpc/walletbalance.h"
#include "pages/pc/populate.h"
#include "wallet/rpc/wallethistory.h"
#include "storage/walletfile.h"
#include "wallet/tickedupdates.h"

#include "configlyra.h"

#define s(s) _scale(s)

walletsend::walletsend() {

}

walletsend::~walletsend() {
    parent->setVisible(false);
}

void walletsend::setVars(QMdiArea *mdiArea, QMdiSubWindow *parentWindow) {
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

    totalLyrSendedLabel = new QLabel(mdiArea);
    totalLyrSendedValueLabel = new QLabel(mdiArea);
    totalLyrSendedLastWeekLabel = new QLabel(mdiArea);

    lastSendLabel = new QLabel(mdiArea);
    lastSendDateLabel = new QLabel(mdiArea);
    lastSendValueLabel = new QLabel(mdiArea);

    walletAddressLabel = new QLabel(mdiArea);
    walletAddressIdLabel = new QLabel(mdiArea);

    destinationWalletIdLabel = new QLabel(mdiArea);
    destinationWalletIdLineEdit = new QLineEdit(mdiArea);
    destinationWalletIdStatusLabel = new QLabel(mdiArea);

    ammountLabel = new QLabel(mdiArea);
    tokenComboBox = new QComboBox(mdiArea);
    ammountLineEdit = new QLineEdit(mdiArea);
    ammountValueStatusLabel = new QLabel(mdiArea);
    ammountApproximateLabel = new QLabel(mdiArea);
    btcUsdTextLabel = new QLabel(mdiArea);
    btcUsdLineEdit = new QLineEdit(mdiArea);
    btcUsdStatusLabel = new QLabel(mdiArea);

    btcUsdLabel = new QLabel(mdiArea);
    btcUsdSwitchLabel = new QLabel(mdiArea);

    //passwordConfirmationLabel = new QLabel(mdiArea);
    //passwordConfirmationLineEdit = new QLineEdit(mdiArea);
    //passwordConfirmationStatusLabel = new QLabel(mdiArea);

    noteLabel = new QLabel(mdiArea);
    noteLineEdit = new QLineEdit(mdiArea);

    sendButton = new QPushButton(mdiArea);

    lastSendedTransitionsArrowLabel = new QLabel(mdiArea);
    lastSendedTransitionsLabel = new QLabel(mdiArea);

    recentTransactionsTableView = new QTableView(mdiArea);
    recentTransactionsItemModel = new QStandardItemModel();

    myWalletLabel->setStyleSheet("color: #555;");
    myWalletLabel->setAlignment(Qt::AlignLeft);
    myWalletLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    walletBalanceLabel->setStyleSheet("color: #aaa;");
    walletBalanceLabel->setAlignment(Qt::AlignLeft);
    walletBalanceLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    walletBalanceValueLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    walletBalanceValueLabel->setAlignment(Qt::AlignLeft);
    walletBalanceValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    walletBalanceValueTotalLabel->setStyleSheet("color: #50E3C2;");
    walletBalanceValueTotalLabel->setAlignment(Qt::AlignLeft);
    walletBalanceValueTotalLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    unreceivedBalanceLabel->setStyleSheet("color: #aaa;");
    unreceivedBalanceLabel->setAlignment(Qt::AlignLeft);
    unreceivedBalanceLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    unreceivedBalanceValueLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    unreceivedBalanceValueLabel->setAlignment(Qt::AlignLeft);
    unreceivedBalanceValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    unreceivedBalanceValueTotalLabel->setStyleSheet("color: #50E3C2;");
    unreceivedBalanceValueTotalLabel->setAlignment(Qt::AlignLeft);
    unreceivedBalanceValueTotalLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    syncButton->setFlat(true);
    syncButton->setCursor(Qt::PointingHandCursor);
    connect(syncButton, SIGNAL(clicked()),this, SLOT(on_Sync_ButtonPressed()));

    totalLyrSendedLabel->setStyleSheet("color: #aaa;");
    totalLyrSendedLabel->setAlignment(Qt::AlignLeft);
    totalLyrSendedLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalLyrSendedValueLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    totalLyrSendedValueLabel->setAlignment(Qt::AlignLeft);
    totalLyrSendedValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalLyrSendedLastWeekLabel->setStyleSheet("color: #50E3C2;");
    totalLyrSendedLastWeekLabel->setAlignment(Qt::AlignLeft);
    totalLyrSendedLastWeekLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    lastSendLabel->setStyleSheet("color: #aaa;");
    lastSendLabel->setAlignment(Qt::AlignLeft);
    lastSendLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    lastSendDateLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    lastSendDateLabel->setAlignment(Qt::AlignLeft);
    lastSendDateLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    lastSendValueLabel->setStyleSheet("color: #50E3C2;");
    lastSendValueLabel->setAlignment(Qt::AlignLeft);
    lastSendValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    walletAddressLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    walletAddressLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    walletAddressLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    walletAddressIdLabel->setStyleSheet("color: #A0B9CE;");
    walletAddressIdLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    walletAddressIdLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    walletAddressIdLabel->setCursor(Qt::PointingHandCursor);
    walletAddressIdLabel->installEventFilter(this);

    destinationWalletIdLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    destinationWalletIdLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    destinationWalletIdLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    destinationWalletIdLineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    connect(destinationWalletIdLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_destinationWalletIdLineEdit_Changed(const QString &)));


    destinationWalletIdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    destinationWalletIdStatusLabel->setScaledContents(true);

    ammountLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    ammountLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ammountLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    tokenComboBox->setAutoFillBackground(false);
    tokenComboBox->setCursor(Qt::PointingHandCursor);
    connect(tokenComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_tokenComboBox_TextChanget(const QString &)));


    ammountLineEdit->setPlaceholderText("0.00000000");
    ammountLineEdit->setAlignment(Qt::AlignRight);
    connect(ammountLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_ammountLineEdit_Changed(const QString &)));

    ammountValueStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    ammountValueStatusLabel->setScaledContents(true);

    ammountApproximateLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    ammountApproximateLabel->setAlignment(Qt::AlignCenter);
    ammountApproximateLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    btcUsdTextLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    btcUsdTextLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    btcUsdTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    btcUsdLineEdit->setPlaceholderText("0.00000000");
    btcUsdLineEdit->setAlignment(Qt::AlignRight);
    btcUsdLineEdit->setEnabled(false);
    connect(btcUsdLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_ammountUsdBtcLineEdit_Changed(const QString &)));

    btcUsdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    btcUsdStatusLabel->setScaledContents(true);


    btcUsdLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    btcUsdLabel->setAlignment(Qt::AlignCenter);
    btcUsdLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    btcUsdSwitchLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    btcUsdSwitchLabel->setCursor(Qt::PointingHandCursor);
    btcUsdSwitchLabel->setScaledContents(true);
    btcUsdSwitchLabel->installEventFilter(this);
    QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/common/switchRight.png");
    btcUsdSwitchLabel->setPixmap(image2);


    /*passwordConfirmationLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    passwordConfirmationLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    passwordConfirmationLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    passwordConfirmationLineEdit->setAlignment(Qt::AlignLeft);
    passwordConfirmationLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #A0B9CE;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");
    passwordConfirmationLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    connect(passwordConfirmationLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_PasswordLineEdit_Changed(const QString &)));

    passwordConfirmationStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
    passwordConfirmationStatusLabel->setScaledContents(true);*/

    noteLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    noteLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    noteLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    noteLineEdit->setAlignment(Qt::AlignLeft);
    sendButton->setFlat(true);
    sendButton->setCursor(Qt::PointingHandCursor);
    connect(sendButton, SIGNAL(clicked()),this, SLOT(on_Send_ButtonPressed()));

    lastSendedTransitionsArrowLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/send/arrow.png); border: 0px solid #eee; color: #fff; ");
    lastSendedTransitionsArrowLabel->setScaledContents(true);

    lastSendedTransitionsLabel->setStyleSheet("color: " COLOR_GREY_DARK ";");
    lastSendedTransitionsLabel->setAlignment(Qt::AlignLeft);
    lastSendedTransitionsLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    refreshStyle();
    updateSend();
    refreshSize();
    refreshLanguage();

    showDetailsWindow = new walletshowdetails();
    showDetailsWindow->init(mdiArea);

    refreshTotalSendedLastWeekTimer.setInterval(5000);
    connect(&refreshTotalSendedLastWeekTimer, SIGNAL(timeout()), this, SLOT(on_TefreshTotalSendedLastWeekTimer()));
    refreshTotalSendedLastWeekTimer.start();
}

void walletsend::refreshFonts() {
    myWalletLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));

    walletBalanceLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    walletBalanceValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    walletBalanceValueTotalLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));

    unreceivedBalanceLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    unreceivedBalanceValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    unreceivedBalanceValueTotalLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));

    syncButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));

    totalLyrSendedLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    totalLyrSendedValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    totalLyrSendedLastWeekLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));

    lastSendLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    lastSendDateLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.1)));
    lastSendValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));

    walletAddressLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    walletAddressIdLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));

    destinationWalletIdLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    destinationWalletIdLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));

    ammountLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    tokenComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    ammountLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    ammountValueStatusLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    ammountApproximateLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));
    btcUsdTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));
    btcUsdLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    btcUsdStatusLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    btcUsdLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));

    //passwordConfirmationLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    //passwordConfirmationLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    //passwordConfirmationStatusLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    noteLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    noteLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    sendButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));

    lastSendedTransitionsLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    recentTransactionsTableView->horizontalHeader()->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    QPushButton *viewDetails;
    QLabel *statusDetails;
    QStandardItem *tmp;
    for(int cnt = 0;  cnt < recentTransactionsItemModel->rowCount(); cnt++) {
        viewDetails = (QPushButton *)recentTransactionsTableView->indexWidget(recentTransactionsItemModel->index(cnt, 3));
        viewDetails->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        statusDetails = (QLabel *)recentTransactionsTableView->indexWidget(recentTransactionsItemModel->index(cnt, 4));
        statusDetails->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        statusDetails->setAlignment(Qt::AlignCenter);

        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 0));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 1));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 2));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.7)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 3));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 4));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        tmp = recentTransactionsItemModel->itemFromIndex(recentTransactionsItemModel->index(cnt, 5));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    }
}

void walletsend::refreshSize() {
    myWalletLabel->setGeometry(s(28), s(15), s(370), s(30));

    walletBalanceLabel->setGeometry(s(57), s(69), s(370), s(20));
    walletBalanceValueLabel->setGeometry(s(57), s(88), s(370), s(25));
    walletBalanceValueTotalLabel->setGeometry(s(57), s(119), s(370), s(20));

    unreceivedBalanceLabel->setGeometry(s(330), s(69), s(370), s(20));
    unreceivedBalanceValueLabel->setGeometry(s(330), s(88), s(170), s(25));
    unreceivedBalanceValueTotalLabel->setGeometry(s(330), s(119), s(370), s(20));

    syncButton->setGeometry(s(490), s(88), s(58), s(27));

    totalLyrSendedLabel->setGeometry(s(604), s(69), s(370), s(20));
    totalLyrSendedValueLabel->setGeometry(s(604), s(88), s(370), s(25));
    totalLyrSendedLastWeekLabel->setGeometry(s(604), s(119), s(370), s(20));

    lastSendLabel->setGeometry(s(877), s(69), s(370), s(20));
    lastSendDateLabel->setGeometry(s(877), s(88), s(370), s(25));
    lastSendValueLabel->setGeometry(s(877), s(119), s(370), s(20));

    walletAddressLabel->setGeometry(s(0), s(168), s(200), s(30));
    walletAddressIdLabel->setGeometry(s(205), s(168), s(870), s(30));

    destinationWalletIdLabel->setGeometry(s(0), s(235), s(200), s(33));
    destinationWalletIdLineEdit->setGeometry(s(215), s(232), s(850), s(39));
    destinationWalletIdStatusLabel->setGeometry(s(1070), s(242), s(20), s(20));

    ammountLabel->setGeometry(s(0), s(280), s(200), s(39));
    tokenComboBox->setGeometry(s(215), s(280), s(200), s(39));
    ammountLineEdit->setGeometry(s(420), s(280), s(200), s(39));
    ammountValueStatusLabel->setGeometry(s(625), s(290), s(20), s(20));
    ammountApproximateLabel->setGeometry(s(650), s(280), s(39), s(39));
    btcUsdTextLabel->setGeometry(s(690), s(280), s(50), s(39));
    btcUsdLineEdit->setGeometry(s(735), s(280), s(200), s(39));
    btcUsdStatusLabel->setGeometry(s(940), s(290), s(20), s(20));

    btcUsdLabel->setGeometry(s(953), s(289), s(160), s(20));
    btcUsdSwitchLabel->setGeometry(s(1020), s(295), s(24), s(12));

    //passwordConfirmationLabel->setGeometry(s(0), s(328), s(200), s(39));
    //passwordConfirmationLineEdit->setGeometry(s(215), s(328), s(850), s(39));
    //passwordConfirmationStatusLabel->setGeometry(s(1070), s(338), s(20), s(20));

    noteLabel->setGeometry(s(0), s(379), s(200), s(39));
    noteLineEdit->setGeometry(s(215), s(379), s(665), s(39));
    sendButton->setGeometry(s(895), s(379), s(194), s(39));

    lastSendedTransitionsArrowLabel->setGeometry(s(55), s(470), s(15), s(15));
    lastSendedTransitionsLabel->setGeometry(s(80), s(470), s(250), s(30));

    recentTransactionsTableView->setGeometry(s(35), s(510), s(1048), s(295));
    recentTransactionsTableView->setColumnWidth(0, s(175));
    recentTransactionsTableView->setColumnWidth(1, s(240));
    recentTransactionsTableView->setColumnWidth(2, s(190));
    recentTransactionsTableView->setColumnWidth(3, s(125));
    recentTransactionsTableView->setColumnWidth(4, s(90));
    recentTransactionsTableView->setColumnWidth(5, s(228));
    QHeaderView* header = recentTransactionsTableView->verticalHeader();
    header->setDefaultSectionSize(s(30));
    refreshFonts();
    refreshStyle();
}
void walletsend::refreshLanguage() {
    myWalletLabel->setText(_tr("My Account") +" - "+ _tr("Send Funds"));

    walletBalanceLabel->setText(_tr("ACCOUNT BALANCE"));

    unreceivedBalanceLabel->setText(_tr("UNRECEIVED BALLANCE"));

    syncButton->setText(_tr("SYNC"));

    totalLyrSendedLabel->setText(_tr("TOTAL LYRA SENT LAST WEEK"));
    totalLyrSendedValueLabel->setText("1,158,010.00 LYR");
    totalLyrSendedLastWeekLabel->setText("12.58 %" + _tr("of Total Wallet"));

    lastSendLabel->setText(_tr("LAST SEND TRANSACTION"));
    lastSendDateLabel->setText("21 " + _tr("January") + " 2021");
    lastSendValueLabel->setText("151,000.00" " LYR");

    walletBalanceValueTotalLabel->setText(textformating::toPercentage(percentageOfTotalSupply) + " " + _tr("of Total Supply"));
    totalLyrSendedLastWeekLabel->setText( textformating::toPercentage(totalLyraPercentageSendedLastWeek) + " " + _tr("of Total Wallet"));

    walletAddressLabel->setText(_tr("Your account address ID") + ":");

    btcUsdLabel->setText("BTC            USD");

    destinationWalletIdLabel->setText(_tr("Destination to account ID") + ":");

    //passwordConfirmationLabel->setText(_tr("Password Confirmation") + ":");

    noteLabel->setText(_tr("Note") + ":");

    sendButton->setText(_tr("SEND"));

    ammountLabel->setText(_tr("Amount") + ":");
    ammountApproximateLabel->setText("~");
    if(events::getBtcUsdSelect())
        btcUsdTextLabel->setText("USD");
    else
        btcUsdTextLabel->setText("BTC");


    lastSendedTransitionsLabel->setText(_tr("LAST SEND TRANSACTIONS"));


    //passwordConfirmationLineEdit->setPlaceholderText(_tr("Password"));
    noteLineEdit->setPlaceholderText(_tr("Note"));

    unreceivedBalanceValueLabel->setText(_tr(events::getUnreceivedBallance()));

    recentTransactionsItemModel->setHeaderData(0, Qt::Horizontal, _tr("Date / Hours"));
    recentTransactionsItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    recentTransactionsItemModel->setHeaderData(1, Qt::Horizontal, _tr("Destination"));
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
    for(int cnt = 0;  cnt < recentTransactionsItemModel->rowCount(); cnt++) {
        viewDetails = (QPushButton *)recentTransactionsTableView->indexWidget(recentTransactionsItemModel->index(cnt, 3));
        viewDetails->setText(_tr("View details"));
    }
    refreshFonts();
}

void walletsend::refreshStyle() {
    syncButton->setStyleSheet("background-color: " BUTON_COLOR_GREEN "; border-radius: " + QString::number((int)s(12)) + "px; color: #eee; ");
    destinationWalletIdLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #A0B9CE;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(5)) + "px;"
                                                    ";}");
    tokenComboBox->setStyleSheet(""
        "QComboBox {   "
               "combobox-popup: 1;"
               "color: " COLOR_GREY_MID ";"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(5)) + "px;"
               "padding: 1px " + QString::number((int)s(18)) + "px 1px " + QString::number((int)s(3)) + "px;"
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
    ammountLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #A0B9CE;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(5)) + "px;"
                                                    ";}");
    btcUsdLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #A0B9CE;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(5)) + "px;"
                                                    ";}");
    noteLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #A0B9CE;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(5)) + "px;"
                                                    ";}");

    sendButton->setStyleSheet("background-color: " BUTON_COLOR_ORANGE "; border-radius: " + QString::number((int)s(18)) + "px; border: 1px solid #eee; color: #fff; ");
}

void walletsend::updateSend() {
    recentTransactionsItemModel->clear();
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
    recentTransactionsTableView->horizontalHeader()->setStyleSheet("color: " COLOR_GREY_MID "; ");
    recentTransactionsTableView->horizontalHeader()->setEnabled(false);
    recentTransactionsTableView->setAlternatingRowColors(true);
    recentTransactionsTableView->setModel(recentTransactionsItemModel);
    recentTransactionsTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    recentTransactionsTableView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    QList<QStringList> list = events::getRecentTxTransactions();
    QStandardItem *tmp;
    QLabel *statusDetails;
    QPushButton *viewDetails;
    for(int cnt = 0; cnt < list.count(); cnt++) {
        QList<QStandardItem *> item = QList<QStandardItem *>();
        recentTransactionsTableView->setRowHidden(cnt, false);
        tmp = new QStandardItem();
        tmp->setText(list[cnt][0]);
        tmp->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setText(list[cnt][1]);
        tmp->setTextAlignment(Qt::AlignCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setText(list[cnt][2]);
        tmp->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setText(list[cnt][3]);
        tmp->setTextAlignment(Qt::AlignCenter);
        tmp->setForeground(QBrush(0xFFFFFF));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setText(list[cnt][4]);
        tmp->setTextAlignment(Qt::AlignCenter);
        tmp->setForeground(QBrush(0xF0F0F0));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);
        tmp = new QStandardItem();
        tmp->setText(list[cnt][5]);
        tmp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        tmp->setForeground(QBrush(0x909090));
        tmp->setEnabled(false);
        tmp->setSelectable(false);
        item.append(tmp);
        recentTransactionsItemModel->appendRow(item);

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
        viewDetails->installEventFilter(this);
        recentTransactionsTableView->setIndexWidget(recentTransactionsItemModel->index(cnt, 3), viewDetails);
        recentTransactionsTableView->setIndexWidget(recentTransactionsItemModel->index(cnt, 4), statusDetails);
        statusDetails = (QLabel *)recentTransactionsTableView->indexWidget(recentTransactionsItemModel->index(cnt, 4));
        statusDetails->setText(list[cnt][4]);
        if(!list[cnt][4].compare(_tr("Completed"))) {
            statusDetails->setStyleSheet("background-color: " BUTON_COLOR_GREEN "; border-radius: " + QString::number((int)s(14)) + "px; border: " + QString::number((int)s(6)) + "px solid #fff; color: #fff; ");
        }
    }
    refreshSize();
}

void walletsend::run() {
    /*if(!mdiArea->isVisible())
        showDetailsWindow.setVisible(false);*/
    if(pastScale != events::getScale()) {
        pastScale = events::getScale();
        updateSend();
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
        unreceivedBalanceValueTotalLabel->setText(_tr("Last sync") + " " + textformating::toDate(lastSyncDateTime));
    }*/
    if(totalLyraSendedLastWeek != events::getTotalLyraSendedLastWeek()) {
        totalLyraSendedLastWeek = events::getTotalLyraSendedLastWeek();
        totalLyrSendedValueLabel->setText(textformating::toValue(totalLyraSendedLastWeek) + " LYR");
    }
    if(totalLyraPercentageSendedLastWeek != events::getTotalLyraPercentageSendedLastWeekOfTotal()) {
        totalLyraPercentageSendedLastWeek = events::getTotalLyraPercentageSendedLastWeekOfTotal();
        totalLyrSendedLastWeekLabel->setText( textformating::toPercentage(totalLyraPercentageSendedLastWeek) + " " + _tr("of Total Wallet"));
    }
    if(lastSendedTransactionsListModifyedCnt != events::getLastSendedTransactionsListModifyedCnt()) {
        lastSendedTransactionsListModifyedCnt = events::getLastSendedTransactionsListModifyedCnt();
        QList<QPair<QString, QString>> rxTr = events::getLastSendedTransactionsList();
        if(rxTr.count() != 0) {
            QPair<QString, QString> tmp = rxTr[0];
            lastSendDateLabel->setText(tmp.first);
            lastSendValueLabel->setText(tmp.second);
        } else {
            lastSendDateLabel->setText("");
            lastSendValueLabel->setText("");
        }
    }
    if(recentTxTransactionsModifyedCnt != events::getRecentTxTransactionsModifyedCnt()) {
        recentTxTransactionsModifyedCnt = events::getRecentTxTransactionsModifyedCnt();
        updateSend();
    }
    if(idModifyedCnt != events::getIdModifyedCnt()) {
        idModifyedCnt = events::getIdModifyedCnt();
        walletAddressIdLabel->setText(events::getId());
        showDetailsWindow->setDetailsVisible(false);
        events::setUnreceivedBallance("Please wait");
        run();
        //wallet::checkNewTransactions();
        tickedupdates::triggerAccountRefresh();
    }
    if(tokenListModifyedCnt != events::getTokenListModifyedCnt()) {
        tokenListModifyedCnt = events::getTokenListModifyedCnt();
        QList<QPair<QString, double>> list = events::getTokenList();
        QString tmp = tokenComboBox->currentText();
        tokenComboBox->clear();
        QPair<QString, double> pair;
        foreach(pair, list) {
            tokenComboBox->addItem(pair.first.replace("tether/", "$"));
        }
        tokenComboBox->setCurrentText(tmp);
        if(list.count()) {
            double value = (events::getBtcUsdSelect() ? events::getTokenPricePair("LYR_USD") : events::getTokenPricePair("LYR_BTC")) * list[tokenComboBox->currentIndex()].second;
            btcUsdLineEdit->setPlaceholderText(_tr("Max") + ": " + textformating::toValue(value, events::getBtcUsdSelect() ? 8 : 8));
        }
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
        btcUsdSwitchLabel->repaint();
        QList<QPair<QString, double>> list = events::getTokenList();
        if(list.count()) {
            double value = (events::getBtcUsdSelect() ? events::getTokenPricePair("LYR_USD") : events::getTokenPricePair("LYR_BTC")) * list[tokenComboBox->currentIndex()].second;
            btcUsdLineEdit->setPlaceholderText(_tr("Max") + ": " + textformating::toValue(value, events::getBtcUsdSelect() ? 8 : 8));
            value = (events::getBtcUsdSelect() ? events::getTokenPricePair("LYR_USD") : events::getTokenPricePair("LYR_BTC")) * ammountLineEdit->text().toDouble();
            if(value != 0.0)
                btcUsdLineEdit->setText(textformating::toValue(value, events::getBtcUsdSelect() ? 8 : 8));
            else
                btcUsdLineEdit->setText("");
        }
    }
    showDetailsWindow->run();
}

bool walletsend::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonRelease) {
        if(mouseEvent->button() == Qt::LeftButton) {
            if (obj == btcUsdSwitchLabel) {
                events::setBtcUsdSelect(!events::getBtcUsdSelect());
                return true;
            } else if(obj == walletAddressIdLabel) {
                QClipboard* clipboard = QApplication::clipboard();
                clipboard->setText(walletAddressIdLabel->text());
                QMessageBox::information( this, this->windowTitle(),
                        _tr("Account ID copied to clipboard."),
                        QMessageBox::Ok,
                        QMessageBox::Ok);
            } else {
                for( int cnt = 0; cnt < recentTransactionsTableView->verticalHeader()->count(); cnt++) {
                    QPushButton *details = (QPushButton *)recentTransactionsTableView->indexWidget(recentTransactionsItemModel->index(cnt, 3));
                    if (obj == details) {
                        showDetails(cnt);
                        return true;
                    }
                }

            }
        }
    }
    return false;
}

void walletsend::on_destinationWalletIdLineEdit_Changed(const QString &str) {
    if(signatures::validateAccountId(str)) {
        destinationWalletIdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
    } else {
        destinationWalletIdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    }
}

void walletsend::on_tokenComboBox_TextChanget(const QString &) {
    if(tokenComboBox->count() != 0) {
        QList<QPair<QString, double>> list = events::getTokenList();
        double value = (events::getBtcUsdSelect() ? events::getTokenPricePair("LYR_USD") : events::getTokenPricePair("LYR_BTC")) * list[tokenComboBox->currentIndex()].second;
        ammountLineEdit->setPlaceholderText(_tr("Max") + ": " + textformating::toValue(list[tokenComboBox->currentIndex()].second ? (list[tokenComboBox->currentIndex()].second - (list[tokenComboBox->currentIndex()].first.compare("LYR") ? 0.00 : LYRA_TX_FEE)) : 0.0));
        btcUsdLineEdit->setPlaceholderText(_tr("Max") + ": " + textformating::toValue(value, events::getBtcUsdSelect() ? 2 : 8));
        on_ammountLineEdit_Changed(ammountLineEdit->text());
        on_ammountUsdBtcLineEdit_Changed(ammountLineEdit->text());
    }
}

void walletsend::on_ammountLineEdit_Changed(const QString &) {
    QList<QPair<QString, double>> list = events::getTokenList();
    if(list[tokenComboBox->currentIndex()].second >= ammountLineEdit->text().toDouble() + LYRA_TX_FEE) {
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

void walletsend::on_ammountUsdBtcLineEdit_Changed(const QString &) {
    QList<QPair<QString, double>> list = events::getTokenList();
    if(list[tokenComboBox->currentIndex()].second >= ammountLineEdit->text().toDouble() + LYRA_TX_FEE) {
        ammountValueStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
        btcUsdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
    } else {
        ammountValueStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
        btcUsdStatusLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    }
    /*double value = (events::getBtcUsdSelect() ? events::getUsdLyrPair() : events::getBtcLyrPair()) / btcUsdLineEdit.text().toDouble();
    if(value != 0.0)
        ammountLineEdit.setText(textformating::toValue(value, events::getBtcUsdSelect() ? 8 : 8));
    else
        ammountLineEdit.setText("");*/
}

/*void walletsend::on_PasswordLineEdit_Changed(const QString &) {
    if(!passwordConfirmationLineEdit->text().compare(events::getWalletUserPassword().second)) {
        passwordConfirmationLineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #A0B9CE;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    } else {
        passwordConfirmationLineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #FF151F;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    }
}*/

void walletsend::on_Sync_ButtonPressed() {
    if(userInputSemaphore)
        return;
    userInputSemaphore = true;
    wallet::sync();
    userInputSemaphore = false;
}

void walletsend::on_Send_ButtonPressed() {
    if(userInputSemaphore)
        return;
    userInputSemaphore = true;
    int index = events::getSelectedNameKeyIndex();
    QString accId = events::getAccountId(index);
    if(!accId.length())
        return;
    QStringList names = events::getAccountNameList();
    sendButton->setText(_tr("WAIT") + "...");
    if(names.count()/* && !passwordConfirmationLineEdit->text().compare(events::getWalletUserPassword().second)*/) {
        if(walletErr_e::WALLET_ERR_OK == walletbalance::send(index, destinationWalletIdLineEdit->text(), tokenComboBox->currentText().replace("$", "tether/"), ammountLineEdit->text().toDouble())) {
            wallethistory::updateAccount(names[index], accId);
            wallethistory::setNote(names[index], wallethistory::getAccount(names[index]).count() - 1, noteLineEdit->text());
            destinationWalletIdLineEdit->clear();
            ammountLineEdit->clear();
            //passwordConfirmationLineEdit->clear();
            noteLineEdit->clear();
            populate::refreshAll();
            events::setWalletHistoryChanged();
            events::setUpdateHistory();
        } else {
            sendButton->setStyleSheet("background-color: " BUTON_COLOR_RED "; border-radius: " + QString::number((int)s(6)) + "px; border: 1px solid #eee; color: #fff; ");
            sendButton->setText(_tr("ERROR!"));
            sendButton->repaint();
            QThread::sleep(1);
        }
    }
    sendButton->setText(_tr("SEND"));
    sendButton->setStyleSheet("background-color: " BUTON_COLOR_ORANGE "; border-radius: " + QString::number((int)s(6)) + "px; border: 1px solid #eee; color: #fff; ");
    userInputSemaphore = false;
}

void walletsend::showDetails(int order) {
    QList<QStringList> list = events::getRecentTxTransactions();
    int transactionNr = list[order][6].toInt() - 1;
    int index = events::getSelectedNameKeyIndex();
    QStringList names = events::getAccountNameList();
    QList<QList<QMap<QString, QString>>> wallet = wallethistory::getAccount(names[index]);
    QList<QMap<QString, QString>> transaction = wallet[transactionNr];
    showDetailsWindow->show(transaction);
}

void walletsend::on_viewDetails0_PushButton() {
    showDetails(0);
}

void walletsend::on_viewDetails1_PushButton() {
    showDetails(1);
}

void walletsend::on_viewDetails2_PushButton() {
    showDetails(2);
}

void walletsend::on_viewDetails3_PushButton() {
    showDetails(3);
}

void walletsend::on_viewDetails4_PushButton() {
    showDetails(4);
}

void walletsend::on_viewDetails5_PushButton() {
    showDetails(5);
}

void walletsend::on_TefreshTotalSendedLastWeekTimer() {
    wallet::calculateLastWeek();
}


