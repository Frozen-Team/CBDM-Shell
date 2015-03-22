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
#include <QSettings>

#include "qtutils.h"

#ifdef STRS
#error "STRS already defined somewhere"
#endif

/**
 *
 */
namespace STRS {

static const QString defaultName = "DefaultName";
static const QString defaultText = "Default Text";
static const QString undefined   = "Undefined";
static const QString baseElement = "BaseElement";

}

class BaseConfComponent : public QWidget
{
    Q_OBJECT
public:
    explicit BaseConfComponent(const QString &parameterName, QWidget *parent = 0);

    enum ComponentsTypes {
        BASE = 0, CHECK_BOX = 1, LINE_EDIT = 2, LABEL = 3, DIR_BROWSER = 4
    };

    virtual QString type()      { return STRS::baseElement; }
    virtual QString getName()   { return STRS::undefined; }
    virtual bool hasValue()     { return false; }

    QString getParameterName() const    { return this->parameterName; }
    QHBoxLayout *getLayout()            { return &layout; }
    virtual const QVariant& getValue()          { return value; }

public slots:
    void setValue(const QVariant &value)        { this->value = value; }
    void setParameterName(const QString &value) { this->parameterName = value; }

protected:
    QHBoxLayout layout;
    QVariant value;
    QString parameterName;
};

class ConfCheckBox : public BaseConfComponent
{
    Q_OBJECT
public:
    explicit ConfCheckBox(QWidget *parent = 0, const QString &name = STRS::defaultName, const QString &text = STRS::defaultText,
                          const QVariant &value = QVariant::fromValue(false), const QString& parameterName = QString("Param"));
    ~ConfCheckBox();

private slots:
    void checkBoxTriggered(bool value);

private:
    QCheckBox *checkBox;
    QSpacerItem *spacer;
    QVariant boolValue;

    // BaseConfComponent interface
public:
    QString type() override { return QString("CheckBox"); }
    QString getName() override;
    bool hasValue() override { return true; }

    // BaseConfComponent interface
public:
    const QVariant &getValue();
};


class ConfLineEdit : public BaseConfComponent
{
    Q_OBJECT
public:
    explicit ConfLineEdit(QWidget *parent = 0, const QString &name = STRS::defaultName, const QString &placeholderText = QString("Placeholder Text"),
                          const QVariant &value = QVariant::fromValue(""), QString &mask = QString("*"), const QString &parameterName  = QString("Param"));
    ~ConfLineEdit();

private:
    QLineEdit* lineEdit;

private slots:
    void lineEditTextChanged(QString text);

    // BaseConfComponent interface
public:
    QString type() override { return QString("LineEdit"); }
    QString getName();
    bool hasValue() override { return true; }
};


class ConfLabel : public BaseConfComponent
{
    Q_OBJECT
public:
    explicit ConfLabel(QWidget *parent = 0, const QString &name = STRS::defaultName,
                          const QVariant &text = QVariant("Label Text"));
    ~ConfLabel();

private:
    QLabel* label;
    QSpacerItem *spacer;

    // BaseConfComponent interface
public:
    QString type() override { return QString("Label"); }
    QString getName() override { return QString("DefaultLabel"); }
    bool hasValue() override { return false; }
};

class ConfDirBrowser : public BaseConfComponent
{
    Q_OBJECT
public:
    explicit ConfDirBrowser(QWidget *parent = 0, const QString &name = STRS::defaultName, const QString &placeholderText = QString("Default text"),
                            const QVariant &value = QVariant::fromValue("Default path"),
                            const QString &buttonText = QString("Browse"), const QString &parameterName = QString("Param"));
    ~ConfDirBrowser();

private slots:
    void pushButtonClicked();
    void lineEditTextChanged(QString text);

private:
    QLineEdit* lineEdit;
    QPushButton* pushButton;

    // BaseConfComponent interface
public:
    QString type() override { return QString("DirBrowser"); }
    QString getName() override;
    bool hasValue() override { return true; }
};

class ConfUiManager : public QWidget
{
    Q_OBJECT
public:
    explicit ConfUiManager(QWidget* parent = nullptr);
    ~ConfUiManager();

public slots:
    void constructUi(QSettings **iniConf);
    void addComponent(QString &name, const QMap<QString, QVariant> &parameters);
    void deleteComponent(const QString &name);
    void saveParameters();

    QLayout *getLayout();
private:
    QVector<BaseConfComponent *> uiComponents;

public:

    QVBoxLayout* layout;
    const QVector<BaseConfComponent *>* getUiComponents() const { return &this->uiComponents; }

private:
    QSettings **iniConf;
};

#endif // CONFIGURABLEUI_H
