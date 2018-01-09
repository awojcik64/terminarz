#include "calculateweekday.h"
#include "ui_calculateweekday.h"

calculateWeekday::calculateWeekday(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calculateWeekday)
{
    ui->setupUi(this);
    ui->wybor_daty->setDate(QDate::currentDate());
    ui->dzien_tygodnia->setText(QDate::currentDate().toString("dddd"));
}

calculateWeekday::~calculateWeekday()
{
    delete ui;
}

void calculateWeekday::on_wybor_daty_dateChanged(const QDate &date)
{
    ui->dzien_tygodnia->setText(date.toString("dddd"));
}
