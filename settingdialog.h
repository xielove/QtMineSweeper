#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include "appresource.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();
    void showEvent(QShowEvent *) override;
    bool Success() const;
    FieldArgs getArgs();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SettingDialog *ui;
    bool m_success = false;
};

#endif // SETTINGDIALOG_H
