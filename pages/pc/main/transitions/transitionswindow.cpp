#include "transitionswindow.h"

#include <QHeaderView>
#include <QScrollBar>
#include <QMouseEvent>
#include <QApplication>

#include "QGraphicsBlurEffect"

#include "language/translate.h"
#include "wallet/events.h"
#include "crypto/signatures.h"
#include "storage/walletfile.h"
#include "pages/pc/populate.h"
#include "wallet/rpc/wallethistory.h"
#include "pages/pc/textformating.h"

#define s(s) _scale(s)

transitionswindow::transitionswindow() {

}

transitionswindow::~transitionswindow() {

}

void transitionswindow::setVars(QMdiSubWindow *window, QWidget *parent) {
    this->setParent(parent);
    this->parent = parent;
    this->window = window;

    mdiAreaTransitions = new QMdiArea(window);
    mdiAreaTransitions->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaTransitions->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaTransitions->setVisible(true);
    widgetTransitions = new QWidget(mdiAreaTransitions);
    widgetTransitions->show();
    setParent(mdiAreaTransitions);
    windowTransitions = new QMdiSubWindow(mdiAreaTransitions);
    windowTransitions->setWidget(widgetTransitions);
    windowTransitions->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowTransitions->setWindowFlag(Qt::FramelessWindowHint, true);
    windowTransitions->setGeometry(0, 0, mdiAreaTransitions->width(), mdiAreaTransitions->height());
    windowTransitions->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/transitions.png)");
    windowTransitions->setVisible(true);

    txDirectionLabel = new QLabel(mdiAreaTransitions);
    txDirectionComboBox = new QComboBox(mdiAreaTransitions);

    tokenLabel = new QLabel(mdiAreaTransitions);
    tokenComboBox = new QComboBox(mdiAreaTransitions);

    filterLabel = new QLabel(mdiAreaTransitions);
    filterLineEdit = new QLineEdit(mdiAreaTransitions);

    historyTableView = new QTableView(mdiAreaTransitions);
    historyItemModel = new QStandardItemModel();

    txDirectionLabel->setStyleSheet("color: #333;");
    txDirectionLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    txDirectionLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    txDirectionLabel->setVisible(true);

    txDirectionComboBox->setCurrentIndex(0);
    txDirectionComboBox->setAutoFillBackground(false);
    txDirectionComboBox->setStyleSheet(""
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
        "QComboBox::down-arrow {image: url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/rangeComboBox.png);}"
        "QComboBox QAbstractItemView {"
               "border: 2px solid darkgray;"
               "color: #aaa;"
               "padding: 1px 1px 1px 1px;"
               "selection-background-color: darkgray;"
               "}"
    );
    txDirectionComboBox->setCursor(Qt::PointingHandCursor);
    txDirectionComboBox->setVisible(true);
    connect(txDirectionComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_TxDirection_Changed(const QString &)));

    tokenLabel->setStyleSheet("color: #333;");
    tokenLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    tokenLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    tokenLabel->setVisible(true);

    tokenComboBox->setCurrentIndex(0);
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
        "QComboBox::down-arrow {image: url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/rangeComboBox.png);}"
        "QComboBox QAbstractItemView {"
               "border: 2px solid darkgray;"
               "color: #aaa;"
               "padding: 1px 1px 1px 1px;"
               "selection-background-color: darkgray;"
               "}"
    );
    tokenComboBox->setCursor(Qt::PointingHandCursor);
    tokenComboBox->setVisible(true);
    connect(tokenComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Token_Changed(const QString &)));

    filterLabel->setStyleSheet("color: #333;");
    filterLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    filterLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    filterLabel->setVisible(true);

    filterLineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    filterLineEdit->setVisible(true);
    connect(filterLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_Filter_Changed(const QString &)));
    filterLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #777;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");

    showDetailsWindow = new walletshowdetails();
    showDetailsWindow->init(mdiAreaTransitions);

    refreshLanguage();
    windowTransitions->repaint();
}

/*
 * NEED OPTIMIZATION.
 */
void transitionswindow::refreshTable() {

    /*if(refreshTableSemaphore)
        return;
    refreshTableSemaphore = true;*/
    supressTableUpdate = true;
    qDebug() << "TRANSACTIONSWINDOW 1: Start refresh table.";

    QScrollBar *scroll = historyTableView->verticalScrollBar();
    int vScrolPos = scroll->value();
    historyTableView->setVisible(false);

    historyItemModel->clear();

    historyItemModel->setColumnCount(10);
    historyItemModel->setRowCount(0);
    refreshSize();

    historyItemModel->setHeaderData(0, Qt::Horizontal, _tr("Nr"));
    historyItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    historyItemModel->setHeaderData(1, Qt::Horizontal, _tr("Account"));
    historyItemModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    historyItemModel->setHeaderData(2, Qt::Horizontal, "");
    historyItemModel->setHeaderData(2, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    historyItemModel->setHeaderData(3, Qt::Horizontal, _tr("Date"));
    historyItemModel->setHeaderData(3, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
    historyItemModel->setHeaderData(4, Qt::Horizontal, _tr("Amount"));
    historyItemModel->setHeaderData(4, Qt::Horizontal, Qt::AlignRight, Qt::TextAlignmentRole);
    historyItemModel->setHeaderData(5, Qt::Horizontal, _tr("Hash"));
    historyItemModel->setHeaderData(5, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    historyItemModel->setHeaderData(6, Qt::Horizontal, _tr("From"));
    historyItemModel->setHeaderData(6, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    historyItemModel->setHeaderData(7, Qt::Horizontal, _tr("To"));
    historyItemModel->setHeaderData(7, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    historyItemModel->setHeaderData(8, Qt::Horizontal, _tr("Details"));
    historyItemModel->setHeaderData(8, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);

    historyTableView->setStyleSheet(
                                   "QTableView { border: none;"
                                   "background-color: transparent;"
                                   "selection-background-color: transparent}");
#ifdef Q_OS_OSX
    historyTableView->setShowGrid(true);
#else
    historyTableView->setShowGrid(false);
#endif
    historyTableView->verticalHeader()->setVisible(false);
    historyTableView->horizontalHeader()->setSectionsClickable(false);
    historyTableView->horizontalHeader()->setStyleSheet("color: #444");
    historyTableView->horizontalHeader()->setEnabled(false);
    historyTableView->setAlternatingRowColors(true);
    //historyTableView.setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTableView->setAutoScroll(false);
    historyTableView->setModel(historyItemModel);
    historyTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    historyTableView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    scroll = historyTableView->horizontalScrollBar();
    scroll->setVisible(false);
    scroll = historyTableView->verticalScrollBar();
    scroll->setVisible(false);

    //int index = events::getSelectedNameKeyIndex();
    //QPair<QString, QString> pair;
    QStringList dirs;
    cumulatedWallet.clear();
    QStringList accountsList = events::getAccountNameList();
    if(accountsList.count() == 0) {
        return;
    }
    int index = events::getSelectedNameKeyIndex();
    QList<QList<QMap<QString, QString>>> wallet = wallethistory::getAccount(accountsList[index]);
    qDebug() << "TRANSACTIONSWINDOW 2: Start refresh table.";
    for (int cnt = 0; cnt < wallet.count(); cnt++) {
        QList<QMap<QString, QString>> transaction = wallet[wallet.count() - cnt - 1];
        QMap<QString, QString> tmp;
        tmp = transaction[3];
        QString sendAcc = tmp["SendAccountId"];
        tmp = transaction[4];
        QString sendHash = tmp["SendHash"];
        tmp = transaction[5];
        QString recvAcc = tmp["RecvAccountId"];
        tmp = transaction[6];
        QString recvHash = tmp["RecvHash"];
        if(filterLineEdit->text().length() != 0 && !sendAcc.contains(filterLineEdit->text()) && !sendHash.contains(filterLineEdit->text()) && !recvAcc.contains(filterLineEdit->text()) && !recvHash.contains(filterLineEdit->text())) {
            continue;
        }
        tmp = transaction[0];
        QString height = tmp["Height"];
        tmp = transaction[2];
        QString timeStamp = tmp["TimeStamp"];
        tmp = transaction[7];
        QStringList tmpKeys = tmp.keys();
        QString amount;
        QString token;
        //if(tmpKeys.count() > 1 || !tmpKeys.contains("LYR")) {
            //tmpKeys.removeAll("LYR");
        foreach(QString key, tmpKeys) {
            amount += "\n" + textformating::toValue(tmp[key]) + " " + key;
        }
        tmpKeys.removeAll("LYR");
        foreach(QString key, tmpKeys) {
            token = key;
        }
        //} else {
            //amount = textformating::toValue(tmp["LYR"]);
            //token = "LYR";
        //}
        if(amount.length())
            amount.remove(0, 1);
        if(tokenComboBox->currentText().compare(_tr("ALL")) && tokenComboBox->currentText().compare(token)) {
            continue;
        }
        tmp = transaction[1];
        if(txDirectionComboBox->currentIndex() == 1 && tmp["IsReceive"].toInt() == 1) {
            continue;
        } else if(txDirectionComboBox->currentIndex() == 2 && tmp["IsReceive"].toInt() == 0) {
            continue;
        }
        cumulatedWallet.append(transaction);
        if(tmp["IsReceive"].toInt()) {
            dirs.append("R");
        } else {
            dirs.append("S");
        }
        QString note;
        if(transaction.count() >= 10) {
            tmp = transaction[9];
            note = tmp["note"];
        }
        QList<QStandardItem *> item = QList<QStandardItem *>();
        QStandardItem *it;

        it = new QStandardItem(); // Account
        it->setForeground(QBrush(0x909090));
        it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText(height);
        item.append(it);

        it = new QStandardItem(); // Account
        it->setForeground(QBrush(0x909090));
        it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText(accountsList[index]);
        item.append(it);

        it = new QStandardItem(); // Tx Type
        //it->setText("");
        item.append(it);

        /*it = new QStandardItem(); // Token
        it->setForeground(QBrush(0x909090));
        it->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText(token);
        item.append(it);*/

        it = new QStandardItem(); // Date
        it->setForeground(QBrush(0x909090));
        it->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        it->setEnabled(false);
        QDateTime date = QDateTime::fromMSecsSinceEpoch(timeStamp.toLongLong());
        it->setText(date.toString("hh:mm:ss yyyy-MM-dd"));
        item.append(it);

        it = new QStandardItem(); // Amount
        it->setForeground(QBrush(0x909090));
        it->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText(amount);
        item.append(it);

        it = new QStandardItem(); // Hash
        it->setForeground(QBrush(0x0093EE));
        it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText(sendHash + "\n" + recvHash);
        item.append(it);

        it = new QStandardItem(); // From
        it->setForeground(QBrush(0x0093EE));
        it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText(sendAcc);
        item.append(it);

        it = new QStandardItem(); // To
        it->setForeground(QBrush(0x0093EE));
        it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText(recvAcc);
        item.append(it);

        it = new QStandardItem();
        item.append(it);

        historyItemModel->appendRow(item);
    }
    QLabel *dir;
    QPushButton *detailsButton;
    qDebug() << "TRANSACTIONSWINDOW 3: Middle refresh table: " << historyTableView->verticalHeader()->count();
    for( int cnt = 0; cnt < historyTableView->verticalHeader()->count(); cnt++) {
        dir = new QLabel();
        QString txt = dirs[cnt];
        if(!txt.compare("R")) {
            dir->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/receive.png); border: 0px; ");
        } else if(!txt.compare("S")){
            dir->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/send.png); border: 0px; ");
        }
        dir->setScaledContents(true);
        dir->repaint();
        historyTableView->setIndexWidget(historyItemModel->index(cnt, 2), dir);

        detailsButton = new QPushButton();
        detailsButton->setText(_tr("DETAILS"));
        detailsButton->setCursor(Qt::PointingHandCursor);
        detailsButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/cyan.png); border-radius: 6px; border: 1px solid #eee; color: #fff; ");
        detailsButton->installEventFilter(this);
        historyTableView->setIndexWidget(historyItemModel->index(cnt, 8), detailsButton);
    }
    if(vScrolPos <= scroll->maximum()) {
        scroll->setValue(vScrolPos);
    }
    qDebug() << "TRANSACTIONSWINDOW 4: End refresh table.";
    refreshSize();
    refreshFonts();
    historyTableView->setVisible(true);
    //refreshTableSemaphore = false;
    qDebug() << "TRANSACTIONSWINDOW 5: End refresh table.";
    supressTableUpdate = false;
}

void transitionswindow::refreshFonts() {
    qDebug() << "TRANSACTIONSWINDOW 6: Start refresh fonts.";
    txDirectionLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    txDirectionComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));

    tokenLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    tokenComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));

    filterLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    filterLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));

    historyTableView->horizontalHeader()->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    QStandardItem *tmp;
    for( int cnt = 0; cnt < historyTableView->verticalHeader()->count(); cnt++) {
        tmp = historyItemModel->itemFromIndex(historyItemModel->index(cnt, 0));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = historyItemModel->itemFromIndex(historyItemModel->index(cnt, 1));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = historyItemModel->itemFromIndex(historyItemModel->index(cnt, 3));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = historyItemModel->itemFromIndex(historyItemModel->index(cnt, 4));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = historyItemModel->itemFromIndex(historyItemModel->index(cnt, 5));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = historyItemModel->itemFromIndex(historyItemModel->index(cnt, 6));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = historyItemModel->itemFromIndex(historyItemModel->index(cnt, 7));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        QPushButton *detailsButton = (QPushButton *)historyTableView->indexWidget(historyItemModel->index(cnt, 8));
        if(detailsButton != 0) {
            detailsButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));
        }
    }
    qDebug() << "TRANSACTIONSWINDOW 7: End refresh fonts.";
}

void transitionswindow::refreshSize() {
    qDebug() << "TRANSACTIONSWINDOW 8: Start refresh size.";
    txDirectionLabel->setGeometry(s(50), s(30), s(130), s(39));
    txDirectionComboBox->setGeometry(s(50), s(80), s(200), s(39));

    tokenLabel->setGeometry(s(325), s(30), s(130), s(39));
    tokenComboBox->setGeometry(s(325), s(80), s(200), s(39));

    filterLabel->setGeometry(s(600), s(30), s(100), s(39));
    filterLineEdit->setGeometry(s(600), s(80), s(450), s(39));

    historyTableView->setGeometry(s(45), s(200), s(1015), s(620));
    historyTableView->setColumnWidth(0, s(35));
    historyTableView->setColumnWidth(1, s(180));
    historyTableView->setColumnWidth(2, s(30));
    historyTableView->setColumnWidth(3, s(150));
    historyTableView->setColumnWidth(4, s(240));
    historyTableView->setColumnWidth(5, s(100));
    historyTableView->setColumnWidth(6, s(100));
    historyTableView->setColumnWidth(7, s(100));
    historyTableView->setColumnWidth(8, s(80));
    for( int cnt = 0; cnt < historyTableView->verticalHeader()->count(); cnt++) {
        historyTableView->setRowHeight(cnt, s(30));
        QLabel *directionLabel = (QLabel *)historyTableView->indexWidget(historyItemModel->index(cnt, 8));
        if(directionLabel != 0) {
            directionLabel->repaint();
        }
    }
    /*
     * Due to an issue with the table in QT we nee to repeat the dimension setup.
     */
    refreshFonts();
    historyTableView->repaint();
    historyTableView->setGeometry(s(45), s(200), s(1015), s(620));
    historyTableView->setColumnWidth(0, s(35));
    historyTableView->setColumnWidth(1, s(180));
    historyTableView->setColumnWidth(2, s(30));
    historyTableView->setColumnWidth(3, s(150));
    historyTableView->setColumnWidth(4, s(240));
    historyTableView->setColumnWidth(5, s(100));
    historyTableView->setColumnWidth(6, s(100));
    historyTableView->setColumnWidth(7, s(100));
    historyTableView->setColumnWidth(8, s(80));
    for( int cnt = 0; cnt < historyTableView->verticalHeader()->count(); cnt++) {
        historyTableView->setRowHeight(cnt, s(30));
        QLabel *directionLabel = (QLabel *)historyTableView->indexWidget(historyItemModel->index(cnt, 8));
        if(directionLabel != 0) {
            directionLabel->repaint();
        }
    }

    refreshFonts();
    historyTableView->repaint();
    qDebug() << "TRANSACTIONSWINDOW 9: End refresh size.";
}

void transitionswindow::refreshLanguage() {
    qDebug() << "TRANSACTIONSWINDOW 10: Start refresh language.";
    disconnect(txDirectionComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_TxDirection_Changed(const QString &)));
    disconnect(tokenComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Token_Changed(const QString &)));
    txDirectionLabel->setText(_tr("DIRECTION") + ":");
    int tmp = txDirectionComboBox->currentIndex();
    if(tmp < 0) {
        tmp = 0;
    }
    txDirectionComboBox->clear();
    txDirectionComboBox->addItems({_tr("ALL"), _tr("SENT"), _tr("RECEIVED")});
    txDirectionComboBox->setCurrentIndex(tmp);

    tokenLabel->setText(_tr("TOKEN") + ":");
    tmp = tokenComboBox->currentIndex();
    tokenComboBox->clear();
    QList<QStringList> assetList = events::getAssets();
    tokenComboBox->addItem(_tr("ALL"));
    foreach(QStringList asset, assetList) {
        tokenComboBox->addItem(asset[0]);
    }
    if(tmp < tokenComboBox->count()) {
        if(tmp < 0) {
            tmp = 0;
        }
        tokenComboBox->setCurrentIndex(tmp);
    }

    filterLabel->setText(_tr("FILTER") + ":");
    filterLineEdit->setPlaceholderText(_tr("ID, Hash"));

    refreshFonts();
    connect(txDirectionComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_TxDirection_Changed(const QString &)));
    connect(tokenComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Token_Changed(const QString &)));
    qDebug() << "TRANSACTIONSWINDOW 11: End refresh language.";
}

void transitionswindow::setState(state_e state) {
    currentState = state;
}

void transitionswindow::run() {
    if(pastState != currentState) {
        pastState = currentState;
        if(currentState == STATE_TRANSITIONS) {
            mdiAreaTransitions->setVisible(true);
            windowTransitions->setVisible(true);
        } else {
            mdiAreaTransitions->setVisible(false);
            windowTransitions->setVisible(false);
            showDetailsWindow->setDetailsVisible(false);
        }
    }
    if(pastScale != events::getScale()) {
        pastScale = events::getScale();
        mdiAreaTransitions->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowTransitions->setGeometry(0, 0, mdiAreaTransitions->width(), mdiAreaTransitions->height());
        refreshSize();
    }
    if(pastLanguage.compare(translate::getCurrentLang())) {
        pastLanguage = translate::getCurrentLang();
        refreshLanguage();
    }
    if(network != events::getRpcNetwork() || selectedNameKeyIndex != events::getSelectedNameKeyIndex() || events::getUpdateHistory() || recentTransactionsCnt != events::getRecentTransactionsModifyedCnt()) {
        network = events::getRpcNetwork();
        recentTransactionsCnt = events::getRecentTransactionsModifyedCnt();
        selectedNameKeyIndex = events::getSelectedNameKeyIndex();
        refreshLanguage();
        refreshTable();
    }
    if(assetsListModifyedCnt != events::getAssetsModifyedCnt()) {
        assetsListModifyedCnt = events::getAssetsModifyedCnt();
        refreshLanguage();
    }
    showDetailsWindow->run();
}

bool transitionswindow::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonRelease) {
        if(mouseEvent->button() == Qt::LeftButton) {
            for( int cnt = 0; cnt < historyTableView->verticalHeader()->count(); cnt++) {
                QPushButton *details = (QPushButton *)historyTableView->indexWidget(historyItemModel->index(cnt, 8));
                if (obj == details) {
                    QScrollBar *scroll = historyTableView->verticalScrollBar();
                    int vScrolPos = scroll->value();
                    QList<QMap<QString, QString>> transaction = cumulatedWallet[cnt];
                    showDetailsWindow->show(transaction);
                    refreshLanguage();
                    refreshTable();
                    if(vScrolPos <= scroll->maximum()) {
                        scroll->setValue(vScrolPos);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

void transitionswindow::on_TxDirection_Changed(const QString &) {
    refreshTable();
}

void transitionswindow::on_Token_Changed(const QString &) {
    refreshTable();
}

void transitionswindow::on_Filter_Changed(const QString &) {
    refreshTable();
}
