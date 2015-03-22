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
 * @brief The SettingsDialog class
 */
class SettingsDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    bool isNeedToShow() { return needToShow; }

    /**
     * @brief getCmakeVersion
     * @return
     */
    QString getCmakeVersion()   const { return this->cmakeVerion; }
    QString getProjectName()    const { return this->projectName; }
    QString getPythonPath()     const { return this->pythonPath; }
    QString getVsPath()         const { return this->vsPath; }
    QString getVsVersion()      const { return this->vsVersion; }
    QString getArchitecture()   const { return this->architecture; }
    QString getOutputPath()     const { return this->outputPath; }
    QString getScriptPath()     const { return this->scriptPath; }
    QString getBuildPath()      const { return this->buildPath; }

    void updateSettings();
signals:
    void logSignal(const QString &logMessage);

    void settingsChanged();

public slots:
    void loadSettings();

    void clearSettings();

private slots:

    void on_actionExit_triggered();

    void on_pythonBrowse_clicked();

    void on_okPushButton_clicked();

    void on_vsBrowse_clicked();

    void on_scriptBrowse_clicked();

    void on_outputBrowse_clicked();

    void on_buildPathBrowse_clicked();

private:

    QSettings* settings;

    QProcess* processs;

    Ui::SettingsDialog *ui;

    bool findPath(const QString &execName, const QStringList &params, QRegExp &findRegExpr, QStringList &checkFolderPathsList, const QString &postPath,
                  const QRegExp &versionRegExpr, QMap<QString, QString> &found);

    inline bool isExists(const QString &path) {
        return QDir(path).exists();
    }

    QString cmakeVerion;
    QString projectName;
    QString pythonPath;
    QString scriptPath;
    QString vsPath;
    QString vsVersion;
    QString architecture;
    QString outputPath;
    QString buildPath;

    bool needToShow;

    bool alllSettingsLoaded;

    // QWidget interface
protected:
    void showEvent(QShowEvent *);
};

#endif // SETTINGSDIALOG_H
