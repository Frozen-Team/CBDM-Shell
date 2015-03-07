#include "dependencymanagermain.h"
#include "ui_dependencymanagermain.h"

#include <QFile>
#include <QDir>
#include <QProcess>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#include <QDialog>

#include <QFontDatabase>
#include <QMessageBox>
#include <QSpacerItem>
#include <QToolButton>
#include <QSizeGrip>

#include <QStandardItemModel>





#include <QFont>
DependencyManagerMain::DependencyManagerMain(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint | Qt::Window), // Remove titlebar
    ui(new Ui::DependencyManagerMain)
{
    ui->setupUi(this);

    setupStyles();

    QMenuBar* windowBar = new QMenuBar(ui->menubar);
    QAction* closeAction = new QAction("X", this);
    //QMenu* maximizeAction = new QMenu("O", windowBar);

    //windowBar->addMenu(maximizeAction);
    windowBar->addAction(closeAction);

    closeAction->setObjectName("X");
    windowBar->setStyleSheet("::item {"
                             "margin-top: 0px;"
                             "background: #990000;"
                             "color: white"
                             "}"
                             "QMenuBar::item:pressed {"
                             "background: #FF0000;"
                             "color: white"
                             "}"
                             "::item:selected {"
                             "background: #CC0000;"
                             "color: white}"
                             );
    ui->menubar->setCornerWidget(windowBar);

////    menuWidget->setStyleSheet( "::item {"
////                               "background: #FF0000;"
////                               "color: white"
////                               "}"
////                                                            "QMenuBar::item:pressed {"
////                                                            "background: #FF0000;"
////                                                            "color: white"
////                                                            "}"
////                                                            "::item:selected {"
////                                                            "background: #CC0000;"
////                                                            "color: white}");

//        // set the custom widget as the main window's menu widget
//    setMenuWidget(menuWidget);


    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));




    QStandardItemModel* model = new QStandardItemModel(this);

    ui->listView->setModel(model);

    for (int i = 0; i < 50; ++i)
    {
        QStandardItem *item = new QStandardItem("Item   " + QString::number(i));

        model->appendRow(item);
    }



//    ui->comboBox->addItem("Hello");
//    ui->comboBox->addItem("Hel789789987 6");
//    ui->comboBox->addItem("He");
//    ui->comboBox->addItem(QIcon(":/res/Logo(white).png"), "235324636");

    ui->archComboBox->addItem("x86");
    ui->archComboBox->addItem("x64");
    ui->archComboBox->setCurrentIndex(0);


    int fontID(-1);

    QFile res(":/res/Resources/ONRAMP.ttf");
        if (res.open(QIODevice::ReadOnly) == false) {
            //if (fontWarningShown == false) {
            //    QMessageBox::warning(0, "Application", (QString)"Impossible d'ouvrir la police " + QChar(0x00AB) + " DejaVu Serif " + QChar(0x00BB) + ".");
            //    fontWarningShown = true;
            //}
        } else {
            fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
            if (fontID == -1) {
                QMessageBox::warning(0, "Application", (QString)"Impossible d'ouvrir la police " + QChar(0x00AB) + " DejaVu Serif " + QChar(0x00BB) + ".");
            }
        }

        //setFont(QFont("ONRANP", 13));
}



DependencyManagerMain::~DependencyManagerMain()
{
    delete ui;
}

void DependencyManagerMain::setupStyles()
{

}

void DependencyManagerMain::on_pushButton_clicked()
{

    //ui->label_5->setText("<font color = green>HIII!!!<>");


    //this->setStyleSheet(stylesString);
}

void DependencyManagerMain::on_actionRebuild_triggered()
{
    QDir dir(QDir::currentPath() + "/debug/Styles/");
    qDebug() << dir.currentPath();
    QStringList filter("*.css");
    QFileInfoList stylesFilesList = dir.entryInfoList(filter, QDir::Files);

    QString stylesString;

    for (QFileInfo &fileInfo : stylesFilesList)
    {
        QFile file(fileInfo.filePath());
        if (file.open(QFile::ReadOnly))
        {
            QTextStream stream(&file);
            stylesString += stream.readAll();
        } else
        {
            qDebug() << "Unable read file: " << fileInfo.filePath();
        }

        file.close();
    }

    this->setStyleSheet(stylesString);
}

void DependencyManagerMain::on_actionExit_triggered()
{
    close();
}



void DependencyManagerMain::on_pythonBrowse_clicked()
{
    QString str(qgetenv("Path"));
    //qDebug() << findPythonPath(str);
    findPythonPath(str, str);

}



void DependencyManagerMain::process()
{
    QString readString = processs->readAll();

    qDebug() << "My output: " << readString;
}

void DependencyManagerMain::bytesRead(qint64 a)
{
    //qDebug() << "Read bytes" << a;
}

bool DependencyManagerMain::findPythonPath(QString &pythonPath, QString &version)
{
    // Trying to find in system variables
    QStringList paths(QString(qgetenv("Path")).split(";"));
    QStringList checkFoldersList(QStringList() << "" << "Program Files" << "Program Files (x86)");
    QRegExp pythonFindExpr("^(.*)Python[3-9][\\d]+((\\\\$)|(//$)|$)");

    QStringList foundPaths;
    for (QString &path : paths)
    {
        if (path.contains(pythonFindExpr))
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
                if (dirInfo.absoluteFilePath().contains(pythonFindExpr))
                {
                    foundPaths << dirInfo.absoluteFilePath();
                }
            }
        }
    }

    processs = new QProcess(this);

    QString program("python");
    QStringList params(QStringList() << "-V");
    //process.readAllStandardOutput();

    processs->setProcessChannelMode(QProcess::MergedChannels);
    processs->start(program, params);;


    connect(processs, SIGNAL(readyRead()), this, SLOT(process()));
    connect(processs, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesRead(qint64)));

    //processs->waitForFinished();
    //process.waitForReadyRead();


    //qDebug() << foundPaths;



    return 0;
}
