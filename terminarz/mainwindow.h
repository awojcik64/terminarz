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
#include <fstream>
#include <QtGui>
#include <QtWidgets>
#include <QFile>
#include <startuplistevents.h>
#include <login_form.h>

namespace Ui {
class MainWindow;
}
/*!
 * \brief Struktura wpisu terminarza
 *
 * Struktura, na podstawie której tworzone są zmienne przechowujące dane wydarzenia.
 * Przechowuje:
 * -Nazwę użytkownika, do którego ono należy.
 * -Przypisaną datę
 * -Przypisaną godzinę
 * -Opis wydarzenia
 * -Stan jego wykonania
 */
struct event_log
{
    QString username;
    QDate date;
    QTime time;
    QString description;
    QString stan;
};

enum header_names {GODZ,DESC,STATE};/*!< Typ numerujący kolumny */
enum sort_rules {TIME, DESCRIPTION, STATUS};/*!< Typ numerujący warunki sortowania*/
enum sort_order {ASCENDING, DESCENDING}; /*!< Typ numerujący sortowanie rosnące/malejące */


/*!
 * \brief Klasa MainWindow
 *
 * Rdzeń całego programu. Na jej podstawie tworzone jest okno główne zawierające:
 * -pasek menu
 * -kalendarz
 * -wydzieloną część na dodawanie wydarzenia
 * -Tabelę, w której wyświetlane są wydarzenia
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    QVector<event_log> storage; //!< Pojemnik na dane z wydarzeniami */
    QFile archive; //!< Przechowuje metadane pliku z bazą wydarzeń
    /*!
     * \brief Konstruktor klasy MainWindow
     *
     * Przygotowuje okno główne programu do pracy.
     * Wywołuje przede wszystkim formularz logowania, listę z nadchodzącymi wydarzeniami itp.
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString lineEdit_text(); //!< Nieużywane: odczyt danych z pola na opis wydarzenia
    QTime timeEdit_time(); //!< Nieużywane: odczyt danych z pola na godzinę wydarzenia
    QDate calendar_date(); //!< Nieużywane: odczyt obecnego dnia z kalendarza
    login_form logowanie; //!< Zmienna do tworzenia formularza logowania

private slots:
    void on_pushButton_clicked(); //!< Wywołuje funkcję addEvent()

    void on_calendarWidget_clicked(const QDate &date); //!< Wywołuje funkcję updateTable() i aktualizuje informacje w pasku poniżej kalendarza.

    void on_lineEdit_returnPressed(); //!< Wywołuje funkcję addEvent()

    void on_tableWidget_customContextMenuRequested(const QPoint &pos); //!< Otwiera kontekstowe menu z możliwością podjęcia decyzji wobec wybranego wpisu

    void on_actionWyczy_dane_triggered(); //!< Usuwa dane z plików, po czym zamyka program.

    void on_actionWyjd_triggered(); //!< Zamyka program

    void on_actionO_programie_triggered(); //!<Wyświetla informacje o programie w oknie dialogowym

    void updateTable(const QDate &date); //!< Aktualizuje informacje w tabeli na podstawie wybranej daty

    void deleteEvent(); //!< Usuwa wybrane wydarzenie

    void setStatus(); //!< Zmienia status wybranego wydarzenia

    void editEvent(); //!< Otwiera okno dialogowe do edycji wydarzenia

    void on_actionWed_ug_statusu_triggered(bool checked); //!< Zmienia regułę sortowania na: według statusu

    void on_actionWed_ug_opisu_triggered(bool checked); //!< Zmienia regułę sortowania na: według opisu

    void on_actionWed_ug_godziny_triggered(bool checked); //!< Zmienia regułę sortowania na: według godziny

    void on_actionRosn_co_triggered(bool checked); //!< Przestawia kolejność sortowania na: rosnąco

    void on_actionMalej_co_triggered(bool checked); //!< Przestawia kolejność sortowania na: malejąco

    void on_actionAplikacja_Weekday_triggered(); //!< Otwiera aplikację Weekday

private:
    Ui::MainWindow *ui;
    bool sort(); //!< Sortuje wydarzenia
    void swap(event_log &entity1, event_log &entity2);//!< Uzupełnienie sortowania: zamienia wydarzenia ze sobą.
    bool writeFile(QFile &archive); //!< Zapisuje wydarzenia do pliku
    bool readFile(QFile &archive); //!< Odczytuje wydarzenia z pliku
    void addEvent(); //!< Dodaje wydarzenia
    unsigned short int sortBy; //!< Numer reguły sortowania
    bool ascending; //!* Determinuje kolejność sortowania
    bool sortComparator(const event_log &arg1, const event_log &arg2); //!* Funkcja porównująca dla sortowania według ustawień reguły i kolejności
};


#endif // MAINWINDOW_H
