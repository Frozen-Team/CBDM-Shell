#include "helpdialog.h"
#include "ui_helpdialog.h"
#include "version.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    ui->versionLabel->setText(QString(QString(VER_FILEDESCRIPTION_STR) + ", Version: " + QString(VER_PRODUCTVERSION_STR)));
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
