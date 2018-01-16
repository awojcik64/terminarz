#ifndef STARTUPLISTEVENTS_H
#define STARTUPLISTEVENTS_H

#include <QMainWindow>
#include <cstring>
#include <QtCore>
#include <iostream>
#include <QDialog>
#include <fstream>
#include <QtGui>
#include <QtWidgets>
#include <QFile>
namespace Ui {
class startupListEvents;
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
struct event_log_struct
{
    QString username;
    QDate date;
    QTime time;
    QString description;
    QString stan;
};
/*!
 * \brief Klasa startupListEvents
 *
 * Klasa, na podstawie której tworzone jest okno wyświetlane po zalogowaniu.
 * Zawiera ona informacje o wydarzeniach nadchodzących na dany dzień i następny.
 */
class startupListEvents : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor klasy startupListEvents
     *
     * Przygotowuje strukturę tabeli, w której wyświetli nadchodzące wydarzenia
     */
    explicit startupListEvents(QString session_username, QWidget *parent = 0);
    ~startupListEvents();
    bool readFile(QFile &archive); //!< Odczytuje wydarzenia z pliku
    QString session_data;

private:
    Ui::startupListEvents *ui;
    QVector<event_log_struct> storage; //!< Pojemnik na odczytane z pliku wydarzenia.
    QFile archive; //!< Przechowuje metadane pliku
    void fetchTable(const QDate &date); //!< Uzupełnia tabelę wydarzeniami.
};

#endif // STARTUPLISTEVENTS_H
