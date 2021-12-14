#include "stake.h"

#include <QMouseEvent>
#include <QHeaderView>
#include <QDateTime>
#include <QTime>
#include <QDate>
#include <QMessageBox>

#include "configlyra.h"
#include "wallet/check.h"
#include "wallet/rpc/profiting.h"
#include "crypto/signatures.h"
#include "pages/pc/textformating.h"

#include "language/translate.h"

#define s(s) _scale(s)

stake::stake() {

}

stake::~stake() {
    delete thisMdiArea;
}

void stake::init(QMdiArea *mdiArea, int accCnt) {
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
    refreshButton = new QPushButton(thisMdiArea);

    accName = new QLineEdit(thisMdiArea);
    daysToStake = new QLineEdit(thisMdiArea);
    voteId = new QLineEdit(thisMdiArea);

    availableTokens = new QLabel(thisMdiArea);
    tokenAmount = new QLineEdit(thisMdiArea);
    stakeOkButton = new QPushButton(thisMdiArea);

    okButton = new QPushButton(thisMdiArea);
    closeButton = new QPushButton(thisMdiArea);

    titleName->setStyleSheet("color: #555;");
    titleName->setAlignment(Qt::AlignLeft);
    titleName->setAttribute(Qt::WA_TranslucentBackground, true);

    refreshButton->setCursor(Qt::PointingHandCursor);
    refreshButton->setFlat(true);
    refreshButton->setStyleSheet("background-color: " BUTON_COLOR_GREEN "; border-radius: " + QString::number((int)s(19)) + "px; border: 1px solid #eee; color: #fff; ");
    connect(refreshButton, SIGNAL(clicked()),this, SLOT(on_Refresh_ButtonPressed()));


    accName->setAttribute(Qt::WA_TranslucentBackground, true);
    accName->setAlignment(Qt::AlignCenter);
    accName->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(19)) + "px;"
                                                    ";}");

    daysToStake->setAttribute(Qt::WA_TranslucentBackground, true);
    daysToStake->setAlignment(Qt::AlignCenter);
    daysToStake->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(19)) + "px;"
                                                    ";}");

    voteId->setAttribute(Qt::WA_TranslucentBackground, true);
    voteId->setAlignment(Qt::AlignCenter);
    voteId->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(19)) + "px;"
                                                    ";}");

    availableTokens->setStyleSheet("color: " COLOR_GREY_DARK ";");
    availableTokens->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    availableTokens->setAttribute(Qt::WA_TranslucentBackground, true);
    availableTokens->setVisible(true);

    tokenAmount->setAttribute(Qt::WA_TranslucentBackground, true);
    tokenAmount->setAlignment(Qt::AlignCenter);
    tokenAmount->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(19)) + "px;"
                                                    ";}");

    stakeOkButton->setCursor(Qt::PointingHandCursor);
    stakeOkButton->setFlat(true);
    stakeOkButton->setStyleSheet("background-color: " BUTON_COLOR_GREEN "; border-radius: " + QString::number((int)s(19)) + "px; border: 1px solid #eee; color: #fff; ");
    connect(stakeOkButton, SIGNAL(clicked()),this, SLOT(on_StakeOk_ButtonPressed()));


    stakingAccTableView = new QTableView(thisMdiArea);
    stakingAccItemModel = new QStandardItemModel();

    stakingAccTableView->setStyleSheet(
                                   "QTableView { border: none;"
                                   "background-color: transparent;"
                                   "selection-background-color: transparent}");
    stakingAccTableView->setShowGrid(false);
    stakingAccTableView->verticalHeader()->setVisible(false);
    stakingAccTableView->horizontalHeader()->setSectionsClickable(false);
    stakingAccTableView->horizontalHeader()->setStyleSheet("color: #777");
    stakingAccTableView->horizontalHeader()->setEnabled(false);
    stakingAccTableView->setAlternatingRowColors(true);
    stakingAccTableView->setModel(stakingAccItemModel);
    stakingAccTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    stakingAccTableView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setFlat(true);
    closeButton->setStyleSheet("background-color: " BUTON_COLOR_GREEN "; border-radius: " + QString::number((int)s(19)) + "px; border: 1px solid #eee; color: #fff; ");
    connect(closeButton, SIGNAL(clicked()),this, SLOT(on_Close_ButtonPressed()));

    okButton->setCursor(Qt::PointingHandCursor);
    okButton->setFlat(true);
    okButton->setStyleSheet("background-color: " BUTON_COLOR_GREEN "; border-radius: " + QString::number((int)s(19)) + "px; border: 1px solid #eee; color: #fff; ");
    connect(okButton, SIGNAL(clicked()),this, SLOT(on_Ok_ButtonPressed()));

    refreshSize();
    refreshLanguage();

    accName->setVisible(false);
    daysToStake->setVisible(false);
    voteId->setVisible(false);
    okButton->setVisible(false);

    availableTokens->setVisible(false);
    tokenAmount->setVisible(false);
    stakeOkButton->setVisible(false);
}

void stake::refreshFonts() {
    titleName->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));
    refreshButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    accName->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    daysToStake->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    voteId->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    availableTokens->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    tokenAmount->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    stakeOkButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    stakingAccTableView->horizontalHeader()->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    QStandardItem *tmp;
    for(int cnt = 0;  cnt < stakingAccTableView->verticalHeader()->count(); cnt++) {
        tmp = stakingAccItemModel->itemFromIndex(stakingAccItemModel->index(cnt, 0));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        tmp = stakingAccItemModel->itemFromIndex(stakingAccItemModel->index(cnt, 1));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        tmp = stakingAccItemModel->itemFromIndex(stakingAccItemModel->index(cnt, 2));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        tmp = stakingAccItemModel->itemFromIndex(stakingAccItemModel->index(cnt, 3));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        tmp = stakingAccItemModel->itemFromIndex(stakingAccItemModel->index(cnt, 4));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        tmp = stakingAccItemModel->itemFromIndex(stakingAccItemModel->index(cnt, 5));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        QPushButton *viewDetails = (QPushButton *)stakingAccTableView->indexWidget(stakingAccItemModel->index(cnt, 5));
        viewDetails->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        if(cnt < stakingAccTableView->verticalHeader()->count() - 1) {
            viewDetails = (QPushButton *)stakingAccTableView->indexWidget(stakingAccItemModel->index(cnt, 6));
            viewDetails->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        }
    }

    okButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    closeButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
}

void stake::refreshSize() {
    titleName->setGeometry(s(28), s(15), s(370), s(30));
    refreshButton->setGeometry(s(959), s(15), s(120), s(40));

    accName->setGeometry(s(328), s(65), s(340), s(40));
    daysToStake->setGeometry(s(678), s(65), s(100), s(40));
    voteId->setGeometry(s(328), s(115), s(450), s(40));

    availableTokens->setGeometry(s(328), s(65), s(440), s(40));
    tokenAmount->setGeometry(s(328), s(115), s(440), s(40));
    stakeOkButton->setGeometry(s(328), s(165), s(440), s(40));

    stakingAccTableView->setGeometry(s(25), s(65), s(1050), s(730));
    stakingAccTableView->setColumnWidth(0, s(150));
    stakingAccTableView->setColumnWidth(1, s(205));
    stakingAccTableView->setColumnWidth(2, s(205));
    stakingAccTableView->setColumnWidth(3, s(120));
    stakingAccTableView->setColumnWidth(4, s(110));
    stakingAccTableView->setColumnWidth(5, s(110));
    stakingAccTableView->setColumnWidth(6, s(110));
    stakingAccTableView->setColumnWidth(7, s(34));
    for( int cnt = 0; cnt < stakingAccTableView->verticalHeader()->count(); cnt++) {
        stakingAccTableView->setRowHeight(cnt, s(34));
    }
    refreshFonts();
    /*
     * Due to an issue with the table in QT we nee to repeat the dimension setup.
     */
    stakingAccTableView->setGeometry(s(25), s(65), s(1050), s(730));
    stakingAccTableView->setColumnWidth(0, s(150));
    stakingAccTableView->setColumnWidth(1, s(205));
    stakingAccTableView->setColumnWidth(2, s(205));
    stakingAccTableView->setColumnWidth(3, s(120));
    stakingAccTableView->setColumnWidth(4, s(110));
    stakingAccTableView->setColumnWidth(5, s(110));
    stakingAccTableView->setColumnWidth(6, s(110));
    stakingAccTableView->setColumnWidth(7, s(34));
    for( int cnt = 0; cnt < stakingAccTableView->verticalHeader()->count(); cnt++) {
        stakingAccTableView->setRowHeight(cnt, s(40));
    }

    refreshFonts();
    stakingAccTableView->repaint();

    okButton->setGeometry(s(328), s(165), s(470), s(40));
    closeButton->setGeometry(s(328), s(790), s(470), s(40));
    refreshFonts();
}

void stake::refreshLanguage() {
    titleName->setText("Stake");
    refreshButton->setText(_tr("REFRESH"));

    accName->setPlaceholderText(_tr("Account name"));
    daysToStake->setPlaceholderText(_tr("Days to stake"));
    voteId->setPlaceholderText(_tr("Stake to account ID"));

    tokenAmount->setPlaceholderText(_tr("Amount"));
    stakeOkButton->setText(_tr("STAKE"));

    stakingAccItemModel->setHeaderData(0, Qt::Horizontal, _tr("Name"));
    stakingAccItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    stakingAccItemModel->setHeaderData(1, Qt::Horizontal, _tr("Staking account ID"));
    stakingAccItemModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    stakingAccItemModel->setHeaderData(2, Qt::Horizontal, _tr("Profiting account ID"));
    stakingAccItemModel->setHeaderData(2, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    stakingAccItemModel->setHeaderData(3, Qt::Horizontal, _tr("Expiry date"));
    stakingAccItemModel->setHeaderData(3, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    stakingAccItemModel->setHeaderData(4, Qt::Horizontal, _tr("Amount"));
    stakingAccItemModel->setHeaderData(4, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    stakingAccItemModel->setHeaderData(5, Qt::Horizontal, "");
    stakingAccItemModel->setHeaderData(5, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    stakingAccItemModel->setHeaderData(6, Qt::Horizontal, "");
    stakingAccItemModel->setHeaderData(6, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    stakingAccItemModel->setHeaderData(7, Qt::Horizontal, "Active");
    stakingAccItemModel->setHeaderData(7, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    okButton->setText(_tr("CREATE STAKING ACCOUNT"));
    closeButton->setText(_tr("CLOSE"));

    refreshFonts();
}

void stake::refreshStakingTable() {
    stakingAccItemModel->clear();
    QString id = events::getAccountId(this->accCnt);
    if(!id.length())
        return;
    QString response = profiting::getBrokerAccounts(this->accCnt);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject result = jsonObject["result"].toObject();
    QDateTime now = QDateTime::currentDateTime();
    qDebug() << now.currentDateTimeUtc().toString();
    int64_t timeNow = now.currentDateTimeUtc().toMSecsSinceEpoch();
    //QDateTime now = QDateTime::currentDateTime();
    if(!result["owner"].toString().compare(id)) {
        QJsonArray stakings = result["stakings"].toArray();
        QStandardItem *tmp;
        QJsonObject obj = QJsonValue(stakings[0]).toObject();
        QLabel *status;
        foreach(const QJsonValue & value, stakings) {
            QJsonObject obj = value.toObject();
            QDateTime endDate = QDateTime::fromString(obj["start"].toString(), Qt::ISODateWithMs);
            QDateTime date = endDate.addDays(obj["days"].toVariant().toLongLong());

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
            tmp->setText(obj["stkid"].toString());
            item.append(tmp);

            tmp = new QStandardItem();
            tmp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            tmp->setForeground(QBrush(0x909090));
            tmp->setEnabled(false);
            tmp->setSelectable(false);
            tmp->setText(obj["voting"].toString());
            item.append(tmp);

            tmp= new QStandardItem();
            tmp->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            tmp->setForeground(QBrush(0x909090));
            tmp->setEnabled(false);
            tmp->setSelectable(false);
            tmp->setText(date.toString());
            item.append(tmp);

            tmp = new QStandardItem();
            tmp->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            tmp->setForeground(QBrush(0x909090));
            tmp->setEnabled(false);
            tmp->setSelectable(false);
            tmp->setText(textformating::toValue(obj["amount"].toDouble()));
            item.append(tmp);

            tmp = new QStandardItem();
            item.append(tmp);
            tmp = new QStandardItem();
            item.append(tmp);
            tmp = new QStandardItem();
            item.append(tmp);

            stakingAccItemModel->appendRow(item);

            status = new QLabel();
            if(date.toMSecsSinceEpoch() > timeNow)
                status->setStyleSheet("image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border: 5px; ");
            else
                status->setStyleSheet("image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border: 5px; ");
            status->setScaledContents(true);
            status->repaint();
            stakingAccTableView->setIndexWidget(stakingAccItemModel->index(stakingAccItemModel->rowCount() - 1, 7), status);
        }
        for( int cnt = 0; cnt < stakingAccTableView->verticalHeader()->count(); cnt++) {
        }

        QPushButton *stakeButton;
        for( int cnt = 0; cnt < stakingAccTableView->verticalHeader()->count(); cnt++) {
            stakeButton = new QPushButton();
            stakeButton->setText(_tr("ADD STAKING"));
            stakeButton->setCursor(Qt::PointingHandCursor);
            stakeButton->setStyleSheet("background-color: " BUTON_COLOR_CYAN "; border-radius: " + QString::number((int)s(16)) + "px; border: " + QString::number((int)s(6)) + "px solid #fff; color: #fff; ");
            stakeButton->installEventFilter(this);
            stakingAccTableView->setIndexWidget(stakingAccItemModel->index(cnt, 5), stakeButton);
        }
        QPushButton *unstakeButton;
        for( int cnt = 0; cnt < stakingAccTableView->verticalHeader()->count(); cnt++) {
            unstakeButton = new QPushButton();
            unstakeButton->setText(_tr("UNSTAKE"));
            unstakeButton->setCursor(Qt::PointingHandCursor);
            unstakeButton->setStyleSheet("background-color: " BUTON_COLOR_RED "; border-radius: " + QString::number((int)s(16)) + "px; border: " + QString::number((int)s(6)) + "px solid #fff; color: #fff; ");
            unstakeButton->installEventFilter(this);
            stakingAccTableView->setIndexWidget(stakingAccItemModel->index(cnt, 6), unstakeButton);
        }
        refreshSize();
        refreshLanguage();
    }
    QList<QStandardItem *> item = QList<QStandardItem *>();
    QStandardItem *it;
    it = new QStandardItem();
    it->setEnabled(false);
    item.append(it);
    it = new QStandardItem();
    it->setEnabled(false);
    item.append(it);
    it = new QStandardItem();
    it->setEnabled(false);
    item.append(it);
    it = new QStandardItem();
    it->setEnabled(false);
    item.append(it);
    it = new QStandardItem();
    it->setEnabled(false);
    item.append(it);
    it = new QStandardItem();
    it->setEnabled(true);
    item.append(it);
    it = new QStandardItem();
    it->setEnabled(false);
    item.append(it);
    stakingAccItemModel->appendRow(item);
    QPushButton *addButton;
    addButton = new QPushButton();
    addButton->setText(_tr("NEW STAKE ACC"));
    addButton->setCursor(Qt::PointingHandCursor);
    addButton->setStyleSheet("background-color: " BUTON_COLOR_GREEN "; border-radius: " + QString::number((int)s(16)) + "px; border: " + QString::number((int)s(6)) + "px solid #fff; color: #fff; ");
    connect(addButton, SIGNAL(clicked()),this, SLOT(createStakingAccount()));
    stakingAccTableView->setIndexWidget(stakingAccItemModel->index(stakingAccTableView->verticalHeader()->count() - 1, 5), addButton);

    refreshSize();
    refreshLanguage();
}

stake::runMode_e stake::getState() {
    return currentMode;
}

void stake::setState(runMode_e state) {
    currentMode = state;
}

void stake::run() {
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
    if(pastMode != currentMode) {
        if(currentMode == runMode_e::RUN) {
            thisMdiArea->setVisible(true);
            thisWindow->setVisible(true);
            refreshStakingTable();
        } else if(currentMode == runMode_e::NONE) {
            thisMdiArea->setVisible(false);
            thisWindow->setVisible(false);
        }
        pastMode = currentMode;
    }
    if(network != events::getNetwork()) {
        network = events::getNetwork();
        refreshStakingTable();
    }
    if(pastState != currentState) {
        if(currentState == state_e::STATE_NONE) {
            accName->setVisible(false);
            daysToStake->setVisible(false);
            voteId->setVisible(false);
            okButton->setVisible(false);
            stakingAccTableView->setVisible(true);
            availableTokens->setVisible(false);
            tokenAmount->setVisible(false);
            stakeOkButton->setVisible(false);
            refreshButton->setVisible(true);
        } else if(currentState == state_e::STATE_CREATE_STACK_ACC) {
            accName->setVisible(true);
            daysToStake->setVisible(true);
            voteId->setVisible(true);
            okButton->setVisible(true);
            refreshButton->setVisible(false);
            stakingAccTableView->setVisible(false);
            refreshStakingTable();
        } else if(currentState == state_e::STATE_ADD_STAKING) {
            stakingAccTableView->setVisible(false);
            availableTokens->setVisible(true);
            tokenAmount->setVisible(true);
            stakeOkButton->setVisible(true);
            refreshButton->setVisible(false);

        }
        pastState = currentState;
    }
}

void stake::createStakingAccount() {
    currentState = state_e::STATE_CREATE_STACK_ACC;
}

void stake::on_Ok_ButtonPressed() {
    if(check::nameSpace(accName->text())) {
        if(daysToStake->text().toInt() >= 3 && daysToStake->text().toInt() < 36500) {
            if(check::accountId(voteId->text())) {
                QMessageBox::StandardButton resBtn = QMessageBox::question( this, this->windowTitle(),
                                                            _tr("Are you sure you want to create this staking account?") + "\n\n" +
                                                                            "Profiting account name:        " + accName->text() + "\n" +
                                                                            "Days to stake:                 " + daysToStake->text() + "\n" +
                                                                            "Stake to this ID:              " + voteId->text() ,
                                                            QMessageBox::No | QMessageBox::Yes,
                                                            QMessageBox::No);
                if (resBtn != QMessageBox::Yes) {
                    return;
                }
                walletErr_e response = profiting::createStakingAcc(this->accCnt, accName->text(), voteId->text(), daysToStake->text().toInt(), true);
                if(response != walletErr_e::WALLET_ERR_OK) {
                    if(response == walletErr_e::WALLET_ERR_NO_FUNDS) {
                        QMessageBox::critical( this, this->windowTitle(),
                                _tr("ERROR: No funds."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
                    } else if(response == walletErr_e::WALLET_ERR_DUPLICATED_NAME){
                        QMessageBox::critical( this, this->windowTitle(),
                                _tr("ERROR: Duplicated name."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
                    } else if(response == walletErr_e::WALLET_ERR_INVALID_BLOCK_TAGS){
                        QMessageBox::critical( this, this->windowTitle(),
                                _tr("ERROR: Invalid block tag."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
                    } else if(response == walletErr_e::WALLET_ERR_INVALID_PROFITING_ACCOUNT){
                        QMessageBox::critical( this, this->windowTitle(),
                                _tr("ERROR: Invalid profiting account."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
                    } else if(response == walletErr_e::WALLET_ERR_CONSENSUS_TIMEOUT){
                        QMessageBox::critical( this, this->windowTitle(),
                                _tr("ERROR: Consensus timeout."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
                    } else {
                        QMessageBox::critical( this, this->windowTitle(),
                                _tr("ERROR: Unknown."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
                    }
                    return;
                }
                accName->clear();
                daysToStake->clear();
                voteId->clear();
                currentState = state_e::STATE_NONE;
                refreshStakingTable();
                return;
            } else {
                QMessageBox::critical( this, this->windowTitle(),
                        _tr("ERROR: Invalid account ID."),
                        QMessageBox::Ok,
                        QMessageBox::Ok);
                return;
            }
        } else {
            QMessageBox::critical( this, this->windowTitle(),
                    _tr("ERROR: Days out of range."),
                    QMessageBox::Ok,
                    QMessageBox::Ok);
            return;
        }
    } else {
        QMessageBox::critical( this, this->windowTitle(),
                _tr("ERROR: Invalid account name."),
                QMessageBox::Ok,
                QMessageBox::Ok);
        return;
    }
}

void stake::on_StakeOk_ButtonPressed() {
    if(tokenAmount->text().toDouble() >= events::getBallance() - 1) {
        QMessageBox::critical( this, this->windowTitle(),
                _tr("ERROR Not enough tokes to stake in this account."),
                QMessageBox::Ok,
                QMessageBox::Ok);
        return;
    }
    if(tokenAmount->text().length() == 0) {
        QMessageBox::critical( this, this->windowTitle(),
                _tr("ERROR Amount can not be empty."),
                QMessageBox::Ok,
                QMessageBox::Ok);
        return;
    }
    if(tokenAmount->text().toDouble() == 0) {
        QMessageBox::critical( this, this->windowTitle(),
                _tr("ERROR Amount can not be zero."),
                QMessageBox::Ok,
                QMessageBox::Ok);
        return;
    }
    QStandardItem *tmp = stakingAccItemModel->itemFromIndex(stakingAccItemModel->index(stakingAccountNr, 1));
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, this->windowTitle(),
                            _tr("Do you want to stake") + " " + textformating::toValue(tokenAmount->text().toDouble()) + " " + _tr("LYR to this account ID:") + "\n" +
                                tmp->text() ,
                            QMessageBox::No | QMessageBox::Yes,
                            QMessageBox::No);
    if (resBtn != QMessageBox::Yes) {
        return;
    }
    if(profiting::addStaking(this->accCnt, tmp->text(), tokenAmount->text().toDouble()) == walletErr_e::WALLET_ERR_OK) {
        QMessageBox::information( this, this->windowTitle(),
                _tr("Staking successfull."),
                QMessageBox::Ok,
                QMessageBox::Ok);
        refreshStakingTable();
        currentState = state_e::STATE_NONE;
    } else {
        QMessageBox::critical( this, this->windowTitle(),
                _tr("ERROR Staking."),
                QMessageBox::Ok,
                QMessageBox::Ok);
    }
}

void stake::on_Refresh_ButtonPressed() {
    refreshStakingTable();
}

void stake::on_Close_ButtonPressed() {
    if(currentState == state_e::STATE_NONE) {
        currentMode = runMode_e::NONE;
    } else {
        currentState = state_e::STATE_NONE;
    }
}

bool stake::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonRelease) {
        if(mouseEvent->button() == Qt::LeftButton) {
            for( int cnt = 0; cnt < stakingAccTableView->verticalHeader()->count(); cnt++) {
                QPushButton *voteButton = (QPushButton *)stakingAccTableView->indexWidget(stakingAccItemModel->index(cnt, 5));
                if (obj == voteButton) {
                    if(cnt <= stakingAccTableView->verticalHeader()->count() - 1) {
                        stakingAccountNr = cnt;
                        currentState = state_e::STATE_ADD_STAKING;
                        availableTokens->setText(_tr("Available tokens:") + " " + textformating::toValue(events::getBallance()) + "LYR");
                    }
                }
                QPushButton *unstakeButton = (QPushButton *)stakingAccTableView->indexWidget(stakingAccItemModel->index(cnt, 6));
                if (obj == unstakeButton) {
                    QMessageBox::StandardButton resBtn = QMessageBox::question( this, this->windowTitle(),
                                            _tr("Are you sure you want to unstake from this account?") + "\n\n" +
                                                _tr("If you unstake before the expiration date") + "\n" +
                                                   "    " + _tr("a") + " " + BREAKING_STAKE_CONTRACT_FEE + "% " + _tr("penalty will be charged."),
                                            QMessageBox::No | QMessageBox::Yes,
                                            QMessageBox::No);
                    if (resBtn != QMessageBox::Yes) {
                        return true;
                    }
                    QStandardItem *tmp;
                    tmp = stakingAccItemModel->itemFromIndex(stakingAccItemModel->index(cnt, 1));
                    if(profiting::unStaking(this->accCnt, tmp->text()) == walletErr_e::WALLET_ERR_OK) {
                        QMessageBox::information( this, this->windowTitle(),
                                _tr("Tokens successfully ustaked."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
                        refreshStakingTable();
                    } else {
                        QMessageBox::critical( this, this->windowTitle(),
                                _tr("ERROR: Unstaking tokens."),
                                QMessageBox::Ok,
                                QMessageBox::Ok);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

