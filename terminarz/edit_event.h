#ifndef EDIT_EVENT_H
#define EDIT_EVENT_H

#include <iostream>
#include <QDialog>
#include <unistd.h>
#include <fstream>
#include <QtGui>
#include <QtWidgets>
#include <QFile>
namespace Ui {
class edit_event;
}
//I tak nie zdasz XD
class edit_event : public QDialog
{
    Q_OBJECT

public:
    explicit edit_event(QWidget *parent = 0);
    ~edit_event();
    void setOriginalDate(QDate arg_date);
    void setOriginalTime(QTime arg_time);
    void setOriginalDescription(QString arg_description);
    QDate getDate();
    QTime getTime();
    QString getDescription();
    bool good();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::edit_event *ui;

    bool edit_success;
    QDate date_new;
    QTime time_new;
    QString description_new;
};

#endif // EDIT_EVENT_H
