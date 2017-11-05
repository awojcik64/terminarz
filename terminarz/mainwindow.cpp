#include "mainwindow.h"
#include "ui_mainwindow.h"

enum header_names {GODZ,DESC};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->verticalHeader()->setVisible(false);
    QStringList headers;
    headers<<"Godzina"<<"Tekst";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setColumnWidth(DESC,600);

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


void MainWindow::updateTable(const QDate &date)
{
    int rowsCount=0;
    int counter=0;

    for(int i=0; i<storage.size(); i++)
    {
        if(storage[i].date == date)
        {
            ui->tableWidget->setRowCount(++rowsCount);
            ui->tableWidget->setItem(i,GODZ,new QTableWidgetItem(storage[i].time.toString()));
            ui->tableWidget->setItem(i,DESC,new QTableWidgetItem(storage[i].description));
            counter++;
        }
    }
    if(counter == 0) ui->tableWidget->setRowCount(0);
}
void MainWindow::on_pushButton_clicked()
{
    event_log buffer;
    buffer.date=ui->calendarWidget->selectedDate();
    buffer.time=ui->timeEdit->time();
    buffer.description=ui->lineEdit->text();
    storage.push_back(buffer);
    updateTable(ui->calendarWidget->selectedDate());

}
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    updateTable(date);
}
