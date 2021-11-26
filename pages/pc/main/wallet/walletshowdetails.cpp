#include "walletshowdetails.h"

#include <QApplication>

#include "language/translate.h"
#include "pages/pc/textformating.h"
#include "extensions/qrcodegen.h"
#include "wallet/events.h"

#define s(s) _scale(s)

walletshowdetails::walletshowdetails() {

}

walletshowdetails::~walletshowdetails() {
    //delete thisWindow;
    //delete thisWidget;
    //delete thisMdiArea;
}

void walletshowdetails::init(QMdiArea *mdiArea) {
    this->parent = mdiArea;
    this->setParent(mdiArea);
    thisMdiArea = new QMdiArea(mdiArea);
    thisMdiArea->setVisible(false);
    thisWidget = new QWidget(thisMdiArea);
    thisWidget->show();
    thisWindow = new QMdiSubWindow(thisMdiArea);
    thisWindow->setWidget(thisWidget);
    thisWindow->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    thisWindow->setWindowFlag(Qt::FramelessWindowHint, true);
    thisWindow->setGeometry(0, 0, thisMdiArea->width(), thisMdiArea->height());
    thisWindow->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/whiteBack.png)");
    thisWindow->setVisible(false);

    graphicsView = new QGraphicsView(thisMdiArea);
    scene = new QGraphicsScene();
    graphicsView->setScene(scene);


    detailTextEdit = new QTextEdit(thisMdiArea);
    detailTextEdit->setReadOnly(true);

    backPushButton = new QPushButton(thisMdiArea);
    backPushButton->setCursor(Qt::PointingHandCursor);
    backPushButton->setFlat(true);
    backPushButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/green.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    backPushButton->setText(_tr("OK"));
    connect(backPushButton, SIGNAL(clicked()),this, SLOT(on_Back_ButtonPressed()));
}

void walletshowdetails::run() {
    if(pastScale != events::getScale()) {
        thisMdiArea->setGeometry(0, 0, parent->width(), parent->height());
        thisWindow->setGeometry(0, 0, thisMdiArea->width(), thisMdiArea->height());
        detailTextEdit->setGeometry(s(10), s(10), parent->width() - s(20), parent->height() - s(70));
        detailTextEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

        backPushButton->setGeometry((parent->width() / 2) - s(50), parent->height() - s(50), s(100), s(40));
        backPushButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/green.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
        backPushButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        pastScale = events::getScale();
        graphicsView->setGeometry((parent->width() / 2) - (size * 4) , (parent->height() / 2) - (size * 4), (size * 8), (size * 8));
    }
    if(pastLanguage.compare(translate::getCurrentLang())) {
        detailTextEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
        backPushButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
        backPushButton->setText(_tr("OK"));
        pastLanguage = translate::getCurrentLang();
    }
}

void walletshowdetails::show(QList<QMap<QString, QString>> transaction) {
    thisMdiArea->setVisible(true);
    thisWindow->setVisible(true);
    detailTextEdit->setVisible(true);
    graphicsView->setVisible(false);
    QString str;
    QString key;
    QMap<QString, QString> tmp;
    tmp = transaction[0];
    str.append(_tr("Height") + ": " + tmp["Height"] + "\n\n");
    tmp = transaction[1];
    if(tmp["IsReceive"].toInt()) {
        str.append(_tr("Direction") + ": " + _tr("RECEIVED") + "\n\n");
    } else {
        str.append(_tr("Direction") + ": " + _tr("SENT") + "\n\n");
    }
    tmp = transaction[2];
    QDateTime date = QDateTime::fromMSecsSinceEpoch(tmp["TimeStamp"].toLongLong());
    str.append(_tr("Time stamp") + ": " + date.toString("hh:mm:ss yyyy-MM-dd") + "\n\n");
    tmp = transaction[3];
    str.append(_tr("Send Account ID") + ": " + tmp["SendAccountId"] + "\n\n");
    tmp = transaction[4];
    str.append(_tr("Send hash") + ": " + tmp["SendHash"] + "\n\n");
    tmp = transaction[5];
    str.append(_tr("Receive Account ID") + ": " + tmp["RecvAccountId"] + "\n\n");
    tmp = transaction[6];
    str.append(_tr("Receive hash") + ": " + tmp["RecvHash"] + "\n\n");
    tmp = transaction[7];
    str.append(_tr("Amount transferred") + ": \n");
    QStringList tmpKeys = tmp.keys();
    foreach(key, tmpKeys) {
        str.append(key + ": " + textformating::toValue(tmp[key]) + "\n");
    }
    tmp = transaction[8];
    tmpKeys = tmp.keys();
    str.append("\n" + _tr("Amount in this account") + ": \n");
    foreach(key, tmpKeys) {
        str.append(key + ": " + textformating::toValue(tmp[key]) + "\n");
    }
    if(transaction.count() >= 10) {
        tmp = transaction[9];
        str.append("\n" + _tr("Note") + ": \n\n" + tmp["note"] + "\n\n");
    }
    detailTextEdit->setText(str);
    back = false;
    while(1) {
        QApplication::processEvents();
        if(back)
            break;
        if(events::getAppClosing())
            break;
    }
    detailTextEdit->setVisible(false);
    thisMdiArea->setVisible(false);
    thisWindow->setVisible(false);
}

void walletshowdetails::show(QString text) {
    thisMdiArea->setVisible(true);
    thisWindow->setVisible(true);
    graphicsView->setVisible(true);
    detailTextEdit->setVisible(false);
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText(text.toUtf8(), tempBuffer, qrcode,
        qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    if (ok) {
        scene->clear();
        scene->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255)));
        int border = 4;
        thisMdiArea->setGeometry(0, 0, parent->width(), parent->height());
        thisWindow->setGeometry(0, 0, thisMdiArea->width(), thisMdiArea->height());
        size = qrcodegen_getSize(qrcode);
        graphicsView->setGeometry(0, 0, thisMdiArea->width(), thisMdiArea->height());
        graphicsView->repaint();
        for (int y = -border; y < size + border; y++) {
            for (int x = -border; x < size + border; x++) {
                if(qrcodegen_getModule(qrcode, x, y)) {
                    //scene.addRect(QRectF(x * 8, y * 8, 8, 8));
                    scene->addRect(QRectF(x * 8 + 1, y * 8 + 1, 6, 6));
                    scene->addRect(QRectF(x * 8 + 2, y * 8 + 2, 4, 4));
                    scene->addRect(QRectF(x * 8 + 3, y * 8 + 3, 2, 2));
                    scene->addRect(QRectF(x * 8 + 4, y * 8 + 4, 1, 1));
                }
            }
        }
        scene->update();
        graphicsView->repaint();
    }
    back = false;
    while(1) {
        QApplication::processEvents();
        if(back)
            break;
        if(events::getAppClosing())
            break;
    }
    graphicsView->setVisible(false);
    thisMdiArea->setVisible(false);
    thisWindow->setVisible(false);
}

void walletshowdetails::setDetailsVisible(bool visible) {
    thisMdiArea->setVisible(visible);
}

void walletshowdetails::on_Back_ButtonPressed() {
    back = true;
}
