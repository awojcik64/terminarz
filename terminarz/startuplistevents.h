#ifndef STARTUPLISTEVENTS_H
#define STARTUPLISTEVENTS_H

#include <QDialog>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QString>
namespace Ui {
class startupListEvents;
}

struct event_log_struct
{
    QDate date;
    QTime time;
    QString description;
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
