#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
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

    readFile(archive);
    if(ui->tableWidget->colorCount()>0) updateTable(ui->calendarWidget->selectedDate());
    /*QMessageBox warn;
    warn.setText("Aplikacja Work in Progress; zglaszanie bledow mile widziane.");
    warn.exec();*/
}
QDataStream &operator>>(QDataStream &in, event_log &buffer)
{
    in>>buffer.date>>buffer.time>>buffer.description;
    return in;
}
QDataStream &operator<<(QDataStream &in, event_log &buffer)
{
    in<<buffer.date<<buffer.time<<buffer.description;
    return in;
}
bool MainWindow::writeFile(QFile &archive)
{
    archive.setFileName("archive.txt");
    if(archive.open(QIODevice::Text | QIODevice::WriteOnly)==false)
    {
        QMessageBox error;
        error.setText("Fatal error!");
        error.setInformativeText("Niepowodzenie w otwarciu pliku do zapisu.");
        QApplication::exit(-1);
    }
    else
    {
        event_log buffer;
        QDataStream plik(&archive);
        for(int i=0; i<storage.size(); i++)
        {
            buffer=storage[i];
            plik<<buffer;
        }
    }
    archive.close();
    return 1;
}

bool MainWindow::readFile(QFile &archive)
{
    archive.setFileName("archive.txt");
    if(archive.exists() == false)
    {
        QMessageBox warn;
        warn.setText("Odnotowano pierwsze uruchomienie.");
        warn.setInformativeText("Zostanie utworzony plik do przechowania danych.");
        warn.exec();
        return 1;
    }
    else{
        if(archive.open(QIODevice::Text | QIODevice::ReadOnly)==false)
        {
            QMessageBox error;
            error.setText("Fatal error!");
            error.setInformativeText("Niepowodzenie w otwarciu pliku do odczytu.");
            QApplication::exit(-1);
        }
        else
        {
            event_log buffer;
            QDataStream plik(&archive);
            while(!plik.atEnd())
            {
                plik>>buffer;
                storage.push_back(buffer);

            }
            archive.close();
            return 1;
        }
    }
    return 0;
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

    for(int i=0; i<storage.size(); i++)
    {
        if(storage[i].date == date)
        {
            ui->tableWidget->setRowCount(++rowsCount);
            ui->tableWidget->setItem(rowsCount-1,GODZ,new QTableWidgetItem(storage[i].time.toString()));
            ui->tableWidget->setItem(rowsCount-1,DESC,new QTableWidgetItem(storage[i].description));

        }
    }
    if(rowsCount == 0) ui->tableWidget->setRowCount(0);
}
void MainWindow::on_pushButton_clicked()
{
    event_log buffer;
    buffer.date=ui->calendarWidget->selectedDate();
    buffer.time=ui->timeEdit->time();
    buffer.description=ui->lineEdit->text();
    storage.push_back(buffer);
    updateTable(ui->calendarWidget->selectedDate());
    writeFile(archive);
}
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    updateTable(date);
}
