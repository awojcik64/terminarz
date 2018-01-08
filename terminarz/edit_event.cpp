#include "edit_event.h"
#include "ui_edit_event.h"

edit_event::edit_event(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_event)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->edit_event_time->setTime(QTime::currentTime());
    edit_success=false;
}
void edit_event::setOriginalDate(QDate arg_date)
{
    ui->dateEdit->setDate(arg_date);
}
void edit_event::setOriginalTime(QTime arg_time)
{
    ui->edit_event_time->setTime(arg_time);
}
void edit_event::setOriginalDescription(QString arg_description)
{
    ui->editEvent_description->setText(arg_description);
}

edit_event::~edit_event()
{
    delete ui;
}

void edit_event::on_buttonBox_accepted()
{
    description_new=ui->editEvent_description->text();
    date_new=ui->dateEdit->date();
    time_new=ui->edit_event_time->time();
    edit_success=true;
}
QDate edit_event::getDate()
{
    return date_new;
}
QTime edit_event::getTime()
{
    return time_new;
}
QString edit_event::getDescription()
{
    return description_new;
}
