#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QMainWindow>
#include <QSettings>
#include <QProcess>
#include <QRegExp>
#include <QDir>

namespace Ui {
class SettingsDialog;
}
/**
 * @brief The SettingsDialog class describes Settings dialog.
 */
class SettingsDialog : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief SettingsDialog Constructor  for dialog
     * @param parent Parent object.
     */
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    /**
     * @brief isNeedToShow Checks if dialog needed to show.
     * @return true if
     */
    bool isNeedToShow() { return needToShow; }

    /**
     * @brief getCmakeVersion Gets cmake version.
     * @return cmake version string.
     */
    QString getCmakeVersion()   const { return this->cmakeVerion; }
    /**
     * @brief getProjectName Gets project name.
     * @return project name string.
     */
    QString getProjectName()    const { return this->projectName; }
    /**
     * @brief getPythonPath Gets python path.
     * @return Python path.
     */
    QString getPythonPath()     const { return this->pythonPath; }
    /**
     * @brief getVsPath Gets Visual Studio path
     * @return Visual Studio path string.
     */
    QString getVsPath()         const { return this->vsPath; }
    /**
     * @brief getVsVersion Gets Visual Studio version.
     * @return Visual Studio version string.
     */
    QString getVsVersion()      const { return this->vsVersion; }
    /**
     * @brief getArchitecture Gets architecture.
     * @return architecture string.
     */
    QString getArchitecture()   const { return this->architecture; }
    /**
     * @brief getOutputPath Gets output path.
     * @return output path.
     */
    QString getOutputPath()     const { return this->outputPath; }
    /**
     * @brief getScriptPath Gets script path.
     * @return script path.
     */
    QString getScriptPath()     const { return this->scriptPath; }
    /**
     * @brief getBuildPath Gets build path.
     * @return build path.
     */
    QString getBuildPath()      const { return this->buildPath; }
    /**
     * @brief updateSettings Updates settings file.
     */
    void updateSettings();
signals:
    /**
     * @brief logSignal Log signal for log output.
     * @param logMessage Message that will be logged.
     */
    void logSignal(const QString &logMessage);
    /**
     * @brief settingsChanged Signal when some settings changed.
     */
    void settingsChanged();

public slots:
    /**
     * @brief loadSettings Event to load settings.
     */
    void loadSettings();
    /**
     * @brief clearSettings Event to clear settings.
     */
    void clearSettings();

private slots:
    /**
     * @brief on_actionExit_triggered Event when Exit triggered.
     */
    void on_actionExit_triggered();
    /**
     * @brief on_pythonBrowse_clicked Event when pythonBrowse clicked.
     */
    void on_pythonBrowse_clicked();
    /**
     * @brief on_okPushButton_clicked Event when okPushButton clicked.
     */
    void on_okPushButton_clicked();
    /**
     * @brief on_vsBrowse_clicked Event when vsBrowse clicked.
     */
    void on_vsBrowse_clicked();
    /**
     * @brief on_scriptBrowse_clicked Event when scriptBrowse clicked.
     */
    void on_scriptBrowse_clicked();
    /**
     * @brief on_outputBrowse_clicked Event when outputBrowse clicked.
     */
    void on_outputBrowse_clicked();
    /**
     * @brief on_buildPathBrowse_clicked Event when buildPathBrowse clicked.
     */
    void on_buildPathBrowse_clicked();

private:
    /**
     * @brief settings Pointer to settings.
     */
    QSettings* settings;
    /**
     * @brief processs Pointer to process runner.
     */
    QProcess* processs;
    /**
     * @brief ui Ui components on the form.
     */
    Ui::SettingsDialog *ui;
    /**
     * @brief findPath Method search path with complex parameters. Searchs visual studio path, python path etc.
     * @param execName Executabe name.
     * @param params Parameters for executable.
     * @param findRegExpr Find regular expression.
     * @param checkFolderPathsList Paths to check for searching.
     * @param postPath Postfix path.
     * @param versionRegExpr Regular expression for version capturing.
     * @param found There will be found paths.
     * @return true if found otherwise false.
     */
    bool findPath(const QString &execName, const QStringList &params, QRegExp &findRegExpr, QStringList &checkFolderPathsList, const QString &postPath,
                  const QRegExp &versionRegExpr, QMap<QString, QString> &found);
    /**
     * @brief isExists Checks if path exists.
     * @param path Path to check.
     * @return true if exists otherwise false.
     */
    inline bool isExists(const QString &path) {
        return QDir(path).exists();
    }
    /**
     * @brief cmakeVerion Cmake version string.
     */
    QString cmakeVerion;
    /**
     * @brief projectName Project name string.
     */
    QString projectName;
    /**
     * @brief pythonPath Python path string.
     */
    QString pythonPath;
    /**
     * @brief scriptPath Script path string.
     */
    QString scriptPath;
    /**
     * @brief vsPath Visual Studio path string.
     */
    QString vsPath;
    /**
     * @brief vsVersion Visual Studio string.
     */
    QString vsVersion;
    /**
     * @brief architecture Architecture string.
     */
    QString architecture;
    /**
     * @brief outputPath Output path string.
     */
    QString outputPath;
    /**
     * @brief buildPath Build path string.
     */
    QString buildPath;
    /**
     * @brief needToShow Need to show flag.
     */
    bool needToShow;
    /**
     * @brief alllSettingsLoaded All settings loaded flag.
     */
    bool alllSettingsLoaded;

    // QWidget interface
protected:
    /**
     * @brief showEvent Reimplemented show event for loading settings->
     */
    void showEvent(QShowEvent *);
};

#endif // SETTINGSDIALOG_H
