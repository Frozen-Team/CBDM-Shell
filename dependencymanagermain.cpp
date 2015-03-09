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

#include "closemenubar.h"



DependencyManagerMain::DependencyManagerMain(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint | Qt::Window), // Remove titlebar and border
    ui(new Ui::DependencyManagerMain)
{
    ui->setupUi(this);
    CloseMenuBar* closeMenu = new CloseMenuBar(ui->menubar);
    ui->menubar->setCornerWidget(closeMenu);
    connect(closeMenu, SIGNAL(onClose()), this, SLOT(close()));
    ui->menubar->setTitle("GUI Dependency Manager");

    settingsDialog = new SettingsDialog(this);
    if (settingsDialog != nullptr)
    {
        settingsDialog->loadSettings();
    }

    ui->availableListView->setModel(&availableListModel);
    ui->usedListView->setModel(&usedListModel);

    loadModulesList("D:\\YandexDisk\\FrozenTeam\\CPPDependenciesManager\\core\\modules\\");

    for (const QString &itemStr : availableList)
    {
        QStandardItem *item = new QStandardItem(itemStr);
        availableListModel.appendRow(item);
    }

    for (int i = 0; i < 50; ++i)
    {

    }
}

DependencyManagerMain::~DependencyManagerMain()
{
    delete ui;
}

void DependencyManagerMain::on_actionExit_triggered()
{
    close();
}

void DependencyManagerMain::on_actionMinimize_triggered()
{
    QWidget::showMinimized();
}

void DependencyManagerMain::on_actionSettings_triggered()
{
    if (settingsDialog != nullptr)
    {
        settingsDialog->show();
    } else {
        QMessageBox::critical(this, "Error showing SettingsDialog!", "Not created dialog. SettingsDialog == nullptr");
    }

}

void DependencyManagerMain::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About");
}

void DependencyManagerMain::loadModulesList(const QString &path)
{
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
