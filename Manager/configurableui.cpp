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
    qDebug() << value.toString();
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
    layout1.addWidget(checkBox);
    layout1.addSpacerItem(spacer);
    setLayout(&layout1);
}

void ConfCheckBox::checkBoxTriggered(bool value)
{
    setValue(QVariant(value));
}

ConfCheckBox::~ConfCheckBox()
{
    delete checkBox;
    delete spacer;
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
    layout1.addWidget(lineEdit);
    setLayout(&layout1);
}

void ConfLineEdit::lineEditTextChanged(QString text)
{
    setValue(QVariant(text));
}

ConfLineEdit::~ConfLineEdit()
{
    delete lineEdit;
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

    layout1.addWidget(lineEdit);
    layout1.addWidget(pushButton);
    setLayout(&layout1);
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

ConfDirBrowser::~ConfDirBrowser()
{
    delete lineEdit;
    delete pushButton;
}

ConfLabel::ConfLabel(QWidget *parent, const QString &name, const QVariant &text, const QString &componentName)
    : BaseConfComponent(componentName, parent)
{
    qDebug() << text.toString();
    label = new QLabel(text.toString());
    if (name != "")
    {
        label->setObjectName(name);
    } else {
        QMessageBox::warning(parent, "Incorrect name", "Specify appropriate name for Label component, please.");
    }
    spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);

    layout1.addWidget(label);
    layout1.addSpacerItem(spacer);
    setLayout(&layout1);
}

ConfLabel::~ConfLabel()
{
    delete label;
    delete spacer;
}

QMap<QString, int> uiConsts = { {"label", 0}, {"lineedit", 1}, {"dirbrowser", 2}, {"checkbox", 3} };

ConfUiManager::ConfUiManager(QWidget *parent)
    : QWidget(parent)
{

}

ConfUiManager::~ConfUiManager()
{

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
                qDebug() << "Must be " << parameters["Text"].toString();
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

void ConfUiManager::deleteUi()
{

}

QLayout *ConfUiManager::getLayout()
{
//    if (layout)
//    {
////        deleteUi();
//        delete layout;
//    }

    layout = new QVBoxLayout();

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
