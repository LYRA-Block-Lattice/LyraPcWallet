#include "settingswindow.h"

#include <QMdiSubWindow>
#include <QMouseEvent>
#include <QHeaderView>
#include <QScrollBar>
#include <QDir>
#include <QFileDialog>

#include "language/translate.h"
#include "wallet/events.h"
#include "crypto/signatures.h"
#include "storage/walletfile.h"
#include "pages/pc/populate.h"
#include "wallet/rpc/wallethistory.h"

#define s(s) _scale(s)

settingswindow::settingswindow() {

}

settingswindow::~settingswindow() {

}

void settingswindow::setVars(QMdiSubWindow *window, QWidget *parent) {
    this->parent = parent;
    this->window = window;

    mdiAreaSettings = new QMdiArea(window);
    mdiAreaSettings->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaSettings->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaSettings->setVisible(true);
    mdiAreaSettings->repaint();
    widgetSettings = new QWidget(mdiAreaSettings);
    widgetSettings->show();
    setParent(mdiAreaSettings);
    windowSettings = new QMdiSubWindow(mdiAreaSettings);
    windowSettings->setWidget(widgetSettings);
    windowSettings->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowSettings->setWindowFlag(Qt::FramelessWindowHint, true);
    windowSettings->setGeometry(0, 0, mdiAreaSettings->width(), mdiAreaSettings->height());
    windowSettings->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/settings.png)");
    windowSettings->setVisible(false);

    generalSettingsLabel = new QLabel(mdiAreaSettings);

    languageLabel = new QLabel(mdiAreaSettings);
    languageComboBox = new QComboBox(mdiAreaSettings);

    appStyleLabel = new QLabel(mdiAreaSettings);
    appStyleComboBox = new QComboBox(mdiAreaSettings);

    networkLabel = new QLabel(mdiAreaSettings);
    networkComboBox = new QComboBox(mdiAreaSettings);

    alternativeValueLabel = new QLabel(mdiAreaSettings);
    alternativeValueComboBox = new QComboBox(mdiAreaSettings);

    windowScaleLabel = new QLabel(mdiAreaSettings);
    windowScaleComboBox = new QComboBox(mdiAreaSettings);

    userManagerSettingsLabel = new QLabel(mdiAreaSettings);
    userManagerSettingsBackgroundLabel = new QLabel(mdiAreaSettings);
    usernameLabel = new QLabel(mdiAreaSettings);
    usernameNameLabel = new QLabel(mdiAreaSettings);
    walletSettingsLabel = new QLabel(mdiAreaSettings);
    userPasswordLabel = new QLabel(mdiAreaSettings);
    userPasswordPassLabel = new QLabel(mdiAreaSettings);
    backupButton = new QPushButton(mdiAreaSettings);
    editButton = new QPushButton(mdiAreaSettings);

    accountsTableView = new QTableView(mdiAreaSettings);
    accountsItemModel = new QStandardItemModel();


    generalSettingsLabel->setStyleSheet("color: #555;");
    generalSettingsLabel->setAlignment(Qt::AlignLeft);
    generalSettingsLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    languageLabel->setStyleSheet("color: #555;");
    languageLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    languageLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    languageComboBox->setCurrentIndex(0);
    languageComboBox->setAutoFillBackground(false);
    languageComboBox->setStyleSheet(""
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
    languageComboBox->setCursor(Qt::PointingHandCursor);
    connect(languageComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Language_Changed(const QString &)));


    appStyleLabel->setStyleSheet("color: #555;");
    appStyleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    appStyleLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    appStyleComboBox->setCurrentIndex(0);
    appStyleComboBox->setAutoFillBackground(false);
    appStyleComboBox->setStyleSheet(""
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
    appStyleComboBox->addItems({"light"});
    appStyleComboBox->setCursor(Qt::PointingHandCursor);
    connect(appStyleComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_AppStyle_Changed(const QString &)));


    networkLabel->setStyleSheet("color: #555;");
    networkLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    networkLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    networkComboBox->setCurrentIndex(0);
    networkComboBox->setAutoFillBackground(false);
    networkComboBox->setStyleSheet(""
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
    networkComboBox->addItems({"testnet", "mainnet"});
    networkComboBox->setCursor(Qt::PointingHandCursor);
    connect(networkComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Network_Changed(const QString &)));

    alternativeValueLabel->setStyleSheet("color: #555;");
    alternativeValueLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    alternativeValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    alternativeValueComboBox->setCurrentIndex(0);
    alternativeValueComboBox->setAutoFillBackground(false);
    alternativeValueComboBox->setStyleSheet(""
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
    alternativeValueComboBox->addItems({"BTC", "USD"});
    alternativeValueComboBox->setCursor(Qt::PointingHandCursor);
    connect(alternativeValueComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_AlternativeValue_Changed(const QString &)));

    windowScaleLabel->setStyleSheet("color: #555;");
    windowScaleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    windowScaleLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    windowScaleComboBox->setCurrentIndex(0);
    windowScaleComboBox->setAutoFillBackground(false);
    windowScaleComboBox->setStyleSheet(""
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
    windowScaleComboBox->addItems({"0.7x", "0.8x", "0.9x", "1.0x", "1.1x", "1.2x", "1.3x", "1.4x", "1.5x"});
    windowScaleComboBox->setCursor(Qt::PointingHandCursor);
    connect(windowScaleComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_ScaleValue_Changed(const QString &)));



    userManagerSettingsLabel->setStyleSheet("color: #555;");
    userManagerSettingsLabel->setAlignment(Qt::AlignLeft);
    userManagerSettingsLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    userManagerSettingsBackgroundLabel->setStyleSheet("background-color: #F8F8F8;");
    userManagerSettingsBackgroundLabel->setAlignment(Qt::AlignLeft);


    usernameLabel->setStyleSheet("color: #555;");
    usernameLabel->setAlignment(Qt::AlignLeft);
    usernameLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    usernameNameLabel->setStyleSheet("color: #999;");
    usernameNameLabel->setAlignment(Qt::AlignLeft);
    usernameNameLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    walletSettingsLabel->setStyleSheet("color: #555;");
    walletSettingsLabel->setAlignment(Qt::AlignLeft);
    walletSettingsLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    userPasswordLabel->setStyleSheet("color: #555;");
    userPasswordLabel->setAlignment(Qt::AlignLeft);
    userPasswordLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    userPasswordPassLabel->setStyleSheet("color: #999;");
    userPasswordPassLabel->setAlignment(Qt::AlignLeft);
    userPasswordPassLabel->setAttribute(Qt::WA_TranslucentBackground, true);


    backupButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/blue_b.png); border-radius: 2px; border: 1px solid #eee; color: #fff; ");
    backupButton->setFlat(true);
    backupButton->setCursor(Qt::PointingHandCursor);
    connect(backupButton, SIGNAL(clicked()),this, SLOT(on_Backup_ButtonPressed()));

    editButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/cyan_b.png); border-radius: 2px; border: 1px solid #eee; color: #fff; ");
    editButton->setFlat(true);
    editButton->setCursor(Qt::PointingHandCursor);
    connect(editButton, SIGNAL(clicked()),this, SLOT(on_Edit_ButtonPressed()));


    accountsTableView->setStyleSheet(
                                   "QTableView { border: none; "
                                   "background-color: transparent; "
                                   "selection-background-color: transparent} ");
    accountsTableView->setShowGrid(false);
    accountsTableView->verticalHeader()->setVisible(false);
    accountsTableView->horizontalHeader()->setSectionsClickable(false);
    accountsTableView->horizontalHeader()->setStyleSheet("color: #555; "
                        "QHeaderView::section {"
                        "border: 0px solid black;"
                        "border-bottom: 0px;  "
                    "}");
    accountsTableView->horizontalHeader()->setCascadingSectionResizes(false);
    accountsTableView->horizontalHeader()->setEnabled(false);
    accountsTableView->horizontalHeader()->setAutoFillBackground(false);
    accountsTableView->setAlternatingRowColors(true);
    accountsTableView->setModel(accountsItemModel);


    usernameNameLabel->setVisible(false);
    userPasswordPassLabel->setVisible(false);

    backupButton->setVisible(false);
    editButton->setVisible(false);

    QString tmpLanguage = translate::getCurrentLang();
    QDir dir(":/resource/lang/");
    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot);
    QStringList stringList = dir.entryList();
    foreach(QString lang, stringList){
        languageComboBox->addItem(lang.remove(".txt"));
    }
    languageComboBox->setCurrentText(tmpLanguage);
    translate::loadLanguage(tmpLanguage);
    //translate::loadLanguage("en");

    updateWalletSettingsTable();
    refreshSize();
    refreshLanguage();

    addAccountWindow = new settingsaddaccount();
    addAccountWindow->init(mdiAreaSettings);
    settingsDeleteAccount = new settingsdeleteaccount();
    settingsDeleteAccount->init(mdiAreaSettings);
    settingsEditAccount = new settingseditaccount();
    settingsEditAccount->init(mdiAreaSettings);
    settingsEditUser = new settingsedituser();
    settingsEditUser->init(mdiAreaSettings);
    settingsShowPrivKey = new settingsshowprivkey();
    settingsShowPrivKey->init(mdiAreaSettings);
}

void settingswindow::updateWalletSettingsTable() {
    QList<QPair<QString, QString>> accountList = events::getWalletNameKeyList();
    accountsItemModel->clear();
    accountsItemModel->setColumnCount(7);
    accountsItemModel->setRowCount(0);

    accountsItemModel->setHeaderData(0, Qt::Horizontal, _tr("Wallet ID"));
    accountsItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(1, Qt::Horizontal, _tr("Voted for"));
    accountsItemModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(2, Qt::Horizontal, _tr("Wallet name"));
    accountsItemModel->setHeaderData(2, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(3, Qt::Horizontal, "");
    accountsItemModel->setHeaderData(3, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(4, Qt::Horizontal, "");
    accountsItemModel->setHeaderData(4, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(5, Qt::Horizontal, "");
    accountsItemModel->setHeaderData(5, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(6, Qt::Horizontal, "");
    accountsItemModel->setHeaderData(6, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    QScrollBar *scroll = accountsTableView->horizontalScrollBar();
    scroll->setVisible(false);
    scroll = accountsTableView->verticalScrollBar();
    scroll->setVisible(false);
    QPair<QString, QString>pair;
    foreach(pair, accountList) {
        QList<QStandardItem *> item = QList<QStandardItem *>();
        QStandardItem *it;
        it = new QStandardItem();
        it->setForeground(QBrush(0x909090));
        it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText(signatures::getAccountIdFromPrivateKey(pair.second));
        item.append(it);
        it = new QStandardItem();
        it->setForeground(QBrush(0x909090));
        it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText("");
        item.append(it);
        it = new QStandardItem();
        it->setForeground(QBrush(0x909090));
        it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        it->setEnabled(false);
        it->setText(pair.first);
        item.append(it);

        it = new QStandardItem();
        item.append(it);
        it = new QStandardItem();
        item.append(it);
        it = new QStandardItem();
        item.append(it);
        it = new QStandardItem();
        item.append(it);

        accountsItemModel->appendRow(item);
    }
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *voteForButton;
    QPushButton *showPrivateKeyButton;
    for( int cnt = 0; cnt < accountsTableView->verticalHeader()->count(); cnt++) {
        voteForButton = new QPushButton();
        voteForButton->setText(_tr("VOTE"));
        voteForButton->setCursor(Qt::PointingHandCursor);
        voteForButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/green.png); border-radius: 6px; border: 1px solid #eee; color: #fff; ");
        voteForButton->installEventFilter(this);
        //accountsTableView.setIndexWidget(accountsItemModel.index(cnt, 3), voteForButton);

        editButton = new QPushButton();
        editButton->setText(_tr("EDIT"));
        editButton->setCursor(Qt::PointingHandCursor);
        editButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/cyan.png); border-radius: 6px; border: 1px solid #eee; color: #fff; ");
        editButton->installEventFilter(this);
        accountsTableView->setIndexWidget(accountsItemModel->index(cnt, 4), editButton);

        deleteButton = new QPushButton();
        deleteButton->setText(_tr("DELETE"));
        deleteButton->setCursor(Qt::PointingHandCursor);
        deleteButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/red.png); border-radius: 6px; border: 1px solid #eee; color: #fff; ");
        deleteButton->installEventFilter(this);
        accountsTableView->setIndexWidget(accountsItemModel->index(cnt, 5), deleteButton);

        showPrivateKeyButton = new QPushButton();
        showPrivateKeyButton->setText(_tr("SHOW KEY"));
        showPrivateKeyButton->setCursor(Qt::PointingHandCursor);
        showPrivateKeyButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/blue.png); border-radius: 6px; border: 1px solid #eee; color: #fff; ");
        showPrivateKeyButton->installEventFilter(this);
        accountsTableView->setIndexWidget(accountsItemModel->index(cnt, 6), showPrivateKeyButton);

    }
    if(events::getWalletLoaded()) {
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
        it->setEnabled(false);
        item.append(it);
        it = new QStandardItem();
        it->setEnabled(false);
        item.append(it);
        accountsItemModel->appendRow(item);
        QPushButton *addButton;
        addButton = new QPushButton();
        addButton->setText(_tr("ADD"));
        addButton->setCursor(Qt::PointingHandCursor);
        addButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/green.png); border-radius: 6px; border: 1px solid #eee; color: #fff; ");
        connect(addButton, SIGNAL(clicked()),this, SLOT(on_Add_ButtonPressed()));
        accountsTableView->setIndexWidget(accountsItemModel->index(accountsTableView->verticalHeader()->count() - 1, 6), addButton);
    }


    refreshFonts();
    refreshSize();

}

void settingswindow::refreshFonts() {
    generalSettingsLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));

    languageLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    languageComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));

    appStyleLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    appStyleComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));

    networkLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    networkComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));

    alternativeValueLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    alternativeValueComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));

    windowScaleLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    windowScaleComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));

    userManagerSettingsLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));
    usernameLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    usernameNameLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    walletSettingsLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));
    userPasswordLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    userPasswordPassLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    backupButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));
    editButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));


    accountsTableView->horizontalHeader()->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    for( int cnt = 0; cnt < accountsTableView->verticalHeader()->count(); cnt++) {
        QStandardItem *tmp = accountsItemModel->itemFromIndex(accountsItemModel->index(cnt, 0));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = accountsItemModel->itemFromIndex(accountsItemModel->index(cnt, 1));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        tmp = accountsItemModel->itemFromIndex(accountsItemModel->index(cnt, 2));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        QPushButton *voteButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 3));
        if(voteButton != 0)
            voteButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));
        QPushButton *editButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 4));
        if(editButton != 0)
            editButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));
        QPushButton *deleteButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 5));
        if(deleteButton != 0)
            deleteButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));
        QPushButton *showPrivKeyButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 6));
        if(showPrivKeyButton != 0)
            showPrivKeyButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.5)));
    }
}

void settingswindow::refreshSize() {
    generalSettingsLabel->setGeometry(s(28), s(15), s(370), s(39));

    languageLabel->setGeometry(s(0), s(52), s(150), s(39));
    languageComboBox->setGeometry(s(150), s(52), s(100), s(39));

    appStyleLabel->setGeometry(s(0), s(100), s(150), s(39));
    appStyleComboBox->setGeometry(s(150), s(100), s(100), s(39));

    networkLabel->setGeometry(s(260), s(52), s(230), s(39));
    networkComboBox->setGeometry(s(490), s(52), s(120), s(39));

    alternativeValueLabel->setGeometry(s(260), s(100), s(230), s(39));
    alternativeValueComboBox->setGeometry(s(490), s(100), s(120), s(39));

    windowScaleLabel->setGeometry(s(610), s(52), s(160), s(39));
    windowScaleComboBox->setGeometry(s(770), s(52), s(100), s(39));

    userManagerSettingsLabel->setGeometry(s(28), s(165), s(370), s(39));
    userManagerSettingsBackgroundLabel->setGeometry(s(50), s(258), s(1015), s(39));

    usernameLabel->setGeometry(s(52), s(215), s(370), s(39));
    usernameNameLabel->setGeometry(s(52), s(270), s(370), s(39));

    walletSettingsLabel->setGeometry(s(28), s(338), s(370), s(39));

    userPasswordLabel->setGeometry(s(450), s(215), s(370), s(39));
    userPasswordPassLabel->setGeometry(s(450), s(270), s(370), s(39));

    backupButton->setGeometry(s(910), s(264), s(58), s(27));
    editButton->setGeometry(s(1000), s(264), s(58), s(27));


    accountsTableView->setGeometry(s(50), s(401), s(1035), s(423));
    accountsTableView->setColumnWidth(0, s(255));
    accountsTableView->setColumnWidth(1, s(255));
    accountsTableView->setColumnWidth(2, s(150));
    accountsTableView->setColumnWidth(3, s(90));
    accountsTableView->setColumnWidth(4, s(90));
    accountsTableView->setColumnWidth(5, s(90));
    accountsTableView->setColumnWidth(6, s(90));
    for( int cnt = 0; cnt < accountsTableView->verticalHeader()->count(); cnt++) {
        accountsTableView->setRowHeight(cnt, s(39));
    }
    refreshFonts();
    /*
     * Due to an issue with the table in QT we nee to repeat the dimension setup.
     */
    accountsTableView->setGeometry(s(50), s(401), s(1035), s(423));
    accountsTableView->setColumnWidth(0, s(255));
    accountsTableView->setColumnWidth(1, s(255));
    accountsTableView->setColumnWidth(2, s(150));
    accountsTableView->setColumnWidth(3, s(90));
    accountsTableView->setColumnWidth(4, s(90));
    accountsTableView->setColumnWidth(5, s(90));
    accountsTableView->setColumnWidth(6, s(90));
    for( int cnt = 0; cnt < accountsTableView->verticalHeader()->count(); cnt++) {
        accountsTableView->setRowHeight(cnt, s(39));
    }
    refreshFonts();
}

void settingswindow::refreshLanguage() {
    generalSettingsLabel->setText(_tr("General settings"));

    languageLabel->setText(_tr("Language") + ":");

    appStyleLabel->setText(_tr("APP style") + ":");

    networkLabel->setText(_tr("Network") + ":");

    alternativeValueLabel->setText(_tr("Default alternative value") + ":");

    windowScaleLabel->setText(_tr("Window scale") + ":");

    userManagerSettingsLabel->setText(_tr("User manager settings"));
    usernameLabel->setText(_tr("Username"));
    userPasswordLabel->setText(_tr("Password"));

    usernameNameLabel->setText(events::getWalletUserPassword().first);
    userPasswordPassLabel->setText("********");

    backupButton->setText(_tr("BACKUP"));
    editButton->setText(_tr("EDIT"));
    walletSettingsLabel->setText(_tr("Wallet settings"));

    refreshFonts();
}

void settingswindow::setState(state_e state) {
    currentState = state;
}

void settingswindow::run() {
    if(pastState != currentState) {
        pastState = currentState;
        if(currentState == STATE_SETTINGS) {
            mdiAreaSettings->setVisible(true);
            windowSettings->setVisible(true);
        } else {
            mdiAreaSettings->setVisible(false);
            windowSettings->setVisible(false);
            settingsEditAccount->setState(settingseditaccount::runMode_e::NONE);
            settingsDeleteAccount->setState(settingsdeleteaccount::runMode_e::NONE);
            settingsEditUser->setState(settingsedituser::runMode_e::NONE);
            addAccountWindow->setState(settingsaddaccount::runMode_e::NONE);
            settingsShowPrivKey->setState(settingsshowprivkey::runMode_e::NONE);
        }
    }
    if(pastScale != events::getScale()) {
        pastScale = events::getScale();
        mdiAreaSettings->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowSettings->setGeometry(0, 0, mdiAreaSettings->width(), mdiAreaSettings->height());
        windowScaleComboBox->setCurrentText(QString::asprintf("%1.1f", pastScale) + "x");
        refreshSize();
    }
    if(pastLanguage.compare(translate::getCurrentLang())) {
        pastLanguage = translate::getCurrentLang();
        languageComboBox->setCurrentText(translate::getCurrentLang());
        refreshLanguage();
        updateWalletSettingsTable();
    }
    if(getWalletUserPasswordChanged != events::getWalletUserPasswordChangedCnt()) {
        getWalletUserPasswordChanged = events::getWalletUserPasswordChangedCnt();
        refreshLanguage();
    }
    if(walletNameKeyListChanged != events::getWalletNameKeyListChanged()) {
        walletNameKeyListChanged = events::getWalletNameKeyListChanged();
        updateWalletSettingsTable();
    }
    if(styleCnt != events::getStyleChangedCnt()) {
        styleCnt = events::getStyleChangedCnt();
        appStyleComboBox->setCurrentText(events::getStyle());
    }
    if(udsSelected != events::getBtcUsdSelect()) {
        udsSelected = events::getBtcUsdSelect();
        if(events::getBtcUsdSelect()) {
            alternativeValueComboBox->setCurrentText("USD");
        } else {
            alternativeValueComboBox->setCurrentText("BTC");
        }
    }
    if(network != events::getNetwork()) {
        network = events::getNetwork();
        if(events::getNetwork() == rpc::network_e::NETWORK_TESTNET) {
            networkComboBox->setCurrentText("testnet");
        } else if(events::getNetwork() == rpc::network_e::NETWORK_MAINNET) {
            networkComboBox->setCurrentText("mainnet");
        }
    }
    if(walletLoaded != events::getWalletLoaded()) {
        walletLoaded = events::getWalletLoaded();
        if(walletLoaded) {
            usernameNameLabel->setVisible(true);
            userPasswordPassLabel->setVisible(true);

            backupButton->setVisible(true);
            editButton->setVisible(true);
        } else {
            usernameNameLabel->setVisible(false);
            userPasswordPassLabel->setVisible(false);

            backupButton->setVisible(false);
            editButton->setVisible(false);
        }
    }
    settingsaddaccount::return_e responseAddAccountWindow = addAccountWindow->run();
    if(responseAddAccountWindow == settingsaddaccount::return_e::RETURN_OK) {

    }
    settingsdeleteaccount::return_e responseAettingsDeleteAccount = settingsDeleteAccount->run();
    if(responseAettingsDeleteAccount == settingsdeleteaccount::return_e::RETURN_OK) {

    }
    settingseditaccount::return_e responseSettingsEditAccount = settingsEditAccount->run();
    if(responseSettingsEditAccount == settingseditaccount::return_e::RETURN_OK) {

    }
    settingsedituser::return_e responseSettingsEditUser = settingsEditUser->run();
    if(responseSettingsEditUser == settingsedituser::return_e::RETURN_OK) {

    }
    settingsShowPrivKey->run();
}

bool settingswindow::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonRelease) {
        if(mouseEvent->button() == Qt::LeftButton) {
            for( int cnt = 0; cnt < accountsTableView->verticalHeader()->count() - 1; cnt++) {
                QPushButton *voteButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 3));
                if (obj == voteButton) {
                    /*settingsEditAccount.init(&mdiAreaSettings, events::getWalletNameKeyList().at(cnt).first);
                    settingsEditAccount.setState(settingseditaccount::runMode_e::RUN);*/
                    return true;
                }
                QPushButton *editButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 4));
                if (obj == editButton) {
                    settingsEditAccount->setState(settingseditaccount::runMode_e::RUN, events::getWalletNameKeyList().at(cnt).first);
                    return true;
                }
                QPushButton *deleteButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 5));
                if (obj == deleteButton) {
                    settingsDeleteAccount->setState(settingsdeleteaccount::runMode_e::RUN, events::getWalletNameKeyList().at(cnt).first);
                    return true;
                }
                QPushButton *showPrivateKeyButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 6));
                if (obj == showPrivateKeyButton) {
                    settingsShowPrivKey->setState(settingsshowprivkey::runMode_e::RUN, events::getWalletNameKeyList().at(cnt).first, events::getWalletNameKeyList().at(cnt).second);
                    return true;
                }
            }
        }
    }
    return false;
}

void settingswindow::on_Backup_ButtonPressed() {
    QFileDialog fileDialog(mdiAreaSettings);
    fileDialog.setReadOnly(true);
    QString dir = fileDialog.getSaveFileName(this, "Save wallet", QString() ,QString("*." DEFAULT_WALLET_EXTENSION));
    if(dir.length() != 0) {
        events::setWalletHistoryChanged();
        walletfile::save(events::getWalletUserPassword().first, events::getWalletUserPassword().second, dir);
    }
}

void settingswindow::on_Edit_ButtonPressed() {
    settingsEditUser->setState(settingsedituser::runMode_e::RUN);
}

void settingswindow::on_Add_ButtonPressed() {
    addAccountWindow->setState(settingsaddaccount::runMode_e::CHOOSE);
}

void settingswindow::on_Language_Changed(const QString &arg1) {
    translate::loadLanguage(arg1);
}

void settingswindow::on_AppStyle_Changed(const QString &arg1) {
    events::setStyle(arg1);
}

void settingswindow::on_Network_Changed(const QString &arg1) {
    networkComboBox->setEnabled(false);
    if(!arg1.compare("testnet")) {
        events::setNetwork(rpc::network_e::NETWORK_TESTNET);
    } else if(!arg1.compare("mainnet")) {
        events::setNetwork(rpc::network_e::NETWORK_MAINNET);
    } else {
        events::setNetwork(rpc::network_e::NETWORK_TESTNET);
    }
    //wallethistory::updateWallets();
    //populate::refreshAll();
    networkComboBox->setEnabled(true);
}

void settingswindow::on_AlternativeValue_Changed(const QString &arg1) {
    if(!arg1.compare("BTC")) {
        events::setBtcUsdSelect(false);
    } else if(!arg1.compare("USD")) {
        events::setBtcUsdSelect(true);
    } else {
        events::setBtcUsdSelect(false);
    }
}

void settingswindow::on_ScaleValue_Changed(const QString &scale) {
    QString sc = scale;
    events::setScale(sc.replace("x", "").toDouble());
}


