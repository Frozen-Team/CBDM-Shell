#include "dependencymanagermain.h"
#include <QApplication>

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

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

    //this->setStyleSheet(stylesString);
    qDebug() << stylesString;

    a.setStyleSheet(stylesString);

    return a.exec();
}
