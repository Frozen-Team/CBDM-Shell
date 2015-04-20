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
#include <QDateTime>

#include <functional>

#include "qtutils.h"
#include "helpdialog.h"



const QString modulesPathSuffix = "\\core\\modules\\";


DependencyManagerMain::DependencyManagerMain(QWidget *parent)
    : ui(new Ui::DependencyManagerMain)
{
    Q_UNUSED(parent)
    ui->setupUi(this);

    moduleSettingsDialog = nullptr;
    moduleSettingsDialog = new ModuleSettings(this);
    if (moduleSettingsDialog != nullptr)
    {
        moduleSettingsDialog->setWindowModality(Qt::ApplicationModal);
    }

    settingsDialog = nullptr;
    settingsDialog = new SettingsDialog(this);
    if (settingsDialog != nullptr)
    {
        settingsDialog->setWindowModality(Qt::ApplicationModal);
        settingsDialog->loadSettings();
        if (settingsDialog->isNeedToShow())
        {
            settingsDialog->show();
        }
    }

    std::function<void (QListView *)> makeListActive = [=](QListView* list) {
        if (list != nullptr){
            activeModulesList = list;
        }
    };
    connect(ui->availableListView, &QListView::clicked, [=]{
        makeListActive(ui->availableListView);
    });
    connect(ui->usedListView, &QListView::clicked, [=]{
        makeListActive(ui->usedListView);
    });
    makeListActive(ui->availableListView);

    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
    connect(settingsDialog, SIGNAL(settingsChanged()), this, SLOT(reloadModules()));
    connect(ui->actionConfigure, SIGNAL(triggered()), this, SLOT(configureModules()));
    connect(genProcess.get(), &GenProcess::progressUpdate, ui->progressBar, &QProgressBar::setValue);
    connect(genProcess.get(), &GenProcess::logWrite, this, &DependencyManagerMain::writeLog);

    ui->availableListView->setModel(&availableListModel);
    ui->usedListView->setModel(&usedListModel);

    ui->availableListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->usedListView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    reloadModules();
}

DependencyManagerMain::~DependencyManagerMain()
{
    delete ui;
}

void DependencyManagerMain::updateUi(int flag)
{
    switch (flag) {
    case 0: // finished

        break;
    default:
        break;
    }
}

void DependencyManagerMain::on_actionStartFromScratch_triggered()
{
    if (settingsDialog != nullptr)
    {
        settingsDialog->clearSettings();
        if (settingsDialog->isNeedToShow())
        {
            settingsDialog->show();
        }
    }
    availableList.clear();
    usedList.clear();
    availableListModel.clear();
    usedListModel.clear();

    reloadModules();
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
    usedListModel.clear();

    loadModulesList(settingsDialog->getScriptPath() + modulesPathSuffix);

    for (const QString &itemStr : availableList)
    {
        QStandardItem *item = new QStandardItem(itemStr);
        availableListModel.appendRow(item);
    }
}

void DependencyManagerMain::writeLog(const QString &message)
{
    QString writeMessage = message;
    if (writeMessage.length() > 0)
    {
        if (writeMessage[writeMessage.length() - 1] == '\r')
        {
            writeMessage[writeMessage.length() - 1] = '\0';
        }
        ui->logsBrowser->append(QDateTime::currentDateTime().toString("[HH:mm:ss] ") + writeMessage);
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

void DependencyManagerMain::updateModulesModels()
{
    int availableListIndex = ui->availableListView->selectionModel()->selectedIndexes().size() > 0 ?
                ui->availableListView->selectionModel()->selectedIndexes().at(0).row() : 0;
    int usedListIndex = ui->usedListView->selectionModel()->selectedIndexes().size() > 0 ?
                ui->usedListView->selectionModel()->selectedIndexes().at(0).row() : 0;
    availableListModel.clear();
    usedListModel.clear();
    for (const auto &itemStr : availableList)
    {
        QStandardItem *item = new QStandardItem(itemStr);
        availableListModel.appendRow(item);
    }
    for (const auto &itemStr : usedList)
    {
        QStandardItem *item = new QStandardItem(itemStr);
        usedListModel.appendRow(item);
    }
    ui->availableListView->selectionModel()->select(availableListModel.index(availableListIndex, 0),
                                                    QItemSelectionModel::Select);
    ui->usedListView->selectionModel()->select(usedListModel.index(usedListIndex, 0),
                                               QItemSelectionModel::Select);
}

void DependencyManagerMain::on_actionHelp_me_triggered()
{
    HelpDialog* helpDialog = new HelpDialog(this);
    helpDialog->show();
}

void DependencyManagerMain::on_availableListView_doubleClicked(const QModelIndex &index)
{    
    showModuleSettingsDialog(index.data().toString());
}

void DependencyManagerMain::on_usedListView_doubleClicked(const QModelIndex &index)
{
    showModuleSettingsDialog(index.data().toString());
}

void DependencyManagerMain::on_allRightButton_clicked()
{
    for (const auto &itemStr : availableList)
    {
        usedList.push_back(itemStr);
    }
    availableList.clear();
    updateModulesModels();
}

void DependencyManagerMain::on_rightButton_clicked()
{
    for (auto &item : ui->availableListView->selectionModel()->selectedIndexes())
    {
        QString &itemStr = item.data().toString();
        usedList.push_back(itemStr);
        availableList.removeAll(itemStr);
    }
    updateModulesModels();
}

void DependencyManagerMain::on_leftButton_clicked()
{
    if (ui->usedListView != nullptr)
    {
        for (auto &item : ui->usedListView->selectionModel()->selectedIndexes())
        {
            QString &itemStr = item.data().toString();
            availableList.push_back(itemStr);
            usedList.removeAll(itemStr);
        }
        updateModulesModels();
    } else
    {

    }
}

void DependencyManagerMain::on_allLeftButton_clicked()
{
    for (const auto &itemStr : usedList)
    {
        availableList.push_back(itemStr);
    }
    usedList.clear();
    updateModulesModels();
}

void DependencyManagerMain::configureModules()
{
    if (ui->usedListView != nullptr)
    {
        for (auto &item : ui->usedListView->selectionModel()->selectedIndexes())
        {
            QString &itemStr = item.data().toString();
            availableList.push_back(itemStr);
            usedList.removeAll(itemStr);
        }
        if (activeModulesList != nullptr)
        {
            for (auto &index : activeModulesList->selectionModel()->selectedIndexes())
            {
                showModuleSettingsDialog(index.data().toString());
            }
        } else
        {
            QMessageBox::critical(this, "Critical", "Error: Cannot show any module settings dialog. activeModulesList == nullptr");
            writeLog(QString("Error: Cannot show any module settings dialog. activeModulesList == nullptr"));
        }
    } else
    {
        QString message = QtUtils::stringFormat(QString("Component is null: %s"), "usedListView");
        QMessageBox::critical(this, "Critical", message);
        writeLog(message);
    }
}

void DependencyManagerMain::on_configureButton_clicked()
{
    configureModules();
}

void DependencyManagerMain::on_generateButton_clicked()
{
    if (settingsDialog == nullptr)
    {
        // Show error message
        return;
    }
    if (settingsDialog->isNeedToShow())
    {
        settingsDialog->show();
    } else
    {
        QFile outFile;
        outFile.setFileName(settingsDialog->getScriptPath() + "/config.py");
        outFile.open(QFile::WriteOnly);

        if(outFile.isOpen())
        {
            // TODO build dir
            QString confStr;
            confStr += "import os\n"
                       "directories = {\n"
                       "    \"solutionDir\": \"" + settingsDialog->getOutputPath() + "\",\n"
                       "    \"downloadDir\": \"Download\",\n"
                       "    \"libFolder\": \"Lib\",\n"
                       "    \"buildDir\": \"" + settingsDialog->getBuildPath() + "\",\n"
                       "    \"tools_path\": \"Tools\",\n"
                       "    \"visualStudioDir\": os.path.abspath(\"" + settingsDialog->getVsPath() + "\")\n"
                       "}\n";
            confStr += "projectName = \"" + settingsDialog->getProjectName() + "\"\n";
            confStr += "cmakeVersion = \"" + settingsDialog->getCmakeVersion() + "\"\n";
            confStr += "cmakeGenerator = \"" + settingsDialog->getVsVersion() + "\"\n";
            confStr += "buildArchitecture = \"" + settingsDialog->getArchitecture() + "\"\n";
            confStr += "dependencies = {\n";
            for (auto &module : usedList)
            {
                confStr += "    \"" + module + "\": { \n";
                QMap<QString, QVariant> map;
                if (moduleSettingsDialog->getModuleValues(QString(settingsDialog->getScriptPath() + modulesPathSuffix + module), &map))
                {
                    for (const auto &key : map.keys())
                    {
                        QVariant value = map[key];
                        confStr += "        \"" + key + "\": " + (value.type() == QVariant::Bool ? (value.toString().toLower() == "true" ?
                                                                                                    "True" : "False") : "\"" + value.toString() + "\"") + ",\n";
                    }
                    confStr[confStr.length() - 2] = ' ';
                    confStr[confStr.length() - 1] = '\n';

                }
                confStr += "    },\n";
            }
            confStr += "}";
            QTextStream textStream(&outFile);
            textStream << confStr;

            outFile.close();
            genProcess->runScript(settingsDialog->getPythonPath(), settingsDialog->getScriptPath());
        } else
        {
             // Show error message
        }
    }
}
