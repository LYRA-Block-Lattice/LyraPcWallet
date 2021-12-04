#include "settingswindow.h"

#include <QMdiSubWindow>
#include <QMouseEvent>
#include <QHeaderView>
#include <QScrollBar>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

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
    //mdiAreaSettings->setVisible(false);
    //windowSettings->setVisible(false);
    //delete windowSettings;
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
    windowSettings->setVisible(true);

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

    customNodeIpLabel = new QLabel(mdiAreaSettings);
    customNodeIpLineEdit = new QLineEdit(mdiAreaSettings);
    customNodeIpButton = new QPushButton(mdiAreaSettings);

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
    generalSettingsLabel->setVisible(true);


    languageLabel->setStyleSheet("color: #555;");
    languageLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    languageLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    languageLabel->setVisible(true);

    languageComboBox->setCurrentIndex(0);
    languageComboBox->setAutoFillBackground(false);
    languageComboBox->setStyleSheet(""
        "QComboBox {   "
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(19)) + "px;"
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
    languageComboBox->setVisible(true);
    connect(languageComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Language_Changed(const QString &)));


    appStyleLabel->setStyleSheet("color: #555;");
    appStyleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    appStyleLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    appStyleLabel->setVisible(true);

    appStyleComboBox->setCurrentIndex(0);
    appStyleComboBox->setAutoFillBackground(false);
    appStyleComboBox->setStyleSheet(""
        "QComboBox {   "
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(19)) + "px;"
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
    appStyleComboBox->setVisible(true);
    connect(appStyleComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_AppStyle_Changed(const QString &)));


    networkLabel->setStyleSheet("color: #555;");
    networkLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    networkLabel->setVisible(true);
    networkLabel->setAttribute(Qt::WA_TranslucentBackground, true);

    networkComboBox->setCurrentIndex(0);
    networkComboBox->setAutoFillBackground(false);
    networkComboBox->setStyleSheet(""
        "QComboBox {   "
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(19)) + "px;"
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
    networkComboBox->addItems(NETWORKS);
    networkComboBox->setCursor(Qt::PointingHandCursor);
    networkComboBox->setVisible(true);
    connect(networkComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Network_Changed(const QString &)));

    alternativeValueLabel->setStyleSheet("color: #555;");
    alternativeValueLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    alternativeValueLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    alternativeValueLabel->setVisible(true);

    alternativeValueComboBox->setCurrentIndex(0);
    alternativeValueComboBox->setAutoFillBackground(false);
    alternativeValueComboBox->setStyleSheet(""
        "QComboBox {   "
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(19)) + "px;"
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
    alternativeValueComboBox->setVisible(true);
    connect(alternativeValueComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_AlternativeValue_Changed(const QString &)));

    windowScaleLabel->setStyleSheet("color: #555;");
    windowScaleLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    windowScaleLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    windowScaleLabel->setVisible(true);

    windowScaleComboBox->setCurrentIndex(0);
    windowScaleComboBox->setAutoFillBackground(false);
    windowScaleComboBox->setStyleSheet(""
        "QComboBox {   "
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(19)) + "px;"
               "padding: 1px 18px 1px 3px;"
               "text-align: right;"
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
    windowScaleComboBox->addItems(SCALE_LIST);
    windowScaleComboBox->setCursor(Qt::PointingHandCursor);
    windowScaleComboBox->setVisible(true);
    connect(windowScaleComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_ScaleValue_Changed(const QString &)));

    customNodeIpLabel->setStyleSheet("color: #555;");
    customNodeIpLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    customNodeIpLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    customNodeIpLabel->setVisible(true);

    customNodeIpLineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    customNodeIpLineEdit->setAlignment(Qt::AlignCenter);
    customNodeIpLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: " + QString::number((int)s(19)) + "px;"
                                                    ";}");
    customNodeIpLineEdit->setVisible(true);

    customNodeIpButton->setStyleSheet("background-color: " BUTON_COLOR_BLUE "; border-radius: " + QString::number((int)s(12)) + "px; border: 1px solid #eee; color: #fff; ");
    customNodeIpButton->setFlat(true);
    customNodeIpButton->setCursor(Qt::PointingHandCursor);
    customNodeIpButton->setVisible(true);
    connect(customNodeIpButton, SIGNAL(clicked()),this, SLOT(on_CustomNodeIp_ButtonPressed()));


    userManagerSettingsLabel->setStyleSheet("color: #555;");
    userManagerSettingsLabel->setAlignment(Qt::AlignLeft);
    userManagerSettingsLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    userManagerSettingsLabel->setVisible(true);

    userManagerSettingsBackgroundLabel->setStyleSheet("background-color: #F8F8F8;");
    userManagerSettingsBackgroundLabel->setAlignment(Qt::AlignLeft);
    userManagerSettingsBackgroundLabel->setVisible(true);


    usernameLabel->setStyleSheet("color: #555;");
    usernameLabel->setAlignment(Qt::AlignLeft);
    usernameLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    usernameLabel->setVisible(true);

    usernameNameLabel->setStyleSheet("color: #999;");
    usernameNameLabel->setAlignment(Qt::AlignLeft);
    usernameNameLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    usernameNameLabel->setVisible(true);

    walletSettingsLabel->setStyleSheet("color: #555;");
    walletSettingsLabel->setAlignment(Qt::AlignLeft);
    walletSettingsLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    walletSettingsLabel->setVisible(true);

    userPasswordLabel->setStyleSheet("color: #555;");
    userPasswordLabel->setAlignment(Qt::AlignLeft);
    userPasswordLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    userPasswordLabel->setVisible(true);

    userPasswordPassLabel->setStyleSheet("color: #999;");
    userPasswordPassLabel->setAlignment(Qt::AlignLeft);
    userPasswordPassLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    userPasswordPassLabel->setVisible(true);


    backupButton->setStyleSheet("background-color: " BUTON_COLOR_BLUE "; border-radius: " + QString::number((int)s(12)) + "px; border: 1px solid #eee; color: #fff; ");
    backupButton->setFlat(true);
    backupButton->setCursor(Qt::PointingHandCursor);
    backupButton->setVisible(true);
    connect(backupButton, SIGNAL(clicked()),this, SLOT(on_Backup_ButtonPressed()));

    editButton->setStyleSheet("background-color: " BUTON_COLOR_CYAN "; border-radius: " + QString::number((int)s(12)) + "px; border: 1px solid #eee; color: #fff; ");
    editButton->setFlat(true);
    editButton->setCursor(Qt::PointingHandCursor);
    editButton->setVisible(true);
    connect(editButton, SIGNAL(clicked()),this, SLOT(on_Edit_ButtonPressed()));


    accountsTableView->setStyleSheet(
                                   "QTableView { border: none; "
                                   "background-color: transparent; "
                                   "selection-background-color: transparent} ");
    accountsTableView->setShowGrid(false);
    accountsTableView->verticalHeader()->setVisible(false);
    accountsTableView->horizontalHeader()->setSectionsClickable(false);
    accountsTableView->horizontalHeader()->setStyleSheet("color: #777; "
                        "QHeaderView::section {"
                        "border: 0px solid black;"
                        "border-bottom: 0px;  "
                    "}");
    accountsTableView->horizontalHeader()->setCascadingSectionResizes(false);
    accountsTableView->horizontalHeader()->setEnabled(false);
    accountsTableView->horizontalHeader()->setAutoFillBackground(false);
    accountsTableView->setAlternatingRowColors(true);
    accountsTableView->setVisible(true);
    accountsTableView->setModel(accountsItemModel);
    accountsTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    accountsTableView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);


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

}

void settingswindow::updateWalletSettingsTable() {
    QList<QPair<QString, QString>> accountList = events::getAccountNameIdList();
    accountsItemModel->clear();
    accountsItemModel->setColumnCount(6);
    accountsItemModel->setRowCount(0);

    accountsItemModel->setHeaderData(0, Qt::Horizontal, _tr("Wallet ID"));
    accountsItemModel->setHeaderData(0, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(1, Qt::Horizontal, _tr("Wallet name"));
    accountsItemModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(2, Qt::Horizontal, "");
    accountsItemModel->setHeaderData(2, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(3, Qt::Horizontal, "");
    accountsItemModel->setHeaderData(3, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(4, Qt::Horizontal, "");
    accountsItemModel->setHeaderData(4, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    accountsItemModel->setHeaderData(5, Qt::Horizontal, "");
    accountsItemModel->setHeaderData(5, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
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
        it->setText(pair.second);
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
        voteForButton->setText(_tr("PROFITING"));
        voteForButton->setCursor(Qt::PointingHandCursor);
        voteForButton->setStyleSheet("background-color: " BUTON_COLOR_GREEN "; border-radius: " + QString::number((int)s(18)) + "px; border: " + QString::number((int)s(6)) + "px solid #fff; color: #fff; ");
        voteForButton->installEventFilter(this);
        accountsTableView->setIndexWidget(accountsItemModel->index(cnt, 2), voteForButton);

        editButton = new QPushButton();
        editButton->setText(_tr("EDIT"));
        editButton->setCursor(Qt::PointingHandCursor);
        editButton->setStyleSheet("background-color: " BUTON_COLOR_CYAN "; border-radius: " + QString::number((int)s(18)) + "px; border: " + QString::number((int)s(6)) + "px solid #fff; color: #fff; ");
        editButton->installEventFilter(this);
        accountsTableView->setIndexWidget(accountsItemModel->index(cnt, 3), editButton);

        deleteButton = new QPushButton();
        deleteButton->setText(_tr("DELETE"));
        deleteButton->setCursor(Qt::PointingHandCursor);
        deleteButton->setStyleSheet("background-color: " BUTON_COLOR_RED "; border-radius: " + QString::number((int)s(18)) + "px; border: " + QString::number((int)s(6)) + "px solid #fff; color: #fff; ");
        deleteButton->installEventFilter(this);
        accountsTableView->setIndexWidget(accountsItemModel->index(cnt, 4), deleteButton);

        showPrivateKeyButton = new QPushButton();
        showPrivateKeyButton->setText(_tr("SHOW KEY"));
        showPrivateKeyButton->setCursor(Qt::PointingHandCursor);
        showPrivateKeyButton->setStyleSheet("background-color: " BUTON_COLOR_BLUE "; border-radius: " + QString::number((int)s(18)) + "px; border: " + QString::number((int)s(6)) + "px solid #fff; color: #fff; ");
        showPrivateKeyButton->installEventFilter(this);
        accountsTableView->setIndexWidget(accountsItemModel->index(cnt, 5), showPrivateKeyButton);

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
        accountsItemModel->appendRow(item);
        QPushButton *addButton;
        addButton = new QPushButton();
        addButton->setText(_tr("ADD"));
        addButton->setCursor(Qt::PointingHandCursor);
        addButton->setStyleSheet("background-color: " BUTON_COLOR_GREEN "; border-radius: " + QString::number((int)s(18)) + "px; border: " + QString::number((int)s(6)) + "px solid #fff; color: #fff; ");
        connect(addButton, SIGNAL(clicked()),this, SLOT(on_Add_ButtonPressed()));
        accountsTableView->setIndexWidget(accountsItemModel->index(accountsTableView->verticalHeader()->count() - 1, 5), addButton);
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

    customNodeIpLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    customNodeIpLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.8)));
    customNodeIpButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));

    userManagerSettingsLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));
    usernameLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    usernameNameLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    walletSettingsLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.2)));
    userPasswordLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    userPasswordPassLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    backupButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    editButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));


    accountsTableView->horizontalHeader()->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
    for( int cnt = 0; cnt < accountsTableView->verticalHeader()->count(); cnt++) {
        QStandardItem *tmp = accountsItemModel->itemFromIndex(accountsItemModel->index(cnt, 0));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(0.6)));
        tmp = accountsItemModel->itemFromIndex(accountsItemModel->index(cnt, 1));
        tmp->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        QPushButton *voteButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 2));
        if(voteButton != 0)
            voteButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        QPushButton *editButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 3));
        if(editButton != 0)
            editButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        QPushButton *deleteButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 4));
        if(deleteButton != 0)
            deleteButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
        QPushButton *showPrivKeyButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 5));
        if(showPrivKeyButton != 0)
            showPrivKeyButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.6)));
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

    customNodeIpLabel->setGeometry(s(610), s(100), s(160), s(39));
    customNodeIpLineEdit->setGeometry(s(770), s(100), s(130), s(39));
    customNodeIpButton->setGeometry(s(910), s(105), s(58), s(27));

    userManagerSettingsLabel->setGeometry(s(28), s(165), s(370), s(39));
    userManagerSettingsBackgroundLabel->setGeometry(s(50), s(258), s(1015), s(39));

    usernameLabel->setGeometry(s(52), s(215), s(370), s(39));
    usernameNameLabel->setGeometry(s(52), s(270), s(370), s(39));

    walletSettingsLabel->setGeometry(s(28), s(338), s(370), s(39));

    userPasswordLabel->setGeometry(s(450), s(215), s(370), s(39));
    userPasswordPassLabel->setGeometry(s(450), s(270), s(370), s(39));

    backupButton->setGeometry(s(887), s(264), s(78), s(27));
    editButton->setGeometry(s(980), s(264), s(78), s(27));


    accountsTableView->setGeometry(s(50), s(401), s(1010), s(423));
    accountsTableView->setColumnWidth(0, s(500));
    accountsTableView->setColumnWidth(1, s(150));
    accountsTableView->setColumnWidth(2, s(90));
    accountsTableView->setColumnWidth(3, s(90));
    accountsTableView->setColumnWidth(4, s(90));
    accountsTableView->setColumnWidth(5, s(90));
    for( int cnt = 0; cnt < accountsTableView->verticalHeader()->count(); cnt++) {
        accountsTableView->setRowHeight(cnt, s(39));
    }
    refreshFonts();
    /*
     * Due to an issue with the table in QT we nee to repeat the dimension setup.
     */
    accountsTableView->setGeometry(s(50), s(401), s(1010), s(423));
    accountsTableView->setColumnWidth(0, s(500));
    accountsTableView->setColumnWidth(1, s(150));
    accountsTableView->setColumnWidth(2, s(90));
    accountsTableView->setColumnWidth(3, s(90));
    accountsTableView->setColumnWidth(4, s(90));
    accountsTableView->setColumnWidth(5, s(90));
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

    customNodeIpLabel->setText(_tr("Custom node IP") + ":");
    customNodeIpLineEdit->setPlaceholderText(_tr("Node IP"));
    customNodeIpButton->setText(_tr("APPLY"));

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
            if(settingsEditAccount) {
                settingsEditAccount->setState(settingseditaccount::runMode_e::NONE);
            }
            if(settingsDeleteAccount) {
                settingsDeleteAccount->setState(settingsdeleteaccount::runMode_e::NONE);
            }
            if(settingsEditUser) {
                settingsEditUser->setState(settingsedituser::runMode_e::NONE);
            }
            if(addAccountWindow) {
                addAccountWindow->setState(settingsaddaccount::runMode_e::NONE);
            }
            if(settingsShowPrivKey) {
                settingsShowPrivKey->setState(settingsshowprivkey::runMode_e::NONE);
            }
            if(createProfitAccount) {
                createProfitAccount->setState(createprofitaccount::runMode_e::NONE);
            }
        }
    }
    if(pastScale != events::getScale()) {
        pastScale = events::getScale();
        mdiAreaSettings->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowSettings->setGeometry(0, 0, mdiAreaSettings->width(), mdiAreaSettings->height());
        windowScaleComboBox->setCurrentText(QString::asprintf("%1.1f", events::getScaleStore()) + "x");
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
    if(walletNameKeyListChanged != events::getAccountNameKeyListChanged()) {
        walletNameKeyListChanged = events::getAccountNameKeyListChanged();
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
    if(customIpChanged != events::getCustomIpChanged()) {
        customIpChanged = events::getCustomIpChanged();
        customNodeIpLineEdit->setText(events::getCustomIp(events::getNetwork()));
    }
    if(addAccountWindow) {
        settingsaddaccount::return_e responseAddAccountWindow = addAccountWindow->run();
        if(responseAddAccountWindow == settingsaddaccount::return_e::RETURN_OK) {

        }
        if(addAccountWindow->getState() == settingsaddaccount::runMode_e::NONE) {
            delete addAccountWindow;
            addAccountWindow = nullptr;
        }
    }
    if(settingsDeleteAccount) {
        settingsdeleteaccount::return_e responseAettingsDeleteAccount = settingsDeleteAccount->run();
        if(responseAettingsDeleteAccount == settingsdeleteaccount::return_e::RETURN_OK) {

        }
        if(settingsDeleteAccount->getState() == settingsdeleteaccount::runMode_e::NONE) {
            delete settingsDeleteAccount;
            settingsDeleteAccount = nullptr;
        }
    }
    if(settingsEditAccount) {
        settingseditaccount::return_e responseSettingsEditAccount = settingsEditAccount->run();
        if(responseSettingsEditAccount == settingseditaccount::return_e::RETURN_OK) {

        }
        if(settingsEditAccount->getState() == settingseditaccount::runMode_e::NONE) {
            delete settingsEditAccount;
            settingsEditAccount = nullptr;
        }
    }
    if(settingsEditUser) {
        settingsedituser::return_e responseSettingsEditUser = settingsEditUser->run();
        if(responseSettingsEditUser == settingsedituser::return_e::RETURN_OK) {

        }
        if(settingsEditUser->getState() == settingsedituser::runMode_e::NONE) {
            delete settingsEditUser;
            settingsEditUser = nullptr;
        }
    }
    if(settingsShowPrivKey) {
        settingsShowPrivKey->run();
        if(settingsShowPrivKey->getState() == settingsshowprivkey::runMode_e::NONE) {
            delete settingsShowPrivKey;
            settingsShowPrivKey = nullptr;
        }
    }
    if(createProfitAccount) {
        createprofitaccount::return_e responseCreateProfitAccountUser = createProfitAccount->run();
        if(responseCreateProfitAccountUser == createprofitaccount::return_e::RETURN_OK) {

        }
        if(createProfitAccount->getState() == createprofitaccount::runMode_e::NONE) {
            delete createProfitAccount;
            createProfitAccount = nullptr;
        }
    }
}

bool settingswindow::eventFilter(QObject *obj, QEvent *event) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    QStringList names = events::getAccountNameList();
    if(event->type() == QEvent::MouseButtonRelease) {
        if(mouseEvent->button() == Qt::LeftButton) {
            for( int cnt = 0; cnt < accountsTableView->verticalHeader()->count() - 1; cnt++) {
                QPushButton *voteButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 2));
                if (obj == voteButton) {
                    if(!createProfitAccount) {
                        createProfitAccount = new createprofitaccount();
                        createProfitAccount->init(mdiAreaSettings, cnt);
                    }
                    createProfitAccount->setState(createprofitaccount::runMode_e::RUN, names[cnt]);
                    return true;
                }
                QPushButton *editButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 3));
                if (obj == editButton) {
                    if(!settingsEditAccount) {
                        settingsEditAccount = new settingseditaccount();
                        settingsEditAccount->init(mdiAreaSettings);
                    }
                    settingsEditAccount->setState(settingseditaccount::runMode_e::RUN, names[cnt]);
                    return true;
                }
                QPushButton *deleteButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 4));
                if (obj == deleteButton) {
                    if(!settingsDeleteAccount) {
                        settingsDeleteAccount = new settingsdeleteaccount();
                        settingsDeleteAccount->init(mdiAreaSettings);
                    }
                    settingsDeleteAccount->setState(settingsdeleteaccount::runMode_e::RUN, names[cnt]);
                    return true;
                }
                QPushButton *showPrivateKeyButton = (QPushButton *)accountsTableView->indexWidget(accountsItemModel->index(cnt, 5));
                if (obj == showPrivateKeyButton) {
                    if(!settingsShowPrivKey) {
                        settingsShowPrivKey = new settingsshowprivkey();
                        settingsShowPrivKey->init(mdiAreaSettings);
                    }
                    settingsShowPrivKey->setState(settingsshowprivkey::runMode_e::RUN, cnt);
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
    if(!settingsEditUser) {
        settingsEditUser = new settingsedituser();
        settingsEditUser->init(mdiAreaSettings);
    }
    settingsEditUser->setState(settingsedituser::runMode_e::RUN);
}

void settingswindow::on_Add_ButtonPressed() {
    if(!addAccountWindow) {
        addAccountWindow = new settingsaddaccount();
        addAccountWindow->init(mdiAreaSettings);
    }
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
    } else if(!arg1.compare("dev")) {
        events::setNetwork(rpc::network_e::NETWORK_DEV);
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

void settingswindow::on_CustomNodeIp_ButtonPressed() {
    if(!customNodeIpLineEdit->text().length() && events::getCustomIp().length() != 0) {
        events::setCustomIp("");
        return;
    }
    bool ok = true;
    QStringList s = customNodeIpLineEdit->text().split(".");
    if(s.count() != 4)
        ok = false;
    foreach (const QString& s, s) {
        if(!ok)
            break;
        int intIp = s.toInt(&ok);
        if(intIp > 255 || ok == false) {
            ok = false;
        }
    }
    if(ok) {
        events::setCustomIp(customNodeIpLineEdit->text(), events::getNetwork());
        QMessageBox::information( this, this->windowTitle(),
                _tr("Custom IP successfully set."),
                  QMessageBox::Ok,
                  QMessageBox::Ok);
    } else {
        QMessageBox::critical( this, this->windowTitle(),
            _tr("ERROR: invalid IP."),
            QMessageBox::Ok,
            QMessageBox::Ok);
    }
}
