#include "configurableui.h"
#include <QMessageBox>
#include <QPushButton>
#include <QVariant>
#include <QDebug>

ConfCheckBox::ConfCheckBox(QWidget *parent, const QString &name, const QString &text,
                           const QVariant &value, const QString &componentName)
    : BaseConfComponent(componentName, parent)
{
    checkBox = new QCheckBox(text);
    if (name != "")
    {
        checkBox->setObjectName(name);
    } else {
        QMessageBox::warning(parent, "Incorrect name", "Specify appropriate name for CheckBox component, please.");
    }

    // TODO check if bool
    //if (value.type() == QVariant::Bool)
    //{
        checkBox->setChecked(value.toBool());
    //} else {
    //    QMessageBox::warning(parent, QString("Incorrect value"), QString("Cannot convert value to bool. CheckBox ComponentName: " + name + "."));
    //}

    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxTriggered(bool)));

    spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);
    getLayout()->addWidget(checkBox);
    getLayout()->addSpacerItem(spacer);
    setLayout(getLayout());
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
        return QString("DefaultCheckBox");
    }
}

//QString ConfCheckBox::getName()
//{
//    if (checkBox)
//    {
//        return checkBox->objectName();
//    }
//}

ConfCheckBox::~ConfCheckBox()
{
}

ConfLineEdit::ConfLineEdit(QWidget *parent, const QString &name, const QString &placeholderText,
                           const QVariant &value, const QString &componentName)
    : BaseConfComponent(componentName, parent)
{
    lineEdit = new QLineEdit(value.toString());
    if (name != "")
    {
        lineEdit->setObjectName(name);
    } else {
        QMessageBox::warning(parent, "Incorrect name", "Specify appropriate name for LineEdit component, please.");
    }
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEditTextChanged(QString)));

    lineEdit->setPlaceholderText(placeholderText);
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
        return QString("DefaultLineEdit");
    }
}

ConfLineEdit::~ConfLineEdit()
{
}

ConfDirBrowser::ConfDirBrowser(QWidget *parent, const QString &name, const QString &placeholderText,
                               const QVariant &value, const QString &buttonText, const QString &componentName1)
    : BaseConfComponent(componentName1, parent)
{
    lineEdit = new QLineEdit(value.toString());
    lineEdit->setPlaceholderText(placeholderText);
    if (name != "")
    {
        lineEdit->setObjectName(name);
    } else {
        QMessageBox::warning(parent, "Incorrect name", "Specify appropriate name for ConfDirBrowser.LineEdit component, please.");
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
    if (browseFolderTrigger(path))
    {
        lineEdit->setText(path);
        setValue(QVariant(path));
    }
}

QString ConfDirBrowser::getName()
{
    if(lineEdit)
    {
        return lineEdit->objectName();
    } else
    {
        return QString("DefaultLineEdit");
    }
}

ConfDirBrowser::~ConfDirBrowser()
{
}

ConfLabel::ConfLabel(QWidget *parent, const QString &name, const QVariant &text, const QString &componentName)
    : BaseConfComponent(componentName, parent)
{
    label = new QLabel(text.toString());
    if (name != "")
    {
        label->setObjectName(name);
    } else {
        QMessageBox::warning(parent, "Incorrect name", "Specify appropriate name for Label component, please.");
    }
    spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);

    getLayout()->addWidget(label);
    getLayout()->addSpacerItem(spacer);
    setLayout(getLayout());
}

ConfLabel::~ConfLabel()
{
}

QString ConfLabel::getName()
{
    return QString("DefaultLabel");
}

QMap<QString, int> uiConsts = { {"label", 0}, {"lineedit", 1}, {"dirbrowser", 2}, {"checkbox", 3} };

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

    uiParams.clear();
    if (layout)
    {
        /*deleteUi();
            for (int i = 0; i < layout->count(); ++i)
            {
                qDebug() << i;
                delete layout->takeAt(i);
                //layout->removeItem();
            }
        delete layout;
        layout = nullptr;*/

        for (auto component : uiComponents)
        {
            delete component;
        }
        delete layout;
        layout = nullptr;
        uiComponents.clear();
    }



    for (const QString &key : (*iniConf)->allKeys())
    {
        QStringList splitValue = key.split("/");
        if (splitValue.size() <= 1)
        {
            continue;
        }
        QString name = splitValue.at(0);
        QString param = splitValue.at(1);
        if (uiParams.keys().indexOf(name) >= 0)
        {
            uiParams[name].push_back(param);
        } else {
            QStringList firstParam = { param };
            uiParams[name] = firstParam;
        }
    }

    // Construction ui
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
    QString typeName = parameters["Type"].toString().toLower();
    if (typeName != "")
    {
        switch (uiConsts[typeName])
        {
            case 0:
            {
                BaseConfComponent *label = new ConfLabel(parentWidget(), name, parameters["Text"], parameters["ParameterName"].toString());
                uiComponents.push_back(label);

                break;
            }
            case 1:
            {
                BaseConfComponent *lineEdit = new ConfLineEdit(parentWidget(), name, parameters["PlaceholderText"].toString(), parameters["Value"], parameters["ParameterName"].toString());
                uiComponents.push_back(lineEdit);

                break;
            }
            case 2:
            {
                BaseConfComponent *dirBrowser = new ConfDirBrowser(parentWidget(), name, parameters["PlaceholderText"].toString(), parameters["Value"], parameters["ButtonText"].toString(), parameters["ParameterName"].toString());
                uiComponents.push_back(dirBrowser);
                break;
            }
            case 3:
            {
                BaseConfComponent *checkBox = new ConfCheckBox(parentWidget(), name, parameters["Text"].toString(), parameters["Value"], parameters["ParameterName"].toString());
                uiComponents.push_back(checkBox);
                break;
            }
            default:
            {
                QMessageBox::warning(this, "Component not found", "Component " + typeName + " not found.");
                break;
            }
        }
    }

}
// TODO realize method
void ConfUiManager::deleteComponent(const QString &componentName)
{

}

void ConfUiManager::saveParameters()
{
    if (*iniConf != nullptr)
    {
        for (const auto component : uiComponents)
        {
            if (component->hasValue())
            {
                QString compName = component->getName();
                QString paramName = component->getParameterName();
                QString value = component->getValue().toString();
                qDebug() << "Name: " + compName;
                qDebug() << "Value: " + value;
                {
                    (*iniConf)->setValue(compName + "/" + "Value", value);
                }
            }
        }
    }
}

void ConfUiManager::deleteUi()
{

}

QLayout *ConfUiManager::getLayout()
{
    layout = new QVBoxLayout();
    //uiComponents

    for (const auto component : uiComponents)
    {
        layout->addWidget(component);
    }

    return layout;
}

BaseConfComponent::BaseConfComponent(const QString &componentName, QWidget *parent)
    : QWidget(parent)
{
    this->parameterName = componentName;
}
QHBoxLayout *BaseConfComponent::getLayout()
{
    return &layout1;
}

QString BaseConfComponent::getParameterName() const
{
    return parameterName;
}

void BaseConfComponent::setParameterName(const QString &value)
{
    parameterName = value;
}

