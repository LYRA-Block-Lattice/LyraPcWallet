#ifndef KEYREVEALER_H
#define KEYREVEALER_H

#include <QDialog>
#include <QGraphicsBlurEffect>

namespace Ui {
class keyrevealer;
}

class keyrevealer : public QDialog {
    Q_OBJECT
public:
    explicit keyrevealer(int nr, bool immediate = true, bool persistent = false, QWidget *parent = nullptr);
    ~keyrevealer();
    QString getKey();
private:
    Ui::keyrevealer *ui;
    enum {
        RESULT_NONE,
        RESULT_REJECTED,
        RESULT_ACCEPTED
    }result = keyrevealer::RESULT_NONE;

   QGraphicsBlurEffect *blurEffectCentralWidget = nullptr;
    int nr;
    bool immediate = true;
    bool persistent = false;
    QString pKey = "";

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // KEYREVEALER_H
