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

    QFile stylesFile(":/res/Resources/Styles/styles.css");
    if (stylesFile.open(QFile::ReadOnly))
    {
        QTextStream textStream(&stylesFile);
        a.setStyleSheet(textStream.readAll());
    } else
    {
        QMessageBox::warning(0, "Application", "Cannot open device for reading stylesheet");
    }

    int fontID(-1);
    QFile res(":/res/Resources/OpenSans-Bold.ttf");
    if (res.open(QIODevice::ReadOnly) == false) {
        QMessageBox::warning(0, "Application", "Cannot open device for reading font.");
    } else {
        fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
        if (fontID == -1) {
            QMessageBox::warning(0, "Application", "Cannot read font");
        }
    }
    QApplication::setFont(QFont("Open Sans", 10));

    return a.exec();
}
