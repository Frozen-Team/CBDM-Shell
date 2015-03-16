#ifndef CONFIGURABLEUI_H
#define CONFIGURABLEUI_H

#include <QObject>
#include <QWidget>
#include <QCheckBox>
#include <QVariant>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>
#include <QHBoxLayout>

#include "dialogutils.h"

// It is not necessary to delete ui components inside each componet. With addWidget method
// component's parent set and destroys with parent.

class BaseConfComponent : public QWidget
{
    Q_OBJECT
public:
    explicit BaseConfComponent(const QString &componentName, QWidget *parent = 0);

    const QVariant& getValue() { return value; }
    const QString& getComponentName() { return this->parameterName; }

    virtual QString type() { return QString("Base element"); }

public slots:
    void setValue(const QVariant &value) { this->value = value; }


public:
    QHBoxLayout layout;
    QVariant value;
    QString parameterName;
};

class ConfCheckBox : public BaseConfComponent
{
    Q_OBJECT
public:
    explicit ConfCheckBox(QWidget *parent = 0, const QString &name = QString("DefaultName"), const QString &text = QString("Default text"),
                          const QVariant &value = QVariant::fromValue(false), const QString& parameterName = QString("Param"));
    ~ConfCheckBox();

private slots:
    void checkBoxTriggered(bool value);

private:
    QCheckBox *checkBox;
    QSpacerItem *spacer;

    // BaseConfComponent interface
public:
    QString type() override { return QString("CheckBox"); }
};


class ConfLineEdit : public BaseConfComponent
{
    Q_OBJECT
public:
    explicit ConfLineEdit(QWidget *parent = 0, const QString &name = QString("DefaultName"), const QString &placeholderText = QString("Placeholder Text"),
                          const QVariant &value = QVariant::fromValue(""), const QString &parameterName  = QString("Param"));
    ~ConfLineEdit();

private:
    QLineEdit* lineEdit;

private slots:
    void lineEditTextChanged(QString text);

    // BaseConfComponent interface
public:
    QString type() override { return QString("LineEdit"); }
};


class ConfLabel : public BaseConfComponent
{
    Q_OBJECT
public:
    explicit ConfLabel(QWidget *parent = 0, const QString &name = QString("DefaultName"),
                          const QVariant &text = QVariant("Label Text"), const QString &parameterName = QString("Param"));
    ~ConfLabel();

private:
    QLabel* label;
    QSpacerItem *spacer;

    // BaseConfComponent interface
public:
    QString type() override { return QString("Label"); }
};

class ConfDirBrowser : public BaseConfComponent
{
    Q_OBJECT
public:
    explicit ConfDirBrowser(QWidget *parent = 0, const QString &name = QString("DefaultName"), const QString &placeholderText = QString("Default text"),
                            const QVariant &value = QVariant::fromValue("Default path"),
                            const QString &buttonText = QString("Browse"), const QString &parameterName1 = QString("Param"));
    ~ConfDirBrowser();

private slots:
    void pushButtonClicked();

private:
    QLineEdit* lineEdit;
    QPushButton* pushButton;

    // BaseConfComponent interface
public:
    QString type() override { return QString("DirBrowser"); }
};

class ConfUiManager : public QWidget
{
    Q_OBJECT

public:
    explicit ConfUiManager(QWidget* parent = nullptr);
    ~ConfUiManager();

public slots:
    void addComponent(QString &name, const QMap<QString, QVariant> &parameters);
    void deleteComponent(const QString &componentName);
    void deleteUi();

    QLayout *getLayout();


public:
    QVector<BaseConfComponent*> uiComponents;
    QVBoxLayout* layout;

};

#endif // CONFIGURABLEUI_H
