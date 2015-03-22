#ifndef DEPENDENCYMANAGERMAIN_H
#define DEPENDENCYMANAGERMAIN_H

#include <QMainWindow>

#include <QProcess>
#include <QMenu>
#include <QSettings>
#include <QStandardItemModel>
#include <QListView>

#include "settingsdialog.h"
#include "modulesettings.h"


namespace Ui {
class DependencyManagerMain;
}

class DependencyManagerMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit DependencyManagerMain(QWidget *parent = 0);
    ~DependencyManagerMain();

    QPoint getWindowPos() { return this->pos(); }
    void setWindowPos(const QPoint& p) { move(p); }

public slots:
    void showSettingsDialog();

    void showModuleSettingsDialog(const QString &moduleName);

    void reloadModules();

    void writeLog(QString &message);

    void configureModules();

private slots:

    void on_actionExit_triggered();

    void on_actionMinimize_triggered();

    void on_actionAbout_Qt_triggered();

    void loadModulesList(const QString &path);

    void updateModulesModels();

    void on_actionHelp_me_triggered();

    void on_availableListView_doubleClicked(const QModelIndex &index);

    void on_usedListView_doubleClicked(const QModelIndex &index);

    void on_allRightButton_clicked();

    void on_rightButton_clicked();

    void on_configureButton_clicked();

    void on_leftButton_clicked();

    void on_allLeftButton_clicked();

    void on_generateButton_clicked();

    void on_actionStartFromScratch_triggered();

private:
    ModuleSettings* moduleSettingsDialog;
    SettingsDialog* settingsDialog;

    QStringList modulesList;
    QStringList availableList;
    QStringList usedList;

    QStandardItemModel availableListModel;
    QStandardItemModel usedListModel;

    QListView* activeModulesList;

    Ui::DependencyManagerMain *ui;
};

#endif // DEPENDENCYMANAGERMAIN_H
