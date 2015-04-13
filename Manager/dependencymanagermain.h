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

/**
 * @brief The DependencyManagerMain class specifies main ui class for Manager.
 */
class DependencyManagerMain : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief DependencyManagerMain Constructor for the main form.
     * @param parent Specifies paranet form.
     */
    explicit DependencyManagerMain(QWidget *parent = 0);
    ~DependencyManagerMain();

    /**
     * @brief getWindowPos Getter for position.
     * @return Current form position.
     */
    QPoint getWindowPos() { return this->pos(); }
    /**
     * @brief setWindowPos Setter for position.
     * @param p Position.
     */
    void setWindowPos(const QPoint& p) { move(p); }

public slots:
    /**
     * @brief showSettingsDialog Event when SettingDialog is showed.
     */
    void showSettingsDialog();

    /**
     * @brief showModuleSettingsDialog Event when ModuleSettingsDialog is showed.
     * @param moduleName
     */
    void showModuleSettingsDialog(const QString &moduleName);

    /**
     * @brief reloadModules Event when reloads all modules.
     */
    void reloadModules();

    /**
     * @brief writeLog Event to write logs in log component.
     * @param message Log message.
     */
    void writeLog(QString &message);

    /**
     * @brief configureModules Event to reconfigure all modules.
     */
    void configureModules();

private slots:

    /**
     * @brief on_actionExit_triggered Event when exit triggered.
     */
    void on_actionExit_triggered();
    /**
     * @brief on_actionMinimize_triggered Event when Minimize triggered.
     */
    void on_actionMinimize_triggered();
    /**
     * @brief on_actionAbout_Qt_triggeredEvent when aboutQt triggered.
     */
    void on_actionAbout_Qt_triggered();
    /**
     * @brief loadModulesList Event when need toad modules list.
     * @param path By path.
     */
    void loadModulesList(const QString &path);
    /**
     * @brief updateModulesModels Event when need update modules ui models.
     */
    void updateModulesModels();
    /**
     * @brief on_actionHelp_me_triggered Event when Help triggered.
     */
    void on_actionHelp_me_triggered();
    /**
     * @brief on_availableListView_doubleClicked Event when availableListView double clicked triggered.
     * @param index Index of clicked element.
     */
    void on_availableListView_doubleClicked(const QModelIndex &index);
    /**
     * @brief on_usedListView_doubleClicked Event when usedListView double clicked triggered.
     * @param index Index of clicked element.
     */
    void on_usedListView_doubleClicked(const QModelIndex &index);
    /**
     * @brief on_allRightButton_clicked Event when allRightButton clicked triggered.
     */
    void on_allRightButton_clicked();
    /**
     * @brief on_rightButton_clicked Event when rightButton clicked triggered.
     */
    void on_rightButton_clicked();
    /**
     * @brief on_configureButton_clicked Event when configureButton clicked triggered.
     */
    void on_configureButton_clicked();
    /**
     * @brief on_leftButton_clicked Event when configureButton clicked triggered.
     */
    void on_leftButton_clicked();
    /**
     * @brief on_allLeftButton_clicked Event when allLeftButton clicked triggered.
     */
    void on_allLeftButton_clicked();
    /**
     * @brief on_generateButton_clicked Event when generateButton clicked triggered.
     */
    void on_generateButton_clicked();
    /**
     * @brief on_actionStartFromScratch_triggered Event when actionStartFromScratch clicked triggered.
     */
    void on_actionStartFromScratch_triggered();

private:
    /**
     * @brief moduleSettingsDialog Modules settings dialog.
     */
    ModuleSettings* moduleSettingsDialog;
    /**
     * @brief settingsDialog Settings dialog.
     */
    SettingsDialog* settingsDialog;
    /**
     * @brief modulesList Modules list.
     */
    QStringList modulesList;
    /**
     * @brief availableList Available list.
     */
    QStringList availableList;
    /**
     * @brief usedList Used list model.
     */
    QStringList usedList;
    /**
     * @brief availableListModel Available list model.
     */
    QStandardItemModel availableListModel;
    /**
     * @brief usedListModel Used list model.
     */
    QStandardItemModel usedListModel;
    /**
     * @brief activeModulesList Current active list.
     */
    QListView* activeModulesList;
    /**
     * @brief ui Ui elements on the form.
     */
    Ui::DependencyManagerMain *ui;
};

#endif // DEPENDENCYMANAGERMAIN_H
