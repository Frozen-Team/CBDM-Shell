#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "QFile"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>

#include "qtutils.h"

const QMap<QString, QString> AVAILABLE_ARCHS = {{"x86", "x86"}, {"x64", "x64"}}; // for reason if different values of archs

SettingsDialog::SettingsDialog(QWidget *parent) :
    ui(new Ui::SettingsDialog), QMainWindow(parent)
{
    ui->setupUi(this);
    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));

    needToShow = false;

    this->setWindowTitle("Settings");
    this->setWindowModality(Qt::ApplicationModal);

    // Loading settings
    settings = new QSettings("configuration.ini", QSettings::IniFormat, this);

    for (const auto &arch : AVAILABLE_ARCHS.keys())
    {
        ui->archComboBox->addItem(arch);
    }

    ui->archComboBox->setCurrentIndex(-1); // None by default
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

//struct SettingsHelper
//{
//    QString settingName;
//    QString execName;
//    QStringList params;
//    QRegExp findRegExpr;
//    QStringList checkFolderPathsList;
//    QString postPath;
//    QRegExp versionRegExpr;
//};

//const QVector<SettingsHelper> settingsHelpers = { {"PythonPath", "python.exe", QStringList() << "-V",
//                                                   QRegExp("^(.*)Python[3-9][\\d]+((\\\\$)|(//$)|$)"), QStringList(), "", QRegExp("(\\d\\.\\d\\.\\d)")},
//                                                  {"VSPath", "cl.exe", QStringList(), QRegExp("VC"),
//                                                   QStringList() << "Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin", "", QRegExp("((\\d+).?){3}")}
//                                                };

const QMap<int, QString> vsVersionMap = {
    {16, "Microsoft Visual Studio 2010"},
    {17, "Microsoft Visual Studio 2012"},
    {18, "Microsoft Visual Studio 2013"}};

void SettingsDialog::loadSettings()
{
    needToShow = false;
    // Check for python
    if ((settings->contains("PythonPath")) && ((pythonPath = settings->value("PythonPath").toString()) != QString("")) && isExists(pythonPath))
    {
        ui->pythonPathLine->setText(pythonPath);
    } else
    {
        QMap<QString, QString> pathsMap;
        if (findPath("python.exe", QStringList() << "-V", QRegExp("^(.*)Python[3-9][\\d]+((\\\\$)|(//$)|$)"),
                     QStringList(), "", QRegExp("(\\d\\.\\d\\.\\d)"), pathsMap))
        {
            pythonPath = pathsMap.keys().last();
            ui->pythonPathLine->setText(pathsMap.keys().last());
        } else {
            needToShow = true;
        }
    }

    // Check for Visual Studio
    if ((settings->contains("VSPath")) && ((vsPath = settings->value("VSPath").toString()) != QString("")) && isExists(vsPath) &&
            ((settings->contains("VSVersion")) && (vsVersion = settings->value("VSVersion").toString()) != QString("")))
    {
        ui->vsVersionLabel->setText(vsVersion);
        ui->vsPathLine->setText(vsPath);
    } else
    {
        QMap<QString, QString> pathsMap;
        if (findPath("cl.exe", QStringList(), QRegExp("VC"),
                     QStringList() << "Program Files (x86)/Microsoft Visual Studio 12.0/" << "Applications/VS 2013", "", QRegExp("((\\d+).?){3}"), pathsMap))
        {
            vsPath = pathsMap.keys().last();
            QRegExp parsVSVer("\\d.");
            parsVSVer.indexIn(pathsMap[vsPath]);
            vsVersion = vsVersionMap[parsVSVer.capturedTexts().at(0).toInt()];
            ui->vsVersionLabel->setText(vsVersion);
            ui->vsPathLine->setText(pathsMap.keys().last());
        } else {
            needToShow = true;
        }
    }

    // Check for architecture
    if ((settings->contains("Architecture")) && ((architecture = settings->value("Architecture").toString()) != QString("")))
    {
        ui->archComboBox->setCurrentText(architecture);
    } else
    {
        needToShow = true;
    }

    // Check for ProjectName
    if ((settings->contains("ProjectName")) && ((projectName = settings->value("ProjectName").toString()) != QString("")))
    {
        ui->projectLine->setText(projectName);
    } else
    {
        needToShow = true;
    }

    // Check for Cmake version
    if ((settings->contains("cmakeVersion")) && ((cmakeVerion = settings->value("cmakeVersion").toString()) != QString("")))
    {
        ui->cmakeLineEdit->setText(cmakeVerion);
    } else
    {
        needToShow = true;
    }


    // Check for Script Path
    if ((settings->contains("ScriptPath")) && ((scriptPath = settings->value("ScriptPath").toString()) != QString("")) && isExists(scriptPath))
    {
        ui->scriptPathLine->setText(scriptPath);
    } else
    {
        needToShow = true;
    }

    // Check for Output Path
    if ((settings->contains("OutputPath")) && ((outputPath = settings->value("OutputPath").toString()) != QString("")))
    {
        ui->outputPathLine->setText(outputPath);
    } else
    {
        needToShow = true;
    }

    // Check for Build Path
    if ((settings->contains("BuildPath")) && ((buildPath = settings->value("BuildPath").toString()) != QString("")))
    {
        ui->buildPathLine->setText(buildPath);
    } else
    {
        needToShow = true;
    }
}

void SettingsDialog::clearSettings()
{
    ui->projectLine->setText("");
    ui->cmakeLineEdit->setText("");
    ui->pythonPathLine->setText("");
    ui->vsPathLine->setText("");
    ui->scriptPathLine->setText("");
    ui->archComboBox->setCurrentIndex(-1);
    ui->outputPathLine->setText("");
    ui->buildPathLine->setText("");
    needToShow = true;

    updateSettings();
}

bool SettingsDialog::findPath(const QString &execName, const QStringList &params, QRegExp &findRegExpr, QStringList &checkFolderPathsList, const QString &postPath,
                              const QRegExp &versionRegExpr, QMap<QString, QString> &found)
{
    // Trying to find in system variables
    QStringList paths(QString(qgetenv("Path")).split(";"));
    QStringList checkFoldersList(QStringList(checkFolderPathsList) << QString(""));
    QStringList foundPaths;

    for (QString &path : paths)
    {
        if (path.contains(findRegExpr))
        {
            foundPaths << path;
        }
    }

    for (QFileInfo &info : QDir::drives())
    {
        for (QString &checkDir : checkFoldersList)
        {
            QDir dir(info.path() + checkDir);
            QFileInfoList infoList = dir.entryInfoList(QDir::Dirs);

            for (QFileInfo &dirInfo : infoList)
            {
                if (dirInfo.absoluteFilePath().contains(findRegExpr))
                {
                    foundPaths << dirInfo.absoluteFilePath();
                }
            }
        }
    }

    // Set true if found at least one
    bool retValue = false;

    for (QString const &path : foundPaths)
    {
        processs = new QProcess(this);

        QString program(path + postPath + "/" + execName);

        processs->setProcessChannelMode(QProcess::MergedChannels);
        processs->start(program, params);

        processs->waitForFinished();
        processs->waitForReadyRead();

        QRegExp parsVersion(versionRegExpr);

        parsVersion.indexIn(processs->readAll(), 0, QRegExp::CaretAtZero);
        QString version = parsVersion.capturedTexts().at(0);

        if (version != "")
        {
            retValue = true;
            if (found.values().indexOf(version) == -1)
            {
                found[path] = version;
            }
        }
    }

    return retValue;
}

void SettingsDialog::showEvent(QShowEvent *)
{
    loadSettings();
}

void SettingsDialog::on_actionExit_triggered()
{
    this->close();
}

void SettingsDialog::on_pythonBrowse_clicked()
{
    if (QtUtils::browseFolderTrigger(pythonPath, this))
    {
        ui->pythonPathLine->setText(pythonPath);
    }
}

void SettingsDialog::updateSettings()
{
    projectName = ui->projectLine->text();
    cmakeVerion = ui->cmakeLineEdit->text();
    pythonPath = ui->pythonPathLine->text().replace("\\", "/");
    scriptPath = ui->scriptPathLine->text().replace("\\", "/");
    vsPath = ui->vsPathLine->text().replace("\\", "/");
    architecture = ui->archComboBox->currentText();
    outputPath = ui->outputPathLine->text().replace("\\", "/");
    buildPath = ui->buildPathLine->text().replace("\\", "/");

    settings->setValue("ProjectName", projectName);
    settings->setValue("cmakeVersion", cmakeVerion);
    settings->setValue("PythonPath", pythonPath);
    settings->setValue("VSPath", vsPath);
    settings->setValue("ScriptPath", scriptPath);
    settings->setValue("VSVersion", vsVersion);
    settings->setValue("Architecture", architecture);
    settings->setValue("OutputPath", outputPath);
    settings->setValue("BuildPath", buildPath);
}

void SettingsDialog::on_okPushButton_clicked()
{
    updateSettings();

    emit settingsChanged();

    close();
}

void SettingsDialog::on_vsBrowse_clicked()
{
    if (QtUtils::browseFolderTrigger(vsPath, this))
    {
        ui->vsPathLine->setText(vsPath);
    }
}

void SettingsDialog::on_scriptBrowse_clicked()
{
    if (QtUtils::browseFolderTrigger(scriptPath, this))
    {
        ui->scriptPathLine->setText(scriptPath);
    }
}

void SettingsDialog::on_outputBrowse_clicked()
{
    if (QtUtils::browseFolderTrigger(outputPath, this))
    {
        ui->outputPathLine->setText(outputPath);
    }
}

void SettingsDialog::on_buildPathBrowse_clicked()
{
    if (QtUtils::browseFolderTrigger(buildPath, this))
    {
        ui->buildPathLine->setText(buildPath);
    }
}
