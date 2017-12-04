/***********************************************************
 * Projekt prowadzony w ramach zajęć projektowych          *
 * Programowanie w Języku C(++), Politechnika Świętokrzyska*
 * Autorzy: Aleksander Wojcik i Wiktor Wojcik, grupa 2ID15A*
 ***********************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <startuplistevents.h>
#include <login_form.h>

namespace Ui {
class MainWindow;
}
struct event_log
{
    QString username;
    QDate date;
    QTime time;
    QString description;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    QVector<event_log> storage;
    QFile archive;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString lineEdit_text();
    QTime timeEdit_time();
    QDate calendar_date();
    login_form logowanie;

private slots:
    void on_pushButton_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_lineEdit_returnPressed();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_actionWyczy_dane_triggered();

    void on_actionWyjd_triggered();

    void on_actionO_programie_triggered();

private:
    Ui::MainWindow *ui;
    void updateTable(const QDate &date);
    bool sort();
    void swap(event_log &entity1, event_log &entity2);
    bool writeFile(QFile &archive);
    bool readFile(QFile &archive);
    void addEvent();
    void deleteEvent(int row);
};


#endif // MAINWINDOW_H
