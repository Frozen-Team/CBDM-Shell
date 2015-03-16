#include "modulesettings.h"
#include "ui_modulesettings.h"

#include <QSettings>
#include <QDebug>
#include <QMessageBox>

QMap<QString, QStringList> uiParams;

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

    for (const QString &key : settings->allKeys())
    {
        QStringList splitValue = key.split("/");
        if (splitValue.size() <= 1)
        {
            continue;
        }
        QString name = splitValue.at(0);
        QString param = splitValue.at(1);
        if (uiParams.keys().indexOf(name) >= 0)
        {
            uiParams[name].push_back(param);
        } else {
            QStringList firstParam = { param };
            uiParams[name] = firstParam;
        }
    }

    // Construction ui
    for (QString &key : uiParams.keys())
    {
        QMap<QString, QVariant> component;
        for (auto &valueKey : uiParams[key])
        {
            QString value = settings->value(key + "/" + valueKey).toString();
            component[valueKey] = value;
        }
        uiManager.addComponent(key, component);
    }
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
    // TODO Add functionality

    close();
}
