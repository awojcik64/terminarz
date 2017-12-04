#include "startuplistevents.h"
#include "ui_startuplistevents.h"
#include <QMessageBox>

enum header_names_startup {DATE,GODZ,DESC};

startupListEvents::startupListEvents(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startupListEvents)
{
    ui->setupUi(this);
    setWindowTitle("Nadchodzące wydarzenia");
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->verticalHeader()->setVisible(false);
    QStringList headers;
    headers<<"Data"<<"Godzina"<<"Opis zadania";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setColumnWidth(DESC,300);
    readFile(archive);
}

startupListEvents::~startupListEvents()
{
    delete ui;
}
QDataStream &operator>>(QDataStream &in, event_log_struct &buffer)
{
    in>>buffer.date>>buffer.time>>buffer.description;
    return in;
}
bool startupListEvents::readFile(QFile &archive)
{
    archive.setFileName("archive.txt");
    if(archive.exists() == false)
    {
        /*QMessageBox warn;
        warn.setText("Odnotowano pierwsze uruchomienie.");
        warn.setInformativeText("Zostanie utworzony plik do przechowania danych w chwili dodania pierwszego wpisu.");
        warn.exec();*/
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
            event_log_struct buffer;
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
void startupListEvents::fetchTable(const QDate &date)
{
    int rowsCount=0;
    for(int i=0; i<storage.size(); i++)
    {
        if(storage[i].date == date && storage[i].time >= QTime::currentTime())
        {
            ui->tableWidget->setRowCount(++rowsCount);
            ui->tableWidget->setItem(rowsCount-1,DATE,new QTableWidgetItem(storage[i].date.toString()));
            ui->tableWidget->setItem(rowsCount-1,GODZ,new QTableWidgetItem(storage[i].time.toString()));
            ui->tableWidget->setItem(rowsCount-1,DESC,new QTableWidgetItem(storage[i].description));

        }
    }

    for(int i=0; i<storage.size(); i++)
    {
        if(storage[i].date==date.addDays(1))
        {
            ui->tableWidget->setRowCount(++rowsCount);
            ui->tableWidget->setItem(rowsCount-1,DATE,new QTableWidgetItem(storage[i].date.toString()));
            ui->tableWidget->setItem(rowsCount-1,GODZ,new QTableWidgetItem(storage[i].time.toString()));
            ui->tableWidget->setItem(rowsCount-1,DESC,new QTableWidgetItem(storage[i].description));
        }
    }
    if(rowsCount == 0) ui->tableWidget->setRowCount(0);
}
