#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
QString MainWindow::lineEdit_text()
{
    return ui->lineEdit->text();
}
QTime MainWindow::timeEdit_time()
{
    return ui->timeEdit->time();
}
QDate MainWindow::calendar_date()
{
    return ui->calendarWidget->selectedDate();
}
void MainWindow::on_pushButton_clicked()
{
    event_log buffer;
    buffer.date=ui->calendarWidget->selectedDate();
    buffer.time=ui->timeEdit->time();
    buffer.description=ui->lineEdit->text();

}

