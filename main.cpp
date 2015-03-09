#include "dependencymanagermain.h"
#include <QApplication>

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QFontDatabase>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    DependencyManagerMain w;
    w.show();

//    QMessageBox::information(nullptr, "Hello", argc + "");

//    for (int i = 0; i < argc; ++i)
//    {
//        QMessageBox::critical(nullptr, "Hello", argv[i]);
//        qDebug() << argv[i];
//    }

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

    a.setStyleSheet(stylesString);

    int fontID(-1);

    QFile res(":/res/Resources/ONRAMP.ttf");
    if (res.open(QIODevice::ReadOnly) == false) {
        QMessageBox::warning(0, "Application", "Cannot open device for reading font.");
    } else {
        fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
        if (fontID == -1) {
            QMessageBox::warning(0, "Application", "Cannot read font");
        }
    }
    a.setFont(QFont("ONRANP", 13));

    return a.exec();
}
