#include "startuplistevents.h"
#include "ui_startuplistevents.h"
#include <QMessageBox>

enum header_names_startup {DATE,GODZ,DESC,STAN};

startupListEvents::startupListEvents(QString session_username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startupListEvents)
{
    ui->setupUi(this);
    setWindowTitle("Nadchodzące wydarzenia");
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->verticalHeader()->setVisible(false);
    QStringList headers;
    headers<<"Data"<<"Godzina"<<"Opis zadania"<<"Stan";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setColumnWidth(DESC,300);
    readFile(archive);
    session_data=session_username;
    fetchTable(QDate::currentDate());
}

startupListEvents::~startupListEvents()
{
    delete ui;
}
QDataStream &operator>>(QDataStream &in, event_log_struct &buffer)
{
    in>>buffer.username>>buffer.date>>buffer.time>>buffer.description>>buffer.stan;
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
            qDebug()<<"[startuplistevents] Wpisano wydarzenia do okna powitalnego"<<endl;
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
        qDebug()<<"[startuplistevents] fetchTable: do porównania: "<<endl;
        qDebug()<<"-> storage[i].date = "<<storage[i].date.toString()<<" z "<< date.toString()<<endl;
        qDebug()<<"-> storage[i].time = "<<storage[i].time.toString()<<" z "<<QTime::currentTime()<<endl;
        qDebug()<<"-> Data o dzień późniejsza: "<<storage[i].date.addDays(1)<<" i "<<QDate::currentDate().addDays(1)<<endl;
        if((storage[i].date == date) && (storage[i].time >= QTime::currentTime()) && (storage[i].username==session_data))
        {
            ui->tableWidget->setRowCount(++rowsCount);
            qDebug()<<"[startuplistevents] Wartość rowsCount"<<rowsCount<<endl;
            ui->tableWidget->setItem(rowsCount-1,DATE,new QTableWidgetItem(storage[i].date.toString()));
            ui->tableWidget->setItem(rowsCount-1,GODZ,new QTableWidgetItem(storage[i].time.toString()));
            ui->tableWidget->setItem(rowsCount-1,DESC,new QTableWidgetItem(storage[i].description));
            ui->tableWidget->setItem(rowsCount-1,STAN,new QTableWidgetItem(storage[i].stan));

        }
    }

    /*for(int i=0; i<storage.size(); i++)
    {
        if(storage[i].date==date.addDays(1))
        {
            ui->tableWidget->setRowCount(++rowsCount);
            ui->tableWidget->setItem(rowsCount-1,DATE,new QTableWidgetItem(storage[i].date.toString()));
            ui->tableWidget->setItem(rowsCount-1,GODZ,new QTableWidgetItem(storage[i].time.toString()));
            ui->tableWidget->setItem(rowsCount-1,DESC,new QTableWidgetItem(storage[i].description));
        }
    }*/
    if(rowsCount == 0) ui->tableWidget->setRowCount(0);
}
