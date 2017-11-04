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
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString lineEdit_text();
    QTime timeEdit_time();
    QDate calendar_date();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

struct event_log
{
    QDate date;
    QTime time;
    QString description;
};


#endif // MAINWINDOW_H
/***********************************************************
 * Projekt prowadzony w ramach zajęć projektowych          *
 * Programowanie w Języku C(++), Politechnika Świętokrzyska*
 * Autorzy: Aleksander Wojcik i Wiktor Wojcik, grupa 2ID15A*
 ***********************************************************/
