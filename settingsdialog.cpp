#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>
#include <QFileDialog>
#include <QMessageBox>
#include "closemenubar.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint | Qt::Window), // Remove titlebar and border
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    CloseMenuBar* closeMenu = new CloseMenuBar(ui->menubar);
    ui->menubar->setCornerWidget(closeMenu);
    connect(closeMenu, SIGNAL(onClose()), this, SLOT(close()));
    ui->menubar->setCornerWidget(closeMenu);
    ui->menubar->setTitle("Settings");

    // Loading settings
    settings = new QSettings("configuration.ini", QSettings::IniFormat, this);

    ui->archComboBox->addItem("x86");
    ui->archComboBox->addItem("x64");
    ui->archComboBox->setCurrentIndex(1); // x64 by default
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
    {15, "Microsoft Visual Studio 2008"},
    {16, "Microsoft Visual Studio 2010"},
    {17, "Microsoft Visual Studio 2012"},
    {18, "Microsoft Visual Studio 2013"}};

void SettingsDialog::loadSettings()
{
    QString path;

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
            if (this->isHidden())
            {
                show();
            }
        }
    }

    if ((settings->contains("VSPath")) && ((path = settings->value("VSPath").toString()) != QString("")) && isExists(path))
    {
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
            ui->vsPathLine->setText(pathsMap.keys().last());
        } else {
            if (this->isHidden())
            {
                show();
            }
        }
    }

    if ((settings->contains("ScriptPath")) && ((path = settings->value("ScriptPath").toString()) != QString("")) && isExists(path))
    {
        ui->scriptFolderPathLine->setText(path);
    } else
    {
        QMap<QString, QString> pathsMap;
        if (findPath("cl.exe", QStringList(), QRegExp("VC"),
                     QStringList() << "Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\Bin" << "Applications\\", "", QRegExp("((\\d+).?){3}"), pathsMap))
        {
            vsPath = pathsMap.keys().last();
            // Cut path:
            vsPath = vsPath.mid(0, vsPath.length() - 6);
            QRegExp parsVSVer("\\d.");
            parsVSVer.indexIn(pathsMap[vsPath]);
            vsVersion = vsVersionMap[parsVSVer.capturedTexts().at(0).toInt()];
            ui->vsPathLine->setText(vsPath);
        } else {
            if (this->isHidden())
            {
                show();
            }
        }
    }

    if ((settings->contains("OutputPath")) && ((path = settings->value("OutputPath").toString()) != QString("")) && isExists(path))
    {
        outputPath = path;
        ui->outputPathLine->setText(path);
    } else
    {
        // Check if window is already showed at this moment. If not then show
        if (this->isHidden())
        {
            show();
        }
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
    QFileDialog dialog;

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);

    // User is BOSS
    if (dialog.exec() != 0)
    {
        pythonPath = dialog.selectedFiles().at(0);
        ui->pythonPathLine->setText(pythonPath);
    }
}

void SettingsDialog::on_okPushButton_clicked()
{
    settings->setValue("PythonPath", pythonPath);
    settings->setValue("VSPath", vsPath);
    settings->setValue("VSVersion", vsVersion);
    settings->setValue("OutputPath", outputPath);

    settings->setValue("OutputPath", outputPath);
    close();
}

void SettingsDialog::on_cancelPushButton_clicked()
{
    close();
}

void SettingsDialog::on_outputPathLine_textChanged(const QString &arg1)
{
    outputPath = arg1;
}