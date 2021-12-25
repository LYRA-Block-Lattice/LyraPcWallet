#include <QMessageBox>

#include "swapwindow.h"

#include "language/translate.h"
#include "wallet/events.h"
#include "wallet/rpc/swap.h"
#include "pages/pc/textformating.h"
#include "configlyra.h"
#include "wallet/rpc/walletbalance.h"
#include "pages/pc/populate.h"
#include "wallet/tickedupdates.h"

#define s(s) _scale(s)

swapwindow::swapwindow() {

}

swapwindow::~swapwindow() {

}

void swapwindow::setVars(QMdiSubWindow *window, QWidget *parent) {
    this->setParent(parent);
    this->parent = parent;
    this->window = window;

    mdiAreaSwap = new QMdiArea(window);
    mdiAreaSwap->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaSwap->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaSwap->setVisible(true);
    widgetSwap = new QWidget(mdiAreaSwap);
    widgetSwap->show();
    setParent(mdiAreaSwap);
    windowSwap = new QMdiSubWindow(mdiAreaSwap);
    windowSwap->setWidget(widgetSwap);
    windowSwap->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowSwap->setWindowFlag(Qt::FramelessWindowHint, true);
    windowSwap->setGeometry(0, 0, mdiAreaSwap->width(), mdiAreaSwap->height());
    windowSwap->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/whiteBack.png)");
    windowSwap->setVisible(true);

    fromLabel = new QLabel(mdiAreaSwap);
    fromLabel->setStyleSheet("color: #777;");
    fromLabel->setAlignment(Qt::AlignLeft);
    fromLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    fromLabel->setVisible(true);

    fromValueLineEdit = new QLineEdit(mdiAreaSwap);
    fromValueLineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    fromValueLineEdit->setVisible(true);
    fromValueLineEdit->setTextMargins(5, 1, 5, 1);
    fromValueLineEdit->setAlignment(Qt::AlignCenter);
    connect(fromValueLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_fromValueLineEdit_textChanged(const QString &)));

    fromAmountLabel = new QLabel(mdiAreaSwap);
    fromAmountLabel->setStyleSheet("color: #777;");
    fromAmountLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    fromAmountLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    fromAmountLabel->setVisible(true);

    fromValueComboBox = new QComboBox(mdiAreaSwap);
    fromValueComboBox->setAttribute(Qt::WA_TranslucentBackground, true);
    fromValueComboBox->setAutoFillBackground(false);
    fromValueComboBox->setVisible(true);
    fromValueComboBox->setCursor(Qt::PointingHandCursor);
    fromValueComboBox->addItems({"LYR"});
    connect(fromValueComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Ticker0_Changed(const QString &)));

    toLabel = new QLabel(mdiAreaSwap);
    toLabel->setStyleSheet("color: #777;");
    toLabel->setAlignment(Qt::AlignLeft);
    toLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    toLabel->setVisible(true);

    toValueLineEdit = new QLineEdit(mdiAreaSwap);
    toValueLineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    toValueLineEdit->setVisible(true);
    toValueLineEdit->setTextMargins(5, 1, 5, 1);
    toValueLineEdit->setAlignment(Qt::AlignCenter);
    connect(toValueLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_toValueLineEdit_textChanged(const QString &)));

    toAmountLabel = new QLabel(mdiAreaSwap);
    toAmountLabel->setStyleSheet("color: #777;");
    toAmountLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    toAmountLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    toAmountLabel->setVisible(true);

    toValueComboBox = new QComboBox(mdiAreaSwap);
    toValueComboBox->setAttribute(Qt::WA_TranslucentBackground, true);
    toValueComboBox->setAutoFillBackground(false);
    toValueComboBox->setVisible(true);
    toValueComboBox->setCursor(Qt::PointingHandCursor);
    QList<QStringList> tokenList;
    if(events::getRpcNetwork() == events::network_e::NETWORK_MAINNET)
        tokenList = SUPPORTED_COINS_MAINNET;
    else
        tokenList = SUPPORTED_COINS_TESTNET;
    foreach(QStringList token, tokenList) {
        toValueComboBox->addItem(token[1] + (token[1].length() ? "/ " : "") + token[0]);
    }

    connect(toValueComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Ticker1_Changed(const QString &)));


    reversePushButton = new QPushButton(mdiAreaSwap);
    reversePushButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/swap/reverse.png); border-radius: 2px; border: 1px solid #eee; color: #fff; ");
    reversePushButton->setFlat(true);
    reversePushButton->setCursor(Qt::PointingHandCursor);
    reversePushButton->setVisible(true);
    connect(reversePushButton, SIGNAL(clicked()),this, SLOT(on_reverseTokens_ButtonPressed()));

    totalLiquidateLabel = new QLabel(mdiAreaSwap);
    totalLiquidateLabel->setStyleSheet("color: #777;");
    totalLiquidateLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    totalLiquidateLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalLiquidateValueLabel = new QLabel(mdiAreaSwap);
    totalLiquidateValueLabel->setStyleSheet("color: #777;");
    totalLiquidateValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    totalLiquidateValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    estimatedPriceLabel = new QLabel(mdiAreaSwap);
    estimatedPriceLabel->setStyleSheet("color: #777;");
    estimatedPriceLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    estimatedPriceLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    estimatedPriceValueLabel = new QLabel(mdiAreaSwap);
    estimatedPriceValueLabel->setStyleSheet("color: #777;");
    estimatedPriceValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    estimatedPriceValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    youSellLabel = new QLabel(mdiAreaSwap);
    youSellLabel->setStyleSheet("color: #777;");
    youSellLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    youSellLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    youSellValueLabel = new QLabel(mdiAreaSwap);
    youSellValueLabel->setStyleSheet("color: #777;");
    youSellValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    youSellValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    youGetLabel = new QLabel(mdiAreaSwap);
    youGetLabel->setStyleSheet("color: #777;");
    youGetLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    youGetLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    youGetValueLabel = new QLabel(mdiAreaSwap);
    youGetValueLabel->setStyleSheet("color: #777;");
    youGetValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    youGetValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    priceimpactLabel = new QLabel(mdiAreaSwap);
    priceimpactLabel->setStyleSheet("color: #777;");
    priceimpactLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    priceimpactLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    priceimpactValueLabel = new QLabel(mdiAreaSwap);
    priceimpactValueLabel->setStyleSheet("color: #777;");
    priceimpactValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    priceimpactValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    poolFeeLabel = new QLabel(mdiAreaSwap);
    poolFeeLabel->setStyleSheet("color: #777;");
    poolFeeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    poolFeeLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    poolFeeValueLabel = new QLabel(mdiAreaSwap);
    poolFeeValueLabel->setStyleSheet("color: #777;");
    poolFeeValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    poolFeeValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    liquidateFeeLabel = new QLabel(mdiAreaSwap);
    liquidateFeeLabel->setStyleSheet("color: #777;");
    liquidateFeeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    liquidateFeeLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    liquidateFeeValueLabel = new QLabel(mdiAreaSwap);
    liquidateFeeValueLabel->setStyleSheet("color: #777;");
    liquidateFeeValueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    liquidateFeeValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    totalLiquidateLabel->setVisible(true);
    totalLiquidateValueLabel->setVisible(true);
    estimatedPriceLabel->setVisible(true);
    estimatedPriceValueLabel->setVisible(true);
    youSellLabel->setVisible(true);
    youSellValueLabel->setVisible(true);
    youGetLabel->setVisible(true);
    youGetValueLabel->setVisible(true);
    priceimpactLabel->setVisible(true);
    priceimpactValueLabel->setVisible(true);
    poolFeeLabel->setVisible(true);
    poolFeeValueLabel->setVisible(true);
    liquidateFeeLabel->setVisible(true);
    liquidateFeeValueLabel->setVisible(true);

    swapPushButton = new QPushButton(mdiAreaSwap);
    swapPushButton->setFlat(true);
    swapPushButton->setCursor(Qt::PointingHandCursor);
    swapPushButton->setVisible(true);
    swapPushButton->setEnabled(false);
    connect(swapPushButton, SIGNAL(clicked()),this, SLOT(on_swap_ButtonPressed()));


    fetchPool.setParent(this);
    fetchPool.setInterval(100);
    fetchPool.start();
    connect(&fetchPool, SIGNAL(timeout()), this, SLOT(on_FetchPool()));

    refreshSize();
}

void swapwindow::setState(state_e state) {
    currentState = state;
}

void swapwindow::refreshFonts() {
    //reversePushButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));

    fromLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    fromValueLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    fromAmountLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    fromValueComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));

    toLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    toValueLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    toAmountLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    toValueComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));


    totalLiquidateLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    totalLiquidateValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    estimatedPriceLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    estimatedPriceValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    youSellLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    youSellValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    youGetLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    youGetValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    priceimpactLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    priceimpactValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    poolFeeLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    poolFeeValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));
    liquidateFeeLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    liquidateFeeValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.9)));

    swapPushButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
}

void swapwindow::refreshSize() {
    reversePushButton->setGeometry(s(280), s(125), s(35), s(30));

    fromLabel->setGeometry(s(320), s(45), s(200), s(19));
    fromValueLineEdit->setGeometry(s(320), s(70), s(200), s(39));
    fromValueLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #777;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(19)) + "px;"
                                                    ";}");
    fromAmountLabel->setGeometry(s(440), s(45), s(350), s(19));
    fromValueComboBox->setGeometry(s(540), s(70), s(250), s(39));
    fromValueComboBox->setStyleSheet(""
        "QComboBox {   "
               "combobox-popup: 1;"
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(19)) + "px;"
               "padding: 1px " + QString::number((int)s(18)) + "px 1px " + QString::number((int)s(3)) + "px;"
               "text-align: right;"
               ";}"
        "QComboBox::drop-down {border-width: 1px;} "
        "QComboBox::down-arrow {image: url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/walletComboBoxArrow.png);}"
        "QComboBox QAbstractItemView {"
               "border: 1px solid darkgray;"
               "color: #aaa;"
               "padding: 1px 1px 1px 1px;"
               "selection-background-color: darkgray;"
               "}"
    );
    toLabel->setGeometry(s(320), s(165), s(200), s(19));
    toValueLineEdit->setGeometry(s(320), s(190), s(200), s(39));
    toValueLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #777;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(19)) + "px;"
                                                    ";}");
    toAmountLabel->setGeometry(s(440), s(165), s(350), s(19));
    toValueComboBox->setGeometry(s(540), s(190), s(250), s(39));
    toValueComboBox->setStyleSheet(""
        "QComboBox {   "
               "combobox-popup: 1;"
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(19)) + "px;"
               "padding: 1px " + QString::number((int)s(18)) + "px 1px " + QString::number((int)s(3)) + "px;"
               "text-align: right;"
               ";}"
        "QComboBox::drop-down {border-width: 1px;} "
        "QComboBox::down-arrow {image: url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/walletComboBoxArrow.png);}"
        "QComboBox QAbstractItemView {"
               "border: 1px solid darkgray;"
               "color: #aaa;"
               "padding: 1px 1px 1px 1px;"
               "selection-background-color: darkgray;"
               "}"
    );

    estimatedPriceLabel->setGeometry(s(320), s(250), s(300), s(19));
    estimatedPriceValueLabel->setGeometry(s(490), s(250), s(300), s(19));
    youSellLabel->setGeometry(s(320), s(275), s(300), s(19));
    youSellValueLabel->setGeometry(s(490), s(275), s(300), s(19));
    youGetLabel->setGeometry(s(320), s(300), s(300), s(19));
    youGetValueLabel->setGeometry(s(490), s(300), s(300), s(19));
    priceimpactLabel->setGeometry(s(320), s(325), s(300), s(19));
    priceimpactValueLabel->setGeometry(s(490), s(325), s(300), s(19));
    poolFeeLabel->setGeometry(s(320), s(350), s(300), s(19));
    poolFeeValueLabel->setGeometry(s(490), s(350), s(300), s(19));
    liquidateFeeLabel->setGeometry(s(320), s(375), s(300), s(19));
    liquidateFeeValueLabel->setGeometry(s(490), s(375), s(300), s(19));

    swapPushButton->setGeometry(s(370), s(410), s(370), s(39));
    swapPushButton->setStyleSheet("background-color: " BUTON_COLOR_BLUE "; border-radius: " + QString::number((int)s(19)) + "px; border: 1px solid #eee; color: #fff; ");

    totalLiquidateLabel->setGeometry(s(320), s(460), s(300), s(38));
    totalLiquidateValueLabel->setGeometry(s(490), s(460), s(300), s(38));
    refreshFonts();
}

void swapwindow::refreshLanguage() {
    //reversePushButton->setText(_tr("REVERSE"));
    fromLabel->setText(_tr("From"));
    //fromValueLineEdit->setPlaceholderText(_tr("FROM VALUE"));

    toLabel->setText(_tr("To"));
    //toValueLineEdit->setPlaceholderText(_tr("TO VALUE"));


    totalLiquidateLabel->setText(_tr("Total Liquidity"));
    estimatedPriceLabel->setText(_tr("Estimated price"));
    youSellLabel->setText(_tr("You will sell"));
    youGetLabel->setText(_tr("You will get"));
    priceimpactLabel->setText(_tr("Price impact"));
    poolFeeLabel->setText(_tr("Pool fee"));
    liquidateFeeLabel->setText(_tr("Network fee"));

    swapPushButton->setText(_tr("Swap"));

    fromAmountLabel->setText(_tr("Balance") + ": " + textformating::toValue(amountToken0, 8));
    toAmountLabel->setText(_tr("Balance") + ": " + textformating::toValue(amountToken1, 8));
    refreshFonts();
}

void swapwindow::refreshTokenAmount() {

}

void swapwindow::repopulatetokensComboBoxes() {
    fromValueComboBox->clear();
    fromValueComboBox->addItems({"LYR"});
    toValueComboBox->clear();
    QList<QStringList> tokenList;
    if(events::getRpcNetwork() == events::network_e::NETWORK_MAINNET)
        tokenList = SUPPORTED_COINS_MAINNET;
    else
        tokenList = SUPPORTED_COINS_TESTNET;
    foreach(QStringList token, tokenList) {
        toValueComboBox->addItem(token[1] + (token[1].length() ? "/ " : "") + token[0]);
    }
    fetchPool.setInterval(1000);
    fetchPool.start();
}

void swapwindow::run() {
    if(pastState != currentState) {
        pastState = currentState;
        if(currentState == STATE_SWAP) {
            mdiAreaSwap->setVisible(true);
            windowSwap->setVisible(true);toValueComboBox->clear();
            repopulatetokensComboBoxes();
            refreshTokenAmount();
        } else {
            mdiAreaSwap->setVisible(false);
            windowSwap->setVisible(false);
        }
    }
    if(pastScale != events::getScale()) {
        pastScale = events::getScale();
        mdiAreaSwap->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowSwap->setGeometry(0, 0, mdiAreaSwap->width(), mdiAreaSwap->height());
        refreshSize();
    }
    if(pastLanguage.compare(translate::getCurrentLang())) {
        pastLanguage = translate::getCurrentLang();
        refreshLanguage();
    }
    if(network != events::getRpcNetwork()) {
        repopulatetokensComboBoxes();
    }
    if(network != events::getRpcNetwork() ||
            selectedNameKeyIndex != events::getSelectedNameKeyIndex() ||
            recentTransactionsCnt != events::getRecentTransactionsModifyedCnt()) {
        network = events::getRpcNetwork();
        recentTransactionsCnt = events::getRecentTransactionsModifyedCnt();
        selectedNameKeyIndex = events::getSelectedNameKeyIndex();
        fetchPool.setInterval(100);
        fetchPool.start();
    }
    if(triggerAccRefresh) {
        triggerAccRefresh = false;
        tickedupdates::triggerAccountRefresh();
    }
}

void swapwindow::on_fromValueLineEdit_textChanged(const QString &) {
    if(valueSource == VALUE_SOURCE_SELF || valueSource == VALUE_SOURCE_TOKEN0) {
        valueSource = VALUE_SOURCE_TOKEN0;
        bool ok = false;
        fromValueLineEdit->text().remove(",").toDouble(&ok);
        if(!ok && fromValueLineEdit->text().length() > 0)
            fromValueLineEdit->setText(fromValueLineEdit->text().left(fromValueLineEdit->text().length()-1));
        else {
            //toValueLineEdit->setText(fromValueLineEdit->text());
            lastEditedVal = LAST_EDITED_VAL_TOKEN0;
            fetchPool.setInterval(1000);
            fetchPool.start();
        }
    }/* else {
        valueSource = VALUE_SOURCE_SELF;
    }*/
}

void swapwindow::on_toValueLineEdit_textChanged(const QString &) {
    if(valueSource == VALUE_SOURCE_SELF || valueSource == VALUE_SOURCE_TOKEN1) {
        valueSource = VALUE_SOURCE_TOKEN1;
        bool ok = false;
        toValueLineEdit->text().remove(",").toDouble(&ok);
        if(!ok && toValueLineEdit->text().length() > 0)
            toValueLineEdit->setText(toValueLineEdit->text().left(toValueLineEdit->text().length()-1));
        else {
            //fromValueLineEdit->setText(toValueLineEdit->text());
            lastEditedVal = LAST_EDITED_VAL_TOKEN1;
            fetchPool.setInterval(1000);
            fetchPool.start();
        }
    }/* else {
        valueSource = VALUE_SOURCE_SELF;
    }*/
}

void swapwindow::on_Ticker0_Changed(const QString &) {
    //refreshTokenAmount();
    valueSource = VALUE_SOURCE_TOKEN0;
    lastEditedVal = LAST_EDITED_VAL_TOKEN0;
    fetchPool.setInterval(100);
    fetchPool.start();
}

void swapwindow::on_Ticker1_Changed(const QString &) {
    //refreshTokenAmount();
    valueSource = VALUE_SOURCE_TOKEN0;
    lastEditedVal = LAST_EDITED_VAL_TOKEN0;
    fetchPool.setInterval(100);
    fetchPool.start();
}

void swapwindow::on_reverseTokens_ButtonPressed() {
    QStringList tmp;
    QString token0 = fromValueComboBox->currentText();
    QString token1 = toValueComboBox->currentText();
    for(int cnt = 0; cnt < fromValueComboBox->count(); cnt++) {
        tmp.append(fromValueComboBox->itemText(cnt));
    }
    fromValueComboBox->clear();
    for(int cnt = 0; cnt < toValueComboBox->count(); cnt++) {
        fromValueComboBox->addItem(toValueComboBox->itemText(cnt));
    }
    toValueComboBox->clear();
    toValueComboBox->addItems(tmp);
    fromValueComboBox->setCurrentText(token1);
    toValueComboBox->setCurrentText(token0);
    fromValueLineEdit->setText(toValueLineEdit->text());
    fetchPool.setInterval(100);
    fetchPool.start();
}

void swapwindow::on_FetchPool() {
    fetchPool.stop();
    QList<QStringList> tList = SUPPORTED_COINS_MAINNET;
    QString tokenFrom = fromValueComboBox->currentText();
    foreach(QStringList token, tList) {
        if(!tokenFrom.compare(token[1] + (token[1].length() ? "/ " : "") + token[0])) {
            tokenFrom = token[0];
            break;
        }
    }
    QString tokenTo = toValueComboBox->currentText();
    foreach(QStringList token, tList) {
        if(!tokenTo.compare(token[1] + (token[1].length() ? "/ " : "") + token[0])) {
            tokenTo = token[0];
            break;
        }
    }

    QList<QPair<QString, double>> tokenList = events::getTokenList();
    poolInfo = pool::info(tokenFrom.replace(SYMBOL_FOR_TETHERED_TOKEN, "tether/"),
                          tokenTo.replace(SYMBOL_FOR_TETHERED_TOKEN, "tether/"));
#if VORBOSE_LEVEL >= 1
    qDebug() << poolInfo.height;
    qDebug() << poolInfo.poolId;
    qDebug() << poolInfo.amount0;
    qDebug() << poolInfo.amount1;
    qDebug() << poolInfo.valid;
#endif
    QPair<QString, double>tmpToken;
    amountToken0 = 0.0;
    foreach(tmpToken, tokenList) {
        if(!tmpToken.first.compare(tokenFrom.replace(SYMBOL_FOR_TETHERED_TOKEN, "tether/"))) {
            amountToken0 = tmpToken.second;
            break;
        }
    }
    amountToken1 = 0.0;
    foreach(tmpToken, tokenList) {
        if(!tmpToken.first.compare(tokenTo.replace(SYMBOL_FOR_TETHERED_TOKEN, "tether/"))) {
            amountToken1 = tmpToken.second;
            break;
        }
    }
    refreshLanguage();
    if(poolInfo.valid) {
        totalLiquidateValueLabel->setText(textformating::toValue(poolInfo.amount0) + " " +
                                          tokenFrom.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN) + "\n" +
                                            textformating::toValue(poolInfo.amount1) + " " +
                                                tokenTo.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
        if(lastEditedVal == LAST_EDITED_VAL_TOKEN0) {
            poolCalculate = pool::calculate(poolInfo.poolId,
                                            tokenFrom.replace(SYMBOL_FOR_TETHERED_TOKEN, "tether/"),
                                                fromValueLineEdit->text().remove(",").toDouble(), 0.01);
        }
        if(poolCalculate.valid) {
#if VORBOSE_LEVEL >= 1
            qDebug() << poolCalculate.providerFee << "ProviderFee";
            qDebug() << poolCalculate.protocolFee << "ProtocolFee";
            qDebug() << poolCalculate.swapInToken << "SwapInToken";
            qDebug() << poolCalculate.swapInAmount << "SwapInAmount";
            qDebug() << poolCalculate.swapOutToken << "SwapOutToken";
            qDebug() << poolCalculate.swapOutAmount << "SwapOutAmount";
            qDebug() << poolCalculate.price << "Price";
            qDebug() << poolCalculate.priceImpact << "PriceImpact";
            qDebug() << poolCalculate.minimumReceive << "MinimumReceived";
            qDebug() << poolCalculate.payToProvider << "PayToProvider";
            qDebug() << poolCalculate.payToAuthorizer << "PayToAuthorizer";
#endif
            estimatedPriceValueLabel->setText(textformating::toValue(poolCalculate.price, 8) + " " + poolCalculate.swapInToken.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN) + " " + _tr("per") + " " + poolCalculate.swapOutToken.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
            youSellValueLabel->setText(textformating::toValue(poolCalculate.swapInAmount, 8) + " " + poolCalculate.swapInToken.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
            youGetValueLabel->setText(textformating::toValue(poolCalculate.swapOutAmount, 8) + " " + poolCalculate.swapOutToken.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
            priceimpactValueLabel->setText(textformating::toValue(poolCalculate.priceImpact * 100.0, 8) + " %");
            poolFeeValueLabel->setText(textformating::toValue(poolCalculate.payToProvider, 8) + " " + poolCalculate.swapInToken.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
            liquidateFeeValueLabel->setText(textformating::toValue(poolCalculate.payToAuthorizer, 8) + " LYR");

            if(lastEditedVal == LAST_EDITED_VAL_TOKEN0) {
                if(fromValueLineEdit->text().length())
                    toValueLineEdit->setText(textformating::toValue(poolCalculate.swapOutAmount, 8));
                else
                    toValueLineEdit->clear();
            } else {
                if(toValueLineEdit->text().length())
                    fromValueLineEdit->setText(textformating::toValue(toValueLineEdit->text().remove(",").toDouble() * poolCalculate.price, 8));
                else
                    fromValueLineEdit->clear();
            }
            swapPushButton->setEnabled(true);
        } else {
            swapPushButton->setEnabled(false);
        }
        valueSource = VALUE_SOURCE_SELF;
    } else {
        totalLiquidateValueLabel->setText(_tr("No liquidity found for this pair"));
        estimatedPriceValueLabel->setText(tokenFrom.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
        youSellValueLabel->setText(tokenFrom.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
        youGetValueLabel->setText(tokenTo.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
        priceimpactValueLabel->setText(" %");
        poolFeeValueLabel->setText(" LYR");
        liquidateFeeValueLabel->setText(tokenFrom.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
    }
}

void swapwindow::on_swap_ButtonPressed() {
    bool unreceived = false;
    int height = 0;
    QList<QStringList> tList = SUPPORTED_COINS_MAINNET;
    QString tokenFrom = fromValueComboBox->currentText();
    foreach(QStringList token, tList) {
        if(!tokenFrom.compare(token[1] + (token[1].length() ? "/ " : "") + token[0])) {
            tokenFrom = token[0];
            break;
        }
    }
    QString tokenTo = toValueComboBox->currentText();
    foreach(QStringList token, tList) {
        if(!tokenTo.compare(token[1] + (token[1].length() ? "/ " : "") + token[0])) {
            tokenTo = token[0];
            break;
        }
    }

    walletErr_e response = swap::swapTokens(events::getSelectedNameKeyIndex(),
                     tokenFrom.replace(SYMBOL_FOR_TETHERED_TOKEN, "tether/"),
                        tokenTo.replace(SYMBOL_FOR_TETHERED_TOKEN, "tether/"),
                            tokenFrom.replace(SYMBOL_FOR_TETHERED_TOKEN, "tether/"),
                                fromValueLineEdit->text().remove(",").toDouble(),
                                    poolCalculate.minimumReceive);
    QString accId = events::getAccountId(events::getSelectedNameKeyIndex());
    switch (response) {
    case walletErr_e::WALLET_ERR_OK:
        QMessageBox::information( this, this->windowTitle(),
                _tr("SWAP successfully completed.") + "\n" +
                    _tr("Now we need to create receive block."),
                QMessageBox::Ok,
                QMessageBox::Ok);
        if(!accId.length()) {
            break;
        }
        walletbalance::balance(accId, &height, &unreceived);
        if(unreceived) {
            wallet::sync();
        }
        triggerAccRefresh = true;
        break;
    default:
        QMessageBox::critical( this, this->windowTitle(),
                _tr("ERROR: Unknown"),
                QMessageBox::Ok,
                QMessageBox::Ok);
        break;
    }
}


