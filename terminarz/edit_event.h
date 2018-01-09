#ifndef EDIT_EVENT_H
#define EDIT_EVENT_H

#include <iostream>
#include <QDialog>
#include <unistd.h>
#include <fstream>
#include <QtGui>
#include <QtWidgets>
#include <QFile>
namespace Ui {
class edit_event;
}

/*!
 * \brief Klasa edit_event.
 *
 * Klasa, na podstawie której tworzone jest okno edycji wydarzenia.
 */

class edit_event : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor edit_event
     *
     * Ustawia obecną datę i godzinę
     * która jest potem nadpisywana przez wpis do edycji.
     * Ustawia również zmienną edit_success warunkująca, czy edycja powiodła się.
     * Domyślna jej wartosć to: false.
     */
    explicit edit_event(QWidget *parent = 0);
    ~edit_event();
    void setOriginalDate(QDate arg_date); /*!< Ustawianie daty wydarzenia do edycji */
    void setOriginalTime(QTime arg_time); /*!< Ustawianie godziny wydarzenia do edycji */
    void setOriginalDescription(QString arg_description); /*!< Ustawianie opisu wydarzenia do edycji*/
    QDate getDate(); /*!< Odczyt daty po edycji*/
    QTime getTime();/*!< Odczyt godziny po edycji*/
    QString getDescription(); /*!< Odczyt opisu po edycji*/
    bool good(); /*!< Sprawdzenie powodzenia edycji*/

private slots:
    void on_buttonBox_accepted(); /*!< Zatwierdzanie zmian */

private:
    Ui::edit_event *ui;

    bool edit_success; /*!< Zmienna określająca powodzenie wprowadzonych zmian */
    QDate date_new; /*!< Przechowuje datę po edycji */
    QTime time_new; /*!< Przechowuje godzinę po edycji */
    QString description_new; /*!< Przechowuje opis po edycji*/
};

#endif // EDIT_EVENT_H
