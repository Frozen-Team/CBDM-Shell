#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include "dialogutils.h"

const QMap<QString, QString> AVAILABLE_ARCHS = {{"x86", "x86"}, {"x64", "x64"}}; // for reason if different values of archs

SettingsDialog::SettingsDialog(QWidget *parent) :
    ui(new Ui::SettingsDialog), QMainWindow(parent)
{
    ui->setupUi(this);

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
    {15, "Microsoft Visual Studio 2008"}, // Is not reccomended!
    {16, "Microsoft Visual Studio 2010"},
    {17, "Microsoft Visual Studio 2012"},
    {18, "Microsoft Visual Studio 2013"}};

void SettingsDialog::loadSettings()
{
    QString path;

    // Check for python
    if ((settings->contains("PythonPath")) && ((path = settings->value("PythonPath").toString()) != QString("")) && isExists(path))
    {
        ui->pythonPathLine->setText(path);
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
    QString vsVersionStr;
    if ((settings->contains("VSPath")) && ((path = settings->value("VSPath").toString()) != QString("")) && isExists(path) &&
            ((settings->contains("VSVersion")) && (vsVersionStr = settings->value("VSVersion").toString()) != QString("")))
    {
        ui->vsVersionLabel->setText(vsVersionStr);
        ui->vsPathLine->setText(path);
    } else
    {
        QMap<QString, QString> pathsMap;
        if (findPath("cl.exe", QStringList(), QRegExp("VC"),
                     QStringList() << "Program Files (x86)\\Microsoft Visual Studio 12.0\\" << "Applications\\", "", QRegExp("((\\d+).?){3}"), pathsMap))
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
    QString arch;
    if ((settings->contains("Architecture")) && ((arch = settings->value("Architecture").toString()) != QString("")))
    {
        architecture = arch;
        ui->archComboBox->setCurrentText(arch);
    } else
    {
        needToShow = true;
    }


    // Check for Script Path
    //"D:\\YandexDisk\\FrozenTeam\\CPPDependenciesManager"
    if ((settings->contains("ScriptPath")) && ((path = settings->value("ScriptPath").toString()) != QString("")) && isExists(path))
    {
        scriptPath = path;
        ui->scriptPathLine->setText(path);
    } else
    {
        needToShow = true;
    }


    // Check for Output Path
    if ((settings->contains("OutputPath")) && ((path = settings->value("OutputPath").toString()) != QString("")))
    {
        outputPath = path;
        ui->outputPathLine->setText(path);
    } else
    {
        needToShow = true;
    }
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
QString SettingsDialog::getScriptPath() const
{
    return scriptPath;
}

QString SettingsDialog::getOutputPath() const
{
    return outputPath;
}

QString SettingsDialog::getArchitecture() const
{
    return architecture;
}

QString SettingsDialog::getVsVersion() const
{
    return vsVersion;
}

QString SettingsDialog::getVsPath() const
{
    return vsPath;
}

QString SettingsDialog::getPythonPath() const
{
    return pythonPath;
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
    if (browseFolderTrigger(pythonPath, this))
    {
        ui->pythonPathLine->setText(pythonPath);
    }
}

void SettingsDialog::on_okPushButton_clicked()
{
    qDebug() << "Emitted";

    pythonPath = ui->pythonPathLine->text();
    vsPath = ui->vsPathLine->text();
    architecture = ui->archComboBox->currentText();
    outputPath = ui->outputPathLine->text();
    scriptPath = ui->scriptPathLine->text();

    settings->setValue("PythonPath", pythonPath);
    settings->setValue("VSPath", vsPath);
    settings->setValue("ScriptPath", scriptPath);
    settings->setValue("VSVersion", vsVersion);
    settings->setValue("Architecture", architecture);
    settings->setValue("OutputPath", outputPath);

    emit settingsChanged();

    close();
}

void SettingsDialog::on_cancelPushButton_clicked()
{
    close();
}

void SettingsDialog::on_vsBrowse_clicked()
{
    if (browseFolderTrigger(vsPath, this))
    {
        ui->vsPathLine->setText(vsPath);
    }
}

void SettingsDialog::on_scriptBrowse_clicked()
{
    if (browseFolderTrigger(scriptPath, this))
    {
        ui->scriptPathLine->setText(scriptPath);
    }
}

void SettingsDialog::on_outputBrowse_clicked()
{
    if (browseFolderTrigger(outputPath, this))
    {
        ui->outputPathLine->setText(outputPath);
    }
}
