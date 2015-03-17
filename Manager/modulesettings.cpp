#include "modulesettings.h"
#include "ui_modulesettings.h"

#include <QSettings>
#include <QDebug>
#include <QMessageBox>

ModuleSettings::ModuleSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModuleSettings)
{
    ui->setupUi(this);

    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));
}

bool ModuleSettings::loadUi(const QString &modulePath)
{
    settings = new QSettings(modulePath + "/ui.ini", QSettings::IniFormat);
    if ((settings->status() != 0) || (!settings->isWritable()))
    {
        QMessageBox::warning(this, "QSetting loading error", "Cannot acces ui.ini.");
        return false;
    }

    QString moduleName = settings->value("ModuleName").toString();
    if (moduleName != "")
    {
        ui->moduleNameLabel->setText(moduleName);
    } else {
        QMessageBox::warning(this, "ModuleName parameter error", "ModuleName parameter is not set in ui.ini. Please, resolve this.");
        return false;
    }

    uiManager.constructUi(&settings);
    ui->verticalLayout->addLayout(uiManager.getLayout());

    return true;
}

ModuleSettings::~ModuleSettings()
{
    delete ui;
}

void ModuleSettings::makeUi()
{

}

void ModuleSettings::on_okPushButton_clicked()
{
    uiManager.saveParameters();
    close();
}
