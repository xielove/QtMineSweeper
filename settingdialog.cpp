#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "QDebug"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::showEvent(QShowEvent *)
{
    this->m_success = false;
}

void SettingDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    auto ok = (QAbstractButton*)ui->buttonBox->button(QDialogButtonBox::Ok);
//    auto cancel = (QAbstractButton*)ui->buttonBox->button(QDialogButtonBox::Cancel);
    qDebug() << m_success;
    if(button == ok){
        this->m_success = true;
    }
    this->close();
}

bool SettingDialog::Success() const
{
    return m_success;
}

FieldArgs SettingDialog::getArgs()
{
    return FieldArgs{ui->rowSpanBox->value(), ui->colSpanBox->value(), ui->mineSpanBox->value()};
}
