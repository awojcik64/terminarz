#ifndef CALCULATEWEEKDAY_H
#define CALCULATEWEEKDAY_H

#include <QDialog>
/*! \brief Aplikacja Weekday - obliczanie dni tygodnia
*
*   Okno w formie dialogowej umożliwiające obliczenie dnia tygodnia.
*/

namespace Ui {
class calculateWeekday;
}
/*! \brief Klasa calculateWeekday
*
*   Klasa, na podstawie której tworzone jest okno dialogowe
*   aplikacji Weekday.
*
*/
class calculateWeekday : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor klasy calculateWeekday
     *
     * Ustawia obecną datę w polu do jej edycji, natomiast obok wstawia obliczony dzień tygodnia.
     */
    explicit calculateWeekday(QWidget *parent = 0);
    ~calculateWeekday();

private slots:
    /*!
     * \brief Funkcja on_wybor_daty_dateChanged
     *
     * Funkcja wywoływana przy każdej zmianie daty w polu do jej edycji.
     */
    void on_wybor_daty_dateChanged(const QDate &date);

private:
    Ui::calculateWeekday *ui; //!Wskaźnik na okno dialogowe.
};

#endif // CALCULATEWEEKDAY_H
