#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>

enum header_names {GODZ,DESC,STATE};
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->verticalHeader()->setVisible(false);
    QStringList headers;
    headers<<"Godzina"<<"Opis zadania"<<"Stan";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setColumnWidth(DESC,450);
    readFile(archive);
    logowanie.exec();
    if(storage.size()>0)
        updateTable(ui->calendarWidget->selectedDate());
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    startupListEvents logon_message;
    logon_message.exec();


    /*QMessageBox warn;
    warn.setText("Aplikacja Work in Progress; zglaszanie bledow mile widziane.");
    warn.exec();*/
}
QDataStream &operator>>(QDataStream &in, event_log &buffer)
{
    in>>buffer.username>>buffer.date>>buffer.time>>buffer.description>>buffer.stan;
    return in;
}
QDataStream &operator<<(QDataStream &in, event_log &buffer)
{
    in<<buffer.username<<buffer.date<<buffer.time<<buffer.description<<buffer.stan;
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

    sort();
    qDebug()<<"[mainwindow] Do porownania: "<<logowanie.session_data<<endl;
    for(int i=0; i<storage.size(); i++)
    {
        if((storage[i].date == date) && (storage[i].username==logowanie.session_data))
        {
            ui->tableWidget->setRowCount(++rowsCount);
            ui->tableWidget->setItem(rowsCount-1,GODZ,new QTableWidgetItem(storage[i].time.toString()));
            ui->tableWidget->setItem(rowsCount-1,DESC,new QTableWidgetItem(storage[i].description));
            ui->tableWidget->setItem(rowsCount-1,STATE, new QTableWidgetItem(storage[i].stan));

        }
    }
    if(rowsCount == 0) ui->tableWidget->setRowCount(0);
}
void MainWindow::on_pushButton_clicked()
{
    addEvent();
}
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    updateTable(date);
}


void MainWindow::swap(event_log &entity1, event_log &entity2)
{
    event_log tmp;
    tmp=entity1;
    entity1=entity2;
    entity2=tmp;
}
bool MainWindow::sort()
{
    int i=0,j=0,k=0;
    for(i=0; i<storage.size(); i++)
    {
        k=i;
        for(j=i+1; j<storage.size(); j++)
            if(storage[j].time<storage[k].time)
            {
                k=j;
            }
        swap(storage[k], storage[i]);
    }
    return 1;
}

void MainWindow::addEvent()
{
    event_log buffer;
    buffer.username=logowanie.session_data;
    qDebug()<<"[mainwindow] Wpisano do buffera: "<<buffer.username<<endl;
    buffer.date=ui->calendarWidget->selectedDate();
    buffer.time=ui->timeEdit->time();
    buffer.description=ui->lineEdit->text();
    buffer.stan="Nie wykonano!";
    storage.push_back(buffer);
    updateTable(ui->calendarWidget->selectedDate());
    writeFile(archive);
}
void MainWindow::on_lineEdit_returnPressed()
{
    addEvent();
}
void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    qDebug()<<"Pressed at: "<<pos.x()<<", "<<pos.y()<<endl;
    QMessageBox::StandardButton result = QMessageBox::question(this, "Usuwanie wpisu","Na pewno usunąć wybrany wpis?", QMessageBox::Yes | QMessageBox::No);
    if(result==QMessageBox::Yes)
    {
        QList<QTableWidgetItem *> to_remove = ui->tableWidget->selectedItems();
        QList<QTableWidgetItem *>::iterator i;
        for(i = to_remove.begin(); i!= to_remove.end(); i++)
        {
            qDebug()<<(*i)->row()<<endl;
            //storage.erase(storage.begin()+(*i)->row());
            int target_row=(*i)->row();
            int counter=0;
            for(int iter=0; iter<storage.size(); iter++)
            {
                qDebug()<<"[mainwindow] Do porownania: "<<logowanie.session_data<<endl;
                if((storage[iter].date==ui->calendarWidget->selectedDate()) && storage[iter].username == logowanie.session_data)
                {

                    if(counter == target_row)
                    {
                        storage.erase(storage.begin()+iter);
                        break;
                    }
                    counter++;
                }

            }
            updateTable(ui->calendarWidget->selectedDate());
            writeFile(archive);
        }
    }

}
void MainWindow::on_actionWyczy_dane_triggered()
{
    QMessageBox::StandardButton result = QMessageBox::question(this,"Usuwanie danych","Usunąć wszystkie dane? Tej operacji nie można cofnąć!", QMessageBox::Yes | QMessageBox::No);
    if(result==QMessageBox::Yes) {
        QFile file("archive.txt");
        file.remove();
        file.open(QIODevice::Text | QIODevice::WriteOnly);
        file.close();
        storage.clear();
        updateTable(QDate::currentDate());
    }
}
void MainWindow::on_actionWyjd_triggered()
{
    exit(0);
}

void MainWindow::on_actionO_programie_triggered()
{
    QMessageBox info;
    info.setText("O programie");
    info.setInformativeText("Niniejszy program powstał na potrzeby\nprojektu na zaliczenie w Politechnice Świętokrzyskiej\nAutorzy:\nAleksander Wójcik\nWiktor Wójcik");
    info.setIcon(QMessageBox::Information);
    info.exec();
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    QMessageBox::StandardButton result = QMessageBox::question(this, "Zmiana statusu!","Na pewno zmianić status wpisu?", QMessageBox::Yes | QMessageBox::No);
    if(result==QMessageBox::Yes)
    {
        QList<QTableWidgetItem *> to_change = ui->tableWidget->selectedItems();
        QList<QTableWidgetItem *>::iterator i;
        for(i = to_change.begin(); i!= to_change.end(); i++)
        {
            qDebug()<<(*i)->row()<<endl;
            int target_row=(*i)->row();
            int counter=0;
            for(int iter=0; iter<storage.size(); iter++)
            {

                if(storage[iter].date==ui->calendarWidget->selectedDate())
                {
                    if(counter == target_row)
                    {
                        if(storage[iter].date==ui->calendarWidget->selectedDate())
                                {
                                    if(storage[iter].stan == "Wykonano!")
                                        storage[iter].stan = "Nie wykonano!";
                                    else
                                        if(storage[iter].stan == "Nie wykonano!")
                                            storage[iter].stan = "Wykonano!";
                                break;

                                }
                    }
                    counter++;

                }

            }
            updateTable(ui->calendarWidget->selectedDate());
            writeFile(archive);
        }
    }
}
