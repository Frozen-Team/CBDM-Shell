#include "dependencymanagermain.h"
#include "ui_dependencymanagermain.h"

#include <QFile>
#include <QDir>
#include <QProcess>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>
#include <QStandardItemModel>


const QString modulesPathSuffix = "\\core\\modules\\";


DependencyManagerMain::DependencyManagerMain(QWidget *parent) :
    ui(new Ui::DependencyManagerMain)
{
    Q_UNUSED(parent)
    ui->setupUi(this);

    moduleSettingsDialog = new ModuleSettings(this);
    moduleSettingsDialog->setWindowModality(Qt::ApplicationModal);

    settingsDialog = new SettingsDialog(this);
    settingsDialog->setWindowModality(Qt::ApplicationModal);
    if (settingsDialog != nullptr)
    {
        settingsDialog->loadSettings();
    }
    if (settingsDialog->isNeedToShow())
    {
        settingsDialog->show();
    }

    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
    connect(settingsDialog, SIGNAL(settingsChanged()), this, SLOT(reloadModules()));

    ui->availableListView->setModel(&availableListModel);
    ui->usedListView->setModel(&usedListModel);

    reloadModules();
}

DependencyManagerMain::~DependencyManagerMain()
{
    delete ui;
}

void DependencyManagerMain::showSettingsDialog()
{
    if (settingsDialog != nullptr)
    {
        settingsDialog->show();
    } else {
        QMessageBox::critical(this, "Error showing SettingsDialog!", "Not created dialog. SettingsDialog == nullptr.");
    }
}

void DependencyManagerMain::showModuleSettingsDialog(const QString &moduleName)
{
    if (moduleSettingsDialog != nullptr)
    {
        if (moduleSettingsDialog->loadUi(settingsDialog->getScriptPath() +
                                         modulesPathSuffix + moduleName))
        {
            moduleSettingsDialog->show();

        }
    } else {
        QMessageBox::critical(this, "Error showing ModuleSettingsDialog!", "Not created dialog. ModuleSettingsDialog == nullptr.");
    }
}

void DependencyManagerMain::reloadModules()
{
    availableListModel.clear();
    loadModulesList(settingsDialog->getScriptPath() + modulesPathSuffix);
    qDebug() << settingsDialog->getScriptPath() + modulesPathSuffix;

    for (const QString &itemStr : availableList)
    {
        QStandardItem *item = new QStandardItem(itemStr);
        availableListModel.appendRow(item);
    }
}

void DependencyManagerMain::on_actionExit_triggered()
{
    close();
}

void DependencyManagerMain::on_actionMinimize_triggered()
{
    QWidget::showMinimized();
}

void DependencyManagerMain::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About");
}

void DependencyManagerMain::loadModulesList(const QString &path)
{
    modulesList.clear();
    availableList.clear();
    usedList.clear();

    QDir dir(path);
    QFileInfoList dirsList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &dir : dirsList)
    {
        QFile pyFile(dir.absoluteFilePath() + "/index.py");

        if (pyFile.exists())
        {
            modulesList.push_back(dir.fileName());
        }
    }
    availableList = modulesList;
}

void DependencyManagerMain::on_actionHelp_me_triggered()
{

}

void DependencyManagerMain::on_availableListView_doubleClicked(const QModelIndex &index)
{    
    showModuleSettingsDialog(index.data().toString());
}
