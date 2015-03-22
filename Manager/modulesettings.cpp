#include "modulesettings.h"
#include "ui_modulesettings.h"

#include <QSettings>
#include <QDebug>
#include <QMessageBox>

#include <qtutils.h>

ModuleSettings::ModuleSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModuleSettings)
{
    ui->setupUi(this);

    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionSave_and_close, SIGNAL(triggered()), this, SLOT(saveAndClose()));
    connect(ui->okPushButton, SIGNAL(pressed()), this, SLOT(saveAndClose()));
    settings = nullptr;
}

bool ModuleSettings::loadUi(const QString &modulePath)
{
    QString filePath = modulePath + "/ui.ini";
    QString moduleFolderName = QtUtils::getNameFromPath(modulePath);
    if (!QtUtils::isFileExists(filePath))
    {
        QMessageBox::warning(this, "QSetting loading error", "File ui.ini is not exists in module folder: " + moduleFolderName);
        return false;
    }
    if (settings)
    {
        delete settings;
        settings = nullptr;
    }
    settings = new QSettings(filePath, QSettings::IniFormat);
    if ((settings->status() != 0) || (!settings->isWritable()))
    {
        QMessageBox::warning(this, "QSetting loading error", "Cannot acces ui.ini in module folder: " + moduleFolderName);
        return false;
    }

    if (settings == nullptr)
    {
        QMessageBox::critical(this, "Critical error", "Cannot create object QSettings. settings == nullptr."
                                                      " Module folder: " + moduleFolderName);
        return false;
    }
    QString moduleName = settings->value("ModuleName").toString();
    if (moduleName != "")
    {
        ui->moduleNameLabel->setText(moduleName);
    } else {
        QMessageBox::warning(this, "ModuleName parameter error", "ModuleName parameter is not set in "
                                                                 "ui.ini. Module folder: " + moduleFolderName);
        return false;
    }

    uiManager.constructUi(&settings);
    if (ui->verticalLayout)
    {
        ui->verticalLayout->addLayout(uiManager.getLayout());
    } else
    {
        QMessageBox::critical(this, "Adding UI Error", "Cannot add component to ui->verticalLayout due "
                                                       "to ui->verticalLayout == nullptr. Module folder: " + moduleFolderName);
        return false;
    }

    return true;
}

ModuleSettings::~ModuleSettings()
{
    delete ui;
}

bool ModuleSettings::getModuleValues(const QString &modulePath, QMap<QString, QVariant>* valuesMap)
{
    if (valuesMap == nullptr)
    {
        QMessageBox::warning(this, "Error", "Argument valuesMap == nullptr in bool ModuleSettings::getModuleValues");
        return false;
    }
    if (!loadUi(modulePath))
    {
        return false;
    }
    for (const auto &component : *uiManager.getUiComponents())
    {
        if (component->hasValue())
        {
            (*valuesMap)[component->getParameterName()] = component->getValue();
        }
    }

    return true;
}

void ModuleSettings::saveAndClose()
{
    uiManager.saveParameters();
    close();
}
