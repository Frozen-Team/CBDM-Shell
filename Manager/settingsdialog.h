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

class SettingsDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();


    void loadSettings();

    bool isNeedToShow() { return needToShow; }

    QString getPythonPath() const;
    QString getVsPath() const;
    QString getVsVersion() const;
    QString getArchitecture() const;
    QString getOutputPath() const;
    QString getScriptPath() const;

signals:
    void logSignal(const QString &logMessage);

    void settingsChanged();

private slots:

    void on_actionExit_triggered();

    void on_pythonBrowse_clicked();

    void on_okPushButton_clicked();

    void on_cancelPushButton_clicked();

    void on_vsBrowse_clicked();

    void on_scriptBrowse_clicked();

    void on_outputBrowse_clicked();

private:

    QSettings* settings;

    QProcess* processs;

    Ui::SettingsDialog *ui;

    bool findPath(const QString &execName, const QStringList &params, QRegExp &findRegExpr, QStringList &checkFolderPathsList, const QString &postPath,
                  const QRegExp &versionRegExpr, QMap<QString, QString> &found);

    inline bool isExists(const QString &path) {
        return QDir(path).exists();
    }

    QString pythonPath;
    QString vsPath;
    QString vsVersion;
    QString architecture;
    QString outputPath;
    QString scriptPath;

    bool needToShow;

    bool alllSettingsLoaded;

    // QWidget interface
protected:
    void showEvent(QShowEvent *);
};

#endif // SETTINGSDIALOG_H
