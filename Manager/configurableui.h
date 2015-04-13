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


namespace STRS {

static const QString defaultName = "DefaultName";
static const QString defaultText = "Default Text";
static const QString undefined   = "Undefined";
static const QString baseElement = "BaseElement";

}

/**
 * @brief The BaseConfComponent class Describes basic component.
 *
 * This class describes basic component for configurable User Interface
 * for the Module Settings Dialog. *
 *
 */
class BaseConfComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief BaseConfComponent Constructs basic component.
     * @param parameterName Defines name of the parameter tha will be generated
     * for python configuration.
     * @param parent Specifies parent object for basic component.
     */
    explicit BaseConfComponent(const QString &parameterName, QWidget *parent = 0);

    /**
     * @brief The ComponentsTypes enum describes all possible basic components
     * in form of enumaration.
     */
    enum ComponentsTypes {
        BASE = 0, CHECK_BOX = 1, LINE_EDIT = 2, LABEL = 3, DIR_BROWSER = 4
    };

    /**
     * @brief type method which must be reimplemented in each component's class.
     * @return type of component in string form.
     */
    virtual QString type()      { return STRS::baseElement; }

    /**
     * @brief getName method which must be reimplemented in each component's class.
     * @return name of component.
     */
    virtual QString getName()   { return STRS::undefined; }
    /**
     * @brief hasValue Chechs if component can contain a value.
     * @return true if can, false if not.
     */
    virtual bool hasValue()     { return false; }

    /**
     * @brief getParameterName method for getting parameter name.
     * @return name of parameter for python configuration.
     */
    QString getParameterName() const    { return this->parameterName; }
    /**
     * @brief getLayout method for getting widget of stored component.
     * @return layout of component.
     */
    QHBoxLayout *getLayout()            { return &layout; }
    /**
     * @brief getValue method which must be reimplemented in each component's class.
     * @return value for python configuration.
     */
    virtual const QVariant& getValue()          { return value; }

public slots:
    /**
     * @brief setValue slot for setting value.
     * @param value Value to be set.
     */
    void setValue(const QVariant &value)        { this->value = value; }
    /**
     * @brief setParameterName slot for setting parameter name.
     * @param value Parameter name to be set.
     */
    void setParameterName(const QString &value) { this->parameterName = value; }

protected:
    /**
     * @brief layout Layout of component.
     */
    QHBoxLayout layout;
    /**
     * @brief value Value for component.
     */
    QVariant value;
    /**
     * @brief parameterName Parameter name for component.
     */
    QString parameterName;
};

/**
 * @brief The ConfCheckBox class describes CheckBox component.
 */
class ConfCheckBox : public BaseConfComponent
{
    Q_OBJECT
public:
    /**
     * @brief ConfCheckBox Constructor for CheckBox.
     * @param parent Parent object for CheckBox component.
     * @param name Name of CheckBox.
     * @param text Caption of CheckBox.
     * @param value Initial value.
     * @param parameterName Name of the parameter.
     */
    explicit ConfCheckBox(QWidget *parent = 0, const QString &name = STRS::defaultName, const QString &text = STRS::defaultText,
                          const QVariant &value = QVariant::fromValue(false), const QString& parameterName = QString("Param"));
    /**
     * Destructor of CheckBox
     */
    ~ConfCheckBox();

private slots:
    /**
     * @brief checkBoxTriggered Event when ChekcBox is triggered.
     * @param value Current value.
     */
    void checkBoxTriggered(bool value);

private:
    /**
     * @brief checkBox CheckBox pointer.
     */
    QCheckBox *checkBox;
    /**
     * @brief spacer Spacer component for layout.
     */
    QSpacerItem *spacer;
    /**
     * @brief boolValue Current value of CheckBox.
     */
    QVariant boolValue;

    // BaseConfComponent interface
public:
    /**
     * @brief type CheckBox type
     * @return QString("CheckBox").
     */
    QString type() override { return QString("CheckBox"); }
    /**
     * @brief getName Getter for component name.
     * @return name of CheckBox.
     */
    QString getName() override;
    bool hasValue() override { return true; }

    // BaseConfComponent interface
public:
    /**
     * @brief getValue Getter for CheckBox value.
     * @return CheckBox value.
     */
    const QVariant &getValue();
};

/**
 * @brief The ConfLineEdit class describes LineEdit component.
 */
class ConfLineEdit : public BaseConfComponent
{
    Q_OBJECT
public:
    /**
     * @brief ConfLineEdit Constructor for LineEdit component.
     * @param parent specifies parent object.
     * @param name Name of component.
     * @param placeholderText Placeholder text of component.
     * @param value Value of component.
     * @param mask Mask for input events.
     * @param parameterName Parameter name for scripts.
     */
    explicit ConfLineEdit(QWidget *parent = 0, const QString &name = STRS::defaultName, const QString &placeholderText = QString("Placeholder Text"),
                          const QVariant &value = QVariant::fromValue(""), QString &mask = QString("*"), const QString &parameterName  = QString("Param"));
    ~ConfLineEdit();

private:
    /**
     * @brief lineEdit Pointer to QLineEdit
     */
    QLineEdit* lineEdit;

private slots:
    /**
     * @brief lineEditTextChanged Event when text of line edit is changed.
     * @param text Current text of LineEdit.
     */
    void lineEditTextChanged(QString text);

    // BaseConfComponent interface
public:
    /**
     * @brief type Coomponent typpe.
     * @return QString("LineEdit").
     */
    QString type() override { return QString("LineEdit"); }
    /**
     * @brief getName Getter for name.
     * @return Name of the LineEdit.
     */
    QString getName();
    /**
     * @brief hasValue Check if LineEdit has value.
     * @return true.
     */
    bool hasValue() override { return true; }
};

/**
 * @brief The ConfLabel class describes Label component.
 */
class ConfLabel : public BaseConfComponent
{
    Q_OBJECT
public:
    /**
     * @brief ConfLabel Constructor for Label component.
     * @param parent Specifies parent object.
     * @param name Sets Label component name.
     * @param text Sets caption of the object.
     */
    explicit ConfLabel(QWidget *parent = 0, const QString &name = STRS::defaultName,
                          const QVariant &text = QVariant("Label Text"));
    ~ConfLabel();

private:
    /**
     * @brief label Pointer to QLabel.
     */
    QLabel* label;
    /**
     * @brief spacer Pointer to spacer component QSpacer for layout.
     */
    QSpacerItem *spacer;

    // BaseConfComponent interface
public:
    /**
     * @brief type return type.
     * @return QString("Label")
     */
    QString type() override { return QString("Label"); }
    /**
     * @brief getName return standard component name.
     * @return QString("DefaultLabel")
     */
    QString getName() override { return QString("DefaultLabel"); }
    /**
     * @brief hasValue Check if component has a value.
     * @return false.
     */
    bool hasValue() override { return false; }
};

/**
 * @brief The ConfDirBrowser class describes Dir browser component.
 */
class ConfDirBrowser : public BaseConfComponent
{
    Q_OBJECT
public:
    /**
     * @brief ConfDirBrowser Constructor for Dir browser.
     * @param parent parent object.
     * @param name Name of the component.
     * @param placeholderText Placeholder text.
     * @param value Default value.
     * @param buttonText Caption of the button.
     * @param parameterName Parameter name.
     */
    explicit ConfDirBrowser(QWidget *parent = 0, const QString &name = STRS::defaultName, const QString &placeholderText = QString("Default text"),
                            const QVariant &value = QVariant::fromValue("Default path"),
                            const QString &buttonText = QString("Browse"), const QString &parameterName = QString("Param"));
    ~ConfDirBrowser();

private slots:
    /**
     * @brief pushButtonClicked Event when browse button clicked.
     */
    void pushButtonClicked();
    /**
     * @brief lineEditTextChanged Event when LineEdit text changed in the component.
     * @param text Current text.
     */
    void lineEditTextChanged(QString text);

private:
    /**
     * @brief lineEdit Pointer to QLineEdit.
     */
    QLineEdit* lineEdit;
    /**
     * @brief pushButton Pointer to QPushButton.
     */
    QPushButton* pushButton;

    // BaseConfComponent interface
public:
    /**
     * @brief type Gets type of component.
     * @return QString("DirBrowser")
     */
    QString type() override { return QString("DirBrowser"); }
    /**
     * @brief getName Getter for name of component.
     * @return name.
     */
    QString getName() override;
    /**
     * @brief hasValue Check if component has value.
     * @return true.
     */
    bool hasValue() override { return true; }
};

/**
 * @brief The ConfUiManager class describes UI manager form.
 */
class ConfUiManager : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief ConfUiManager Constructor for ui manager.
     * @param parent Specifies parent widget.
     */
    explicit ConfUiManager(QWidget* parent = nullptr);
    ~ConfUiManager();

public slots:
    /**
     * @brief constructUi Method that constructs ui from QSetting object
     * @param iniConf Settings object
     */
    void constructUi(QSettings **iniConf);
    /**
     * @brief addComponent Adds component on main form.
     * @param name Component name.
     * @param parameters Parameters of component.
     */
    void addComponent(QString &name, const QMap<QString, QVariant> &parameters);
    /**
     * @brief deleteComponent Delete component by it's name.
     * @param name Component's name.
     */
    void deleteComponent(const QString &name);
    /**
     * @brief saveParameters Save back parameters to settings file.
     */
    void saveParameters();
    /**
     * @brief getLayout Getter for layout->
     * @return Layout of all components.
     */
    QLayout *getLayout();
private:
    /**
     * @brief uiComponents Stores all components in vector.
     */
    QVector<BaseConfComponent *> uiComponents;

public:

    /**
     * @brief layout Pointer to QLaypout for components.
     */
    QVBoxLayout* layout;
    /**
     * @brief getUiComponents Getter for components.
     * @return Vector that contains all added components.
     */
    const QVector<BaseConfComponent *>* getUiComponents() const { return &this->uiComponents; }

private:
    /**
     * @brief iniConf Pointer to setting.
     */
    QSettings **iniConf;
};

#endif // CONFIGURABLEUI_H
