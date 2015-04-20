#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

namespace Ui {
class HelpDialog;
}
/**
 * @brief The HelpDialog class describes ui class dialog for help purposes.
 */
class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief HelpDialog Constructor
     * @param parent Parent object.
     */
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

private:
    /**
     * @brief ui Ui components.
     */
    Ui::HelpDialog *ui;
};

#endif // HELPDIALOG_H
