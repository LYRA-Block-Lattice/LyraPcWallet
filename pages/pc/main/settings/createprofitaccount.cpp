#include <QClipboard>
#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QHeaderView>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "createprofitaccount.h"

#include "wallet/events.h"

#include "language/translate.h"
#include "wallet/check.h"
#include "wallet/rpc/profiting.h"
#include "crypto/signatures.h"

#define s(s) _scale(s)

createprofitaccount::createprofitaccount() {

}

createprofitaccount::~createprofitaccount() {
    delete thisMdiArea;
}

void createprofitaccount::init(QMdiArea *mdiArea, int accCnt) {
    this->accCnt = accCnt;

    this->parent = mdiArea;
    this->setParent(mdiArea);
    thisMdiArea = new QMdiArea(mdiArea);
    //thisMdiArea.setParent(mdiArea);
    thisMdiArea->setVisible(false);
    //this->walletName = walletName;

    thisWidget = new QWidget(thisMdiArea);
    thisWidget->show();
    thisWindow = new QMdiSubWindow(thisMdiArea);
    thisWindow->setWidget(thisWidget);
    thisWindow->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    thisWindow->setWindowFlag(Qt::FramelessWindowHint, true);
    thisWindow->setGeometry(0, 0, thisMdiArea->width(), thisMdiArea->height());
    thisWindow->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/whiteBack.png)");
    thisWindow->setVisible(false);

    titleName = new QLabel(thisMdiArea);

    accName = new QLineEdit(thisMdiArea);
    accType = new QComboBox(thisMdiArea);
    shareRatio = new QLineEdit(thisMdiArea);
    seats = new QLineEdit(thisMdiArea);

    okButton = new QPushButton(thisMdiArea);
    cancelButton = new QPushButton(thisMdiArea);

    titleName->setStyleSheet("color: #333;");
    titleName->setAlignment(Qt::AlignCenter);
    titleName->setAttribute(Qt::WA_TranslucentBackground, true);

    accName->setAttribute(Qt::WA_TranslucentBackground, true);
    accName->setAlignment(Qt::AlignCenter);
    accName->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");

    accType->setAutoFillBackground(false);
    //accType->setAlignment(Qt::AlignCenter);
    accType->setStyleSheet(""
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
    accType->addItems({"Node", "Oracle", "Merchant", "Yield"});
    accType->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));
    for (int i = 0 ; i < accType->count() ; ++i) {
        accType->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }
    accType->setCursor(Qt::PointingHandCursor);

    shareRatio->setAttribute(Qt::WA_TranslucentBackground, true);
    shareRatio->setAlignment(Qt::AlignCenter);
    shareRatio->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");

    seats->setAttribute(Qt::WA_TranslucentBackground, true);
    seats->setAlignment(Qt::AlignCenter);
    seats->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");

    cancelButton->setCursor(Qt::PointingHandCursor);
    cancelButton->setFlat(true);
    cancelButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/green.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    connect(cancelButton, SIGNAL(clicked()),this, SLOT(on_Cancel_ButtonPressed()));

    okButton->setCursor(Qt::PointingHandCursor);
    okButton->setFlat(true);
    okButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/orange.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    connect(okButton, SIGNAL(clicked()),this, SLOT(on_Ok_ButtonPressed()));

    profitingAccTableView = new QTableView(thisMdiArea);
    profitingAccItemModel = new QStandardItemModel();

    profitingAccTableView->setStyleSheet(
                                   "QTableView { border: none;"
                                   "background-color: transparent;"
                                   "selection-background-color: transparent}");
    profitingAccTableView->setShowGrid(false);
    profitingAccTableView->verticalHeader()->setVisible(false);
    profitingAccTableView->horizontalHeader()->setSectionsClickable(false);
    profitingAccTableView->horizontalHeader()->setStyleSheet("color: #777");
    profitingAccTableView->horizontalHeader()->setEnabled(false);
    profitingAccTableView->setAlternatingRowColors(true);
    profitingAccTableView->setModel(profitingAccItemModel);
    //profitingAccTableView->setCursor(Qt::PointingHandCursor);
    profitingAccTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    profitingAccTableView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    connect(profitingAccTableView, SIGNAL(clicked(QModelIndex) ),
        this,SLOT(profitingAccItemClicked(QModelIndex) ) );

    refreshSize();
    refreshLanguage();
}

void createprofitaccount::refreshFonts() {
    titleName->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));

    accName->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    accType->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    shareRatio->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    seats->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    cancelButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    okButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    profitingAccTableView->horizontalHeader()->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    QStandardItem *tmp;
    for(int cnt = 0;  cnt < profitingAccTableView->verticalHeader()->count(); cnt++) {
        QPushButton *viewDetails = (QPushButton *)profitingAccTableView->indexWidget(profitingAccItemModel->index(cnt, 5));
        viewDetails->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));
        tmp = profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 0));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 1));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 2));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 3));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 4));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 5));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
    }
}

void createprofitaccount::refreshSize() {
    titleName->setGeometry(s(328), s(10), s(470), s(40));

    accName->setGeometry(s(328), s(65), s(360), s(40));
    accType->setGeometry(s(698), s(65), s(100), s(40));
    shareRatio->setGeometry(s(328), s(115), s(470), s(40));
    seats->setGeometry(s(328), s(165), s(470), s(40));

    cancelButton->setGeometry(s(328), s(215), s(470), s(40));
    okButton->setGeometry(s(328), s(265), s(470), s(40));

    profitingAccTableView->setGeometry(s(55), s(360), s(1010), s(475));
    profitingAccTableView->setColumnWidth(0, s(150));
    profitingAccTableView->setColumnWidth(1, s(70));
    profitingAccTableView->setColumnWidth(2, s(70));
    profitingAccTableView->setColumnWidth(3, s(70));
    profitingAccTableView->setColumnWidth(4, s(530));
    profitingAccTableView->setColumnWidth(5, s(120));
    for( int cnt = 0; cnt < profitingAccTableView->verticalHeader()->count(); cnt++) {
        profitingAccTableView->setRowHeight(cnt, s(34));
    }
    refreshFonts();
    /*
     * Due to an issue with the table in QT we nee to repeat the dimension setup.
     */
    profitingAccTableView->setGeometry(s(55), s(360), s(1010), s(475));
    profitingAccTableView->setColumnWidth(0, s(150));
    profitingAccTableView->setColumnWidth(1, s(70));
    profitingAccTableView->setColumnWidth(2, s(70));
    profitingAccTableView->setColumnWidth(3, s(70));
    profitingAccTableView->setColumnWidth(4, s(530));
    profitingAccTableView->setColumnWidth(5, s(120));
    for( int cnt = 0; cnt < profitingAccTableView->verticalHeader()->count(); cnt++) {
        profitingAccTableView->setRowHeight(cnt, s(34));
    }

    refreshFonts();
    profitingAccTableView->repaint();

    refreshFonts();
}

void createprofitaccount::refreshLanguage() {
    cancelButton->setText(_tr("CLOSE"));
    okButton->setText(_tr("OK"));

    profitingAccItemModel->setHeaderData(0, Qt::Horizontal, _tr("Name"));
    profitingAccItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    profitingAccItemModel->setHeaderData(1, Qt::Horizontal, _tr("Type"));
    profitingAccItemModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    profitingAccItemModel->setHeaderData(2, Qt::Horizontal, _tr("Share ratio"));
    profitingAccItemModel->setHeaderData(2, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    profitingAccItemModel->setHeaderData(3, Qt::Horizontal, _tr("Seats"));
    profitingAccItemModel->setHeaderData(3, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    profitingAccItemModel->setHeaderData(4, Qt::Horizontal, _tr("Profiting ID"));
    profitingAccItemModel->setHeaderData(4, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    profitingAccItemModel->setHeaderData(5, Qt::Horizontal, "");
    profitingAccItemModel->setHeaderData(5, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    refreshFonts();
}

void createprofitaccount::refreshProfitingTable() {
    profitingAccItemModel->clear();
    QList<QPair<QString, QString>> pair = events::getWalletNameKeyList();
    QString id = signatures::getAccountIdFromPrivateKey(pair[this->accCnt].second);
    QString response = profiting::getBrokerAccounts(id);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject result = jsonObject["result"].toObject();
    if(!result["owner"].toString().compare(id)) {
        QJsonArray profits = result["profits"].toArray();
        //res.stakings = jsonObject["stakings"].toArray();
        QStandardItem *tmp;
        foreach(const QJsonValue & value, profits) {
            QJsonObject obj = value.toObject();
            //recentTransactionsTableView->setRowHidden(cnt, true);
            QList<QStandardItem *> item = QList<QStandardItem *>();

            tmp = new QStandardItem();
            tmp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            tmp->setForeground(QBrush(0x909090));
            tmp->setEnabled(false);
            tmp->setSelectable(false);
            tmp->setText(obj["name"].toString());
            item.append(tmp);

            tmp = new QStandardItem();
            tmp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            tmp->setForeground(QBrush(0x909090));
            tmp->setEnabled(false);
            tmp->setSelectable(false);
            tmp->setText(obj["type"].toString());
            item.append(tmp);

            tmp= new QStandardItem();
            tmp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            tmp->setForeground(QBrush(0x909090));
            tmp->setEnabled(false);
            tmp->setSelectable(false);
            tmp->setText(QString::number(obj["shareratio"].toDouble() * 100.0));
            item.append(tmp);

            tmp = new QStandardItem();
            tmp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            tmp->setForeground(QBrush(0x909090));
            tmp->setEnabled(false);
            tmp->setSelectable(false);
            tmp->setText(QString::number(obj["seats"].toInt()));
            item.append(tmp);

            tmp = new QStandardItem();
            tmp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            tmp->setForeground(QBrush(0x909090));
            tmp->setEnabled(true);
            tmp->setSelectable(false);
            tmp->setText(obj["pftid"].toString());
            tmp->setEditable(false);
            item.append(tmp);

            tmp = new QStandardItem();
            item.append(tmp);

            profitingAccItemModel->appendRow(item);
        }
        QPushButton *stakeButton;
        for( int cnt = 0; cnt < profitingAccTableView->verticalHeader()->count(); cnt++) {
            stakeButton = new QPushButton();
            stakeButton->setText(_tr("CREATE DIVIDENDS"));
            stakeButton->setCursor(Qt::PointingHandCursor);
            stakeButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/cyan.png); border-radius: 6px; border: 1px solid #eee; color: #fff; ");
            stakeButton->installEventFilter(this);
            profitingAccTableView->setIndexWidget(profitingAccItemModel->index(cnt, 5), stakeButton);
        }
        refreshSize();
        refreshLanguage();
    }
}

createprofitaccount::runMode_e createprofitaccount::getState() {
    return currentMode;
}

void createprofitaccount::setState(runMode_e state, QString walletName) {
    titleName->setText(walletName);
    accName->setPlaceholderText(_tr("Account name"));
    accType->setPlaceholderText(_tr("Account type"));
    shareRatio->setPlaceholderText(_tr("Share ratio from 0.0% to 100.0%"));
    seats->setPlaceholderText(_tr("Number of seats from 0 to 100"));
    currentMode = state;
}

createprofitaccount::return_e createprofitaccount::run() {
    if(pastMode != currentMode) {
        if(pastScale != events::getScale()) {
            pastScale = events::getScale();
            thisMdiArea->setGeometry(0, 0, parent->width(), parent->height());
            thisWindow->setGeometry(0, 0, thisMdiArea->width(), thisMdiArea->height());
            refreshSize();
        }
        if(pastLanguage.compare(translate::getCurrentLang())) {
            pastLanguage = translate::getCurrentLang();
            refreshLanguage();
        }
        if(currentMode == runMode_e::RUN) {
            thisMdiArea->setVisible(true);
            thisWindow->setVisible(true);
            refreshProfitingTable();
        } else if(currentMode == runMode_e::NONE) {
            accName->clear();
            thisMdiArea->setVisible(false);
            thisWindow->setVisible(false);
        } else if(currentMode == runMode_e::CANCEL) {
            thisMdiArea->setVisible(false);
            thisWindow->setVisible(false);
            currentMode = runMode_e::NONE;
            return return_e::RETURN_CANCEL;
        } else  if(currentMode == runMode_e::OK) {
            thisMdiArea->setVisible(false);
            thisWindow->setVisible(false);
            currentMode = runMode_e::NONE;
            return return_e::RETURN_OK;
        }
        pastMode = currentMode;
    }
    return return_e::RETURN_NONE;
}

void createprofitaccount::on_Cancel_ButtonPressed() {
    currentMode = runMode_e::CANCEL;
}

void createprofitaccount::on_Ok_ButtonPressed() {
    if(this->accCnt == -1) {
        QMessageBox::critical( this, this->windowTitle(),
                _tr("Invalid window initialization."),
                QMessageBox::Ok,
                QMessageBox::Ok);
        return;
    }
    if(check::name(accName->text())) {
        if(shareRatio->text().toDouble() >= 0.0 && shareRatio->text().toDouble() <= 100.0) {
            if(seats->text().toInt() >= 0 && seats->text().toInt() <= 100) {
                QMessageBox::StandardButton resBtn = QMessageBox::question( this, this->windowTitle(),
                                                            _tr("Are you sure you want to create this profiting account?") + "\n\n" +
                                                                            "Profiting account name:        " + accName->text() + "\n" +
                                                                            "Profiting account type:        " + accType->currentText() + "\n" +
                                                                            "Profiting account share ratio: " + shareRatio->text() + "%\n" +
                                                                            "Profiting account seats:       " + seats->text() ,
                                                            QMessageBox::No | QMessageBox::Yes,
                                                            QMessageBox::No);
                if (resBtn != QMessageBox::Yes) {
                    return;
                }
                QList<QPair<QString, QString>> pair = events::getWalletNameKeyList();
                walletErr_e response = profiting::createProfitingAcc(pair[this->accCnt].second, accName->text(), accType->currentText(), shareRatio->text().toDouble(), seats->text().toInt());
                if(response != walletErr_e::WALLET_ERR_OK) {
                    QMessageBox::critical( this, this->windowTitle(),
                            _tr("ERROR creating profiting account."),
                            QMessageBox::Ok,
                            QMessageBox::Ok);
                    return;
                }
                accName->clear();
                shareRatio->clear();
                seats->clear();
                refreshProfitingTable();
                return;
            } else {
                QMessageBox::critical( this, this->windowTitle(),
                        _tr("Invalid seats number."),
                        QMessageBox::Ok,
                        QMessageBox::Ok);
                return;
            }
        } else {
            QMessageBox::critical( this, this->windowTitle(),
                    _tr("Invalid percentage."),
                    QMessageBox::Ok,
                    QMessageBox::Ok);
            return;
        }
    } else {
        QMessageBox::critical( this, this->windowTitle(),
                _tr("Invalid name."),
                QMessageBox::Ok,
                QMessageBox::Ok);
        return;
    }
    currentMode = runMode_e::OK;
}

bool createprofitaccount::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonRelease) {
        if(mouseEvent->button() == Qt::LeftButton) {
            for( int cnt = 0; cnt < profitingAccTableView->verticalHeader()->count(); cnt++) {
                QPushButton *voteButton = (QPushButton *)profitingAccTableView->indexWidget(profitingAccItemModel->index(cnt, 5));
                if (obj == voteButton) {
                    QStandardItem *tmp = profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 4));
                    QMessageBox::StandardButton resBtn = QMessageBox::question( this, this->windowTitle(),
                                            _tr("Do you want to create dividends for") + "?\n" +
                                                profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 0))->text() + "\n" +
                                                profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 1))->text() + "\n" +
                                                tmp->text(),
                                            QMessageBox::No | QMessageBox::Yes,
                                            QMessageBox::No);
                    if (resBtn != QMessageBox::Yes) {
                        return true;
                    }
                    QList<QPair<QString, QString>> pair = events::getWalletNameKeyList();
                    walletErr_e response = profiting::createDividents(pair[this->accCnt].second, profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(cnt, 4))->text());
                    if(response != walletErr_e::WALLET_ERR_OK) {
                        QMessageBox::critical( this, this->windowTitle(),
                                _tr("ERROR creating dividends."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
                    }
                }
            }
        }
    }
    return false;
}

void createprofitaccount::profitingAccItemClicked(QModelIndex idx) {
    if(idx.column() == 4) {
        QStandardItem *tmp = profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(idx.row(), 4));
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(tmp->text());
        QMessageBox::information( this, this->windowTitle(),
                _tr("Account ID copied to clipboard.") + "\n" +
                                  profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(idx.row(), 0))->text() + "\n" +
                                  profitingAccItemModel->itemFromIndex(profitingAccItemModel->index(idx.row(), 1))->text() + "\n" +
                                  tmp->text(),
                QMessageBox::Ok,
                QMessageBox::Ok);
    }
}
