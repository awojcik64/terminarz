#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <edit_event.h>

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
    ui->tableWidget->setColumnWidth(GODZ, 60);
    ui->tableWidget->setColumnWidth(DESC,450);
    ui->tableWidget->setColumnWidth(STATE, 110);
    ui->timeEdit->setTime(QTime::currentTime());
    readFile(archive);
    logowanie.exec();
    sortBy=TIME;
    ascending=true;
    if(storage.size()>0)
        updateTable(ui->calendarWidget->selectedDate());
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    startupListEvents logon_message(logowanie.session_data);
    logon_message.exec();
    statusBar()->showMessage("Dodatkowe opcje dotyczące istniejących wydarzeń dostępne są pod prawym przyciskiem myszy.");

    QActionGroup* sortGroup = new QActionGroup(this);
    sortGroup->addAction(ui->actionWed_ug_godziny);
    sortGroup->addAction(ui->actionWed_ug_opisu);
    sortGroup->addAction(ui->actionWed_ug_statusu);
    sortGroup->setExclusive(true);
    ui->actionWed_ug_godziny->setChecked(true);
    QActionGroup* sortOrderGroup = new QActionGroup(this);
    sortOrderGroup->addAction(ui->actionRosn_co);
    sortOrderGroup->addAction(ui->actionMalej_co);
    sortOrderGroup->setExclusive(true);
    ui->actionRosn_co->setChecked(true);

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
bool MainWindow::sortComparator(const event_log &arg1,const  event_log &arg2)
{
    switch(sortBy)
    {
    case TIME:
    {
        if(ascending)
        {
            return (arg1.time<arg2.time);
        }else return (arg1.time>arg2.time);
    }break;
    case DESCRIPTION:
    {
        if(ascending)
        {
            return (arg1.description<arg2.description);
        }else return (arg1.description>arg2.description);
    }break;
    case STATUS:
    {
        if(ascending)
        {
            return (arg1.stan<arg2.stan);
        }else return (arg1.stan>arg2.stan);
    }break;
    default:
    {
        QMessageBox critical_error;
        critical_error.setIcon(QMessageBox::Critical);
        critical_error.setText("Błąd sortowania");
        critical_error.setInformativeText("Sortowanie jest uszkodzone.");
        critical_error.exec();
        exit(-1);
    }
    }
}
bool MainWindow::sort()
{
    int i=0,j=0,k=0;
    for(i=0; i<storage.size(); i++)
    {
        k=i;
        for(j=i+1; j<storage.size(); j++)
            //if(storage[j].time<storage[k].time)
            if(sortComparator(storage[j], storage[k]))
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
void testowy()
{
    QMessageBox test;
    test.setText("REEEE");
    test.exec();
}
void MainWindow::deleteEvent()
{
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

void MainWindow::setStatus()
{
    QMessageBox::StandardButton result =QMessageBox::Yes; //QMessageBox::question(this, "Zmiana statusu!","Na pewno zmianić status wpisu?", QMessageBox::Yes | QMessageBox::No);
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

                if(storage[iter].date==ui->calendarWidget->selectedDate() && storage[iter].username == logowanie.session_data)
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

void MainWindow::editEvent()
{
    QList<QTableWidgetItem *> to_change = ui->tableWidget->selectedItems();
    QList<QTableWidgetItem *>::iterator i;
    for(i = to_change.begin(); i!= to_change.end(); i++)
    {
        qDebug()<<(*i)->row()<<endl;
        int target_row=(*i)->row();
        int counter=0;
        int iter;
        for(iter=0; iter<storage.size(); iter++)
        {

            if(storage[iter].date==ui->calendarWidget->selectedDate() && storage[iter].username == logowanie.session_data)
            {
                if(counter == target_row)
                {
                    if(storage[iter].date==ui->calendarWidget->selectedDate())
                    {
                        break;
                    }
                }
                counter++;

            }

        }
        edit_event editor;
        editor.setOriginalDate(storage[iter].date);
        editor.setOriginalTime(storage[iter].time);
        editor.setOriginalDescription(storage[iter].description);
        editor.exec();
        if(editor.good()==true) {
            storage[iter].date=editor.getDate();
            storage[iter].time=editor.getTime();
            storage[iter].description=editor.getDescription();
            updateTable(ui->calendarWidget->selectedDate());
            writeFile(archive);
        }
    }
}

void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu *table_menu = new QMenu(this);
    QAction* set_status = new QAction("Zmień status", this);
    set_status->setStatusTip("Zmień status wydarzenia.");
    connect(set_status, SIGNAL(triggered()), this, SLOT(setStatus()));
    table_menu->addAction(set_status);
    table_menu->addSeparator();
    QAction* action_edit = new QAction("Edytuj", this);
    action_edit->setStatusTip("Edytuj datę, godzinę, opis wydarzenia.");
    connect(action_edit, SIGNAL(triggered()), this, SLOT(editEvent()));
    table_menu->addAction(action_edit);
    QAction* delete_event = new QAction("Usuń", this);
    delete_event->setStatusTip("Usuń wpis. Tej akcji nie można cofnąć.");
    connect(delete_event, SIGNAL(triggered()), this, SLOT(deleteEvent()));
    table_menu->addAction(delete_event);
    table_menu->exec(QCursor::pos());
    qDebug()<<"Pressed at: "<<pos.x()<<", "<<pos.y()<<endl;
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
        QFile file2("Users.txt");
        file2.remove();
        file2.open(QIODevice::Text | QIODevice::WriteOnly);
        file.close();
        QMessageBox info;
        info.setIcon(QMessageBox::Warning);
        info.setText("Zamknięcie programu");
        info.setInformativeText("W celu zatwierdzenia zmian, program zostanie zamknięty...");
        info.exec();
        exit(0);
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

void MainWindow::on_actionWed_ug_statusu_triggered(bool checked)
{
    sortBy=STATUS;
    updateTable(ui->calendarWidget->selectedDate());
}

void MainWindow::on_actionWed_ug_opisu_triggered(bool checked)
{
    sortBy=DESCRIPTION;
    updateTable(ui->calendarWidget->selectedDate());
}


void MainWindow::on_actionWed_ug_godziny_triggered(bool checked)
{
    sortBy=TIME;
    updateTable(ui->calendarWidget->selectedDate());
}

void MainWindow::on_actionRosn_co_triggered(bool checked)
{
    ascending=true;
    updateTable(ui->calendarWidget->selectedDate());
}

void MainWindow::on_actionMalej_co_triggered(bool checked)
{
    ascending=false;
    updateTable(ui->calendarWidget->selectedDate());
}
