#include "info.h"
#include "ui_info.h"

#include "config.h"

info::info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::info)
{
    ui->setupUi(this);
    ui->label_3->setText(ui->label_3->text().replace("<__VERSION__>", APP_VERSION));
}

info::~info()
{
    delete ui;
}
