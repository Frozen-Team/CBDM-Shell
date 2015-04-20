#ifndef MODULESETTINGS_H
#define MODULESETTINGS_H

#include <QMainWindow>
#include <QSettings>
#include "configurableui.h"

namespace Ui {
class ModuleSettings;
}

/**
 * @brief The ModuleSettings class describes Module settings dialog.
 */
class ModuleSettings : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief ModuleSettings Constructor for dialog.
     * @param parent Parent object.
     */
    explicit ModuleSettings(QWidget *parent = 0);
    /**
     * @brief loadUi Loads ui.
     * @param modulePath Path for loading ui.
     * @return true if everythin is ok.
     */
    bool loadUi(const QString &modulePath);
    ~ModuleSettings();

private:
    /**
     * @brief ui Ui components on the Module Settings dialog.
     */
    Ui::ModuleSettings *ui;

public slots:
    /**
     * @brief getModuleValues Getter for values map of all ui components
     * @param modulePath Path to moduele.
     * @param valuesMap Key-vales of all component.
     * @return true if all is ok.
     */
    bool getModuleValues(const QString &modulePath, QMap<QString, QVariant> *valuesMap);

private slots:
    /**
     * @brief saveAndClose Saves all parameters and close dialog.
     */
    void saveAndClose();

private:
    /**
     * @brief settings Pointer to QSettings
     */
    QSettings* settings;
    /**
     * @brief uiManager Ui manager instance.
     */
    ConfUiManager uiManager;
};

#endif // MODULESETTINGS_H
