#include "configurableui.h"
#include <QMessageBox>
#include <QPushButton>
#include <QVariant>
#include <QDebug>
#include <QRegExpValidator>

#include "qtutils.h"

#ifdef STRS
#error "STRS already defined somewhere"
#endif

namespace STRS {

static const QString incorrectName = "Incorrect name";
static const QString specifyName = "Specify appropriate name for %s component, please.";
static const QString componentNotFound = "Component not found";
static const QString componentNameNotFound = "Component %s not found";
static const QString checkBox = "CheckBox";
static const QString placeholder = "PlaceholderText";
static const QString lineEdit = "LineEdit";
static const QString browserLineEdit = "ConfDirBrowser.LineEdit";
static const QString label = "Label";
static const QString type = "Type";
static const QString text = "Text";
static const QString mask = "Mask";
static const QString buttonText = "ButtonText";
static const QString value = "Value";
static const QString parameterName = "ParameterName";

static const QString noRealization = "No realization";
static const QString notNecessary = "Method is not necessary in v1.0.0 RC";

}

ConfCheckBox::ConfCheckBox(QWidget *parent, const QString &name, const QString &text,
                           const QVariant &value, const QString &componentName)
    : BaseConfComponent(componentName, parent)
{
    checkBox = nullptr;
    checkBox = new QCheckBox(text);

    if (checkBox)
    {
        setValue(value);
        connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxTriggered(bool)));

        if (name.length() != 0)
        {
            checkBox->setObjectName(name);
        } else {
            QMessageBox::warning(parent, STRS::incorrectName, QtUtils::stringFormat(STRS::specifyName, STRS::checkBox));
        }
        checkBox->setChecked(value.toBool());

        spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);
        getLayout()->addWidget(checkBox);
        getLayout()->addSpacerItem(spacer);
        setLayout(getLayout());
    } else
    {

    }
}

void ConfCheckBox::checkBoxTriggered(bool value)
{
    setValue(QVariant(value));
}

QString ConfCheckBox::getName()
{
    if (checkBox)
    {
        return checkBox->objectName();
    } else
    {
        return STRS::checkBox;
    }
}

const QVariant &ConfCheckBox::getValue()
{
    return value;
}

ConfCheckBox::~ConfCheckBox()
{
}

ConfLineEdit::ConfLineEdit(QWidget *parent, const QString &name, const QString &placeholderText,
                           const QVariant &value, QString &mask, const QString &parameterName)
    : BaseConfComponent(parameterName, parent)
{
    lineEdit = new QLineEdit();
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEditTextChanged(QString)));
    lineEdit->setText(value.toString());
    if (name.length() != 0)
    {
        lineEdit->setObjectName(name);
    } else {
        QMessageBox::warning(parent, STRS::incorrectName, QtUtils::stringFormat(STRS::specifyName, STRS::lineEdit));
    }

    lineEdit->setPlaceholderText(placeholderText);

    QRegExpValidator* validator = new QRegExpValidator(QRegExp(mask), this);
    lineEdit->setValidator(validator);

    getLayout()->addWidget(lineEdit);
    setLayout(getLayout());
}

void ConfLineEdit::lineEditTextChanged(QString text)
{
    setValue(QVariant(text));
}

QString ConfLineEdit::getName()
{
    if (lineEdit)
    {
        return lineEdit->objectName();
    } else
    {
        return STRS::lineEdit;
    }
}

ConfLineEdit::~ConfLineEdit()
{
}

ConfDirBrowser::ConfDirBrowser(QWidget *parent, const QString &name, const QString &placeholderText,
                               const QVariant &value, const QString &buttonText, const QString &parameterName)
    : BaseConfComponent(parameterName, parent)
{
    lineEdit = new QLineEdit();
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEditTextChanged(QString)));
    lineEdit->setText(value.toString());
    lineEdit->setPlaceholderText(placeholderText);
    if (name.length() != 0)
    {
        lineEdit->setObjectName(name);
    } else {
        QMessageBox::warning(parent, STRS::incorrectName, QtUtils::stringFormat(STRS::specifyName, STRS::browserLineEdit));
    }
    pushButton = new QPushButton(buttonText);
    connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));

    getLayout()->addWidget(lineEdit);
    getLayout()->addWidget(pushButton);
    setLayout(getLayout());
}

void ConfDirBrowser::pushButtonClicked()
{
    QString path;
    if (QtUtils::browseFolderTrigger(path))
    {
        lineEdit->setText(path);
        setValue(QVariant(path));
    }
}

void ConfDirBrowser::lineEditTextChanged(QString text)
{
    setValue(QVariant(text));
}

QString ConfDirBrowser::getName()
{
    if(lineEdit)
    {
        return lineEdit->objectName();
    } else
    {
        return STRS::browserLineEdit;
    }
}

ConfDirBrowser::~ConfDirBrowser()
{
}

ConfLabel::ConfLabel(QWidget *parent, const QString &name, const QVariant &text)
    : BaseConfComponent(name, parent)
{
    label = new QLabel(text.toString());
    if (name != "")
    {
        label->setObjectName(name);
    } else {
        QMessageBox::warning(parent, STRS::incorrectName, QtUtils::stringFormat(STRS::specifyName, STRS::label));
    }
    spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);

    getLayout()->addWidget(label);
    getLayout()->addSpacerItem(spacer);
    setLayout(getLayout());
}

ConfLabel::~ConfLabel()
{
}



ConfUiManager::ConfUiManager(QWidget *parent)
    : QWidget(parent), layout(nullptr)
{

}

ConfUiManager::~ConfUiManager()
{

}

QMap<QString, QStringList> uiParams;

void ConfUiManager::constructUi(QSettings **iniConf)
{
    this->iniConf = iniConf;

    // Delete last layout
    uiParams.clear();
    if (layout)
    {
        for (auto component : uiComponents)
        {
            delete component;
        }
        delete layout;
        layout = nullptr;
        uiComponents.clear();
    }

    QtUtils::splitQSettingsKeys((*iniConf)->allKeys(), uiParams);

    // Construct ui
    for (QString &key : uiParams.keys())
    {
        QMap<QString, QVariant> component;
        for (auto &valueKey : uiParams[key])
        {
            QString value = (*iniConf)->value(key + "/" + valueKey).toString();
            component[valueKey] = value;
        }
        addComponent(key, component);
    }
}

void ConfUiManager::addComponent(QString &name, const QMap<QString, QVariant> &parameters)
{
    QString typeName = parameters[STRS::type].toString().toLower();
    if (typeName == "")
    {
        return;
    }

    const QMap<QString, int> uiConsts = { {"label",       BaseConfComponent::BASE},
                                          {"lineedit",    BaseConfComponent::LINE_EDIT},
                                          {"dirbrowser",  BaseConfComponent::DIR_BROWSER},
                                          {"checkbox",    BaseConfComponent::CHECK_BOX}
                                        };

    switch (uiConsts[typeName])
    {
        case BaseConfComponent::BASE:
        {
            BaseConfComponent *label = new ConfLabel(parentWidget(), name, parameters[STRS::text]);
            uiComponents.push_back(label);
            break;
        }
        case BaseConfComponent::LINE_EDIT:
        {
            QString mask = parameters[STRS::mask].toString();
            BaseConfComponent *lineEdit = new ConfLineEdit(parentWidget(), name, parameters[STRS::placeholder].toString(),
                    parameters[STRS::value], (mask.length() == 0 ? ".+" : mask), parameters[STRS::parameterName].toString());
            uiComponents.push_back(lineEdit);
            break;
        }
        case BaseConfComponent::DIR_BROWSER:
        {
            BaseConfComponent *dirBrowser = new ConfDirBrowser(parentWidget(), name, parameters[STRS::placeholder].toString(),
                    parameters[STRS::value], parameters[STRS::buttonText].toString(), parameters[STRS::parameterName].toString());
            uiComponents.push_back(dirBrowser);
            break;
        }
        case BaseConfComponent::CHECK_BOX:
        {
            BaseConfComponent *checkBox = new ConfCheckBox(parentWidget(), name, parameters[STRS::text].toString(), QVariant(parameters[STRS::value].toBool()),
                    parameters[STRS::parameterName].toString());
            uiComponents.push_back(checkBox);
            break;
        }
        default:
        {
            QMessageBox::warning(this, STRS::componentNotFound, QtUtils::stringFormat(STRS::componentNameNotFound, typeName));
            break;
        }
    }
}

void ConfUiManager::deleteComponent(const QString &name)
{
    Q_UNUSED(name)
    QMessageBox::critical(this, STRS::noRealization, STRS::notNecessary);
}

void ConfUiManager::saveParameters()
{
    if (*iniConf == nullptr)
    {
        // TODO Error message
        return;
    }

    for (const auto component : uiComponents)
    {
        if (component->hasValue())
        {
            QString compName = component->getName();
            QString value = component->getValue().toString();
            {
                (*iniConf)->setValue(compName + "/" + STRS::value, value);
            }
        }
    }
}

QLayout *ConfUiManager::getLayout()
{
    layout = new QVBoxLayout();

    for (const auto component : uiComponents)
    {
        layout->addWidget(component);
    }

    return layout;
}

BaseConfComponent::BaseConfComponent(const QString &parameterName, QWidget *parent)
    : QWidget(parent)
{
    this->parameterName = parameterName;
}

