#include "startuplistevents.h"
#include "ui_startuplistevents.h"
#include <QMessageBox>
/*!
 * \brief Typ numerujący nagłówki kolumn
 */
enum header_names_startup {ROW_DATE,GODZ,DESC,STAN};

startupListEvents::startupListEvents(QString session_username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::startupListEvents)
{
    ui->setupUi(this);
    setWindowTitle("Nadchodzące wydarzenia"); //!< Ustawianie nazwy okna
    ui->tableWidget->setColumnCount(4); //!< Ustawianie ilości kolumn
    ui->tableWidget->setRowCount(0); //!< Ustawienie początkowej ilości wierszy
    ui->tableWidget->verticalHeader()->setVisible(false); //!< Usuwanie nagłówka pionowego
    QStringList headers; //!< Przechowuje nagłówki
    headers<<"Data"<<"Godzina"<<"Opis zadania"<<"Stan"; //!< Wczytanie nazw nagłówków.
    ui->tableWidget->setHorizontalHeaderLabels(headers); //!< Wczytanie listy z nagłówkami kolumn tabeli
    ui->tableWidget->setColumnWidth(GODZ, 60); //!<Rozmiar kolumny godziny
    ui->tableWidget->setColumnWidth(DESC,300); //!<Rozmiar kolumny na opis wydarzenia
    ui->tableWidget->setColumnWidth(ROW_DATE, 200); //!< Rozmiar kolumny daty
    ui->tableWidget->setColumnWidth(STAN, 110);
    readFile(archive); //!< Odczyt wydarzeń z pliku
    session_data=session_username; //!< Ustalenie nazwy bieżącego użytkownika
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
    archive.setFileName("archive.txt"); //!<Ustawienie nazwy pliku
    if(archive.exists() == false)
    {
        return 1;
    }
    else{
        if(archive.open(QIODevice::Text | QIODevice::ReadOnly)==false) //!< Otwieranie pliku i sprawdzenie statusu powodzenia operacji
        {
            QMessageBox error; //!<Generowanie komunikatu o błędzie w przypadku niepowodzenia
            error.setText("Fatal error!");
            error.setInformativeText("Niepowodzenie w otwarciu pliku do odczytu.");
            QApplication::exit(-1); //!< Awaryjne zamknięcie aplikacji
        }
        else
        {
            event_log_struct buffer; //!< Zmienna tymczasowa na przechowywanie wpisów
            QDataStream plik(&archive);
            while(!plik.atEnd()) //!< Odczytywanie kolejnych danych z pliku
            {
                plik>>buffer;
                storage.push_back(buffer);

            }
            qDebug()<<"[startuplistevents] Wpisano wydarzenia do okna powitalnego"<<endl;
            archive.close(); //!< Zamknięcie pliku
            return 1;
        }
    }
    return 0;
}
void startupListEvents::fetchTable(const QDate &date)
{
    int rowsCount=0; //!<Początkowa ilość wierszy
    for(int i=0; i<storage.size(); i++)
    {
        qDebug()<<"[startuplistevents] fetchTable: do porównania: "<<endl;
        qDebug()<<"-> storage[i].date = "<<storage[i].date.toString()<<" z "<< date.toString()<<endl;
        qDebug()<<"-> storage[i].time = "<<storage[i].time.toString()<<" z "<<QTime::currentTime()<<endl;
        qDebug()<<"-> Data o dzień późniejsza: "<<storage[i].date.addDays(1)<<" i "<<QDate::currentDate().addDays(1)<<endl;
        /*!
         * Wczytywanie wydarzeń na bieżący dzień
         *
         */
        if((storage[i].date == date) && (storage[i].time >= QTime::currentTime()) && (storage[i].username==session_data))
        {
            ui->tableWidget->setRowCount(++rowsCount);
            qDebug()<<"[startuplistevents] Wartość rowsCount"<<rowsCount<<endl;
            ui->tableWidget->setItem(rowsCount-1,ROW_DATE,new QTableWidgetItem(storage[i].date.toString("dddd, d MMMM yyyy")));
            ui->tableWidget->setItem(rowsCount-1,GODZ,new QTableWidgetItem(storage[i].time.toString()));
            ui->tableWidget->setItem(rowsCount-1,DESC,new QTableWidgetItem(storage[i].description));
            ui->tableWidget->setItem(rowsCount-1,STAN,new QTableWidgetItem(storage[i].stan));

        }
    }
    //! Wczytywanie wydarzeń na dzień późniejszy
    for(int i=0; i<storage.size(); i++)
    {
        if(storage[i].date==date.addDays(1))
        {
            ui->tableWidget->setRowCount(++rowsCount);
            ui->tableWidget->setItem(rowsCount-1,ROW_DATE,new QTableWidgetItem(storage[i].date.toString("dddd, d MMMM yyyy")));
            ui->tableWidget->setItem(rowsCount-1,GODZ,new QTableWidgetItem(storage[i].time.toString()));
            ui->tableWidget->setItem(rowsCount-1,DESC,new QTableWidgetItem(storage[i].description));
            ui->tableWidget->setItem(rowsCount-1,STAN,new QTableWidgetItem(storage[i].stan));
        }
    }
    if(rowsCount == 0) ui->tableWidget->setRowCount(0);
}
