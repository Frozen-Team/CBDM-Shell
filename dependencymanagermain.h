#ifndef DEPENDENCYMANAGERMAIN_H
#define DEPENDENCYMANAGERMAIN_H

#include <QMainWindow>

#include <QProcess>


namespace Ui {
class DependencyManagerMain;
}

class DependencyManagerMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit DependencyManagerMain(QWidget *parent = 0);
    ~DependencyManagerMain();

private slots:
    void on_pushButton_clicked();

    void on_actionRebuild_triggered();

    void on_actionExit_triggered();


    void onHover(QAction *action);

    void on_pythonBrowse_clicked();

private slots:
    void process();
    void bytesRead(qint64 a);

private:
    QProcess* processs;

    Ui::DependencyManagerMain *ui;

    bool findPythonPath(QString &pythonPath, QString &version);

    void setupStyles();
};

#endif // DEPENDENCYMANAGERMAIN_H
