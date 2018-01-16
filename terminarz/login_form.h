#ifndef LOGIN_FORM_H
#define LOGIN_FORM_H
#include <iostream>
#include <QDialog>
#include <fstream>
#include <QtGui>
#include <QtWidgets>
#include <QFile>
/*!
 * \brief Struktura UserData
 *
 * Na jej podstawie tworzony jest obiekt przchowujący login i hasło użytkownika.
 */
struct UserData
{
    QString login; /*!< Nazwa użytkownika*/
    QString password; /*!< Hasło użytkownika*/
};

namespace Ui {
class login_form;
}
/*!
 * \brief Klasa login_form
 *
 * Klasa, na podstawie której tworzony jest formularz logowania.
 */
class login_form : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor login_form
     *
     * Odczytuje użytkowników, ustawia pole tekstowe
     * odpowiadające za hasło na zakryte.
     */
    explicit login_form(QWidget *parent = 0);
    ~login_form();
    QFile user_file; /*!< Przechowuje metadane pliku */
    QString zaloguj(); /*!< Funkcja sprawdzająca poprawność logowania i logująca użytkownika*/
    QString session_data;
private slots:
    void on_buttonBox_accepted(); /*!< Wywołuje funkcję zaloguj() */

    void on_addUser_clicked(); /*!< Rejestruje użytkownika, sprawdzając poprawność logowania */

    void on_buttonBox_rejected(); /*!< Anuluje jakiekolwiek czynności i zamyka program*/

private:
    Ui::login_form *ui;
    QVector<UserData> users; /*!< Przechowuje wszystkich użytkowników*/
    bool writeFile(QFile &user_file);/*!< Zapisuje nowo zarejestrowanych użytkowników do pliku. */
    bool odczyt_uzytkownikow(QFile &user_file); /*!< Wczytuje użytkowników z Users.txt do wektorowej tablicy users*/
};
#endif // LOGIN_FORM_H
