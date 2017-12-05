#ifndef STARTUPLISTEVENTS_H
#define STARTUPLISTEVENTS_H

#include <QMainWindow>
#include <cstring>
#include <QtCore>
#include <iostream>
#include <QDialog>
#include <unistd.h>
#include <fstream>
#include <QtGui>
#include <QtWidgets>
#include <QFile>
namespace Ui {
class startupListEvents;
}

struct event_log_struct
{
    QDate date;
    QTime time;
    QString description;
    QString stan;
};

class startupListEvents : public QDialog
{
    Q_OBJECT

public:
    explicit startupListEvents(QWidget *parent = 0);
    ~startupListEvents();
    bool readFile(QFile &archive);

private:
    Ui::startupListEvents *ui;
    QVector<event_log_struct> storage;
    QFile archive;
    void fetchTable(const QDate &date);
};

#endif // STARTUPLISTEVENTS_H
