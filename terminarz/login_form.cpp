#include "login_form.h"
#include "ui_login_form.h"

login_form::login_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_form)
{
    ui->setupUi(this);
    setWindowTitle("Logowanie do terminarza");
    odczyt_uzytkownikow(user_file);
    qDebug()<<"[login_form] Wczytano uzytkownikow"<<endl;
    ui->EditPassword->setEchoMode(QLineEdit::Password); //!< Ustawianie pola tak, by zakrywało hasło.
}

login_form::~login_form()
{
    delete ui;
}
/*!
 * \brief operator >>
 *
 * Przeładowany operator ułatwiający operację na plikach.
 * Wpisuje dane z buffera i je zwraca, jako wynik.
 */
QDataStream &operator>>(QDataStream &in, UserData &buffer)
{
    in>>buffer.login>>buffer.password;
    return in;
}


QString login_form::zaloguj()
{
    QString login,haslo;
    login = ui->EditLogin->text();
    haslo = ui->EditPassword->text();
    bool czy_istnieje_user=false;
    for(int i=0; i<users.size(); i++)
    {
        if(login==users[i].login) //!< Sprawdzenie, czy użytkownik istnieje
        {
            czy_istnieje_user=true;
            if(haslo == users[i].password) //!< Sprawdzanie poprawności hasła
            {
                session_data=login;
                qDebug()<<"[login_form] Wpisano: "<<login<<" - wartosc session_data = "<<session_data<<endl;
                return login;
            }
            else
            {
                /*!
                 * \brief Komunikat o niewłaściwym haśle.
                 */
                QMessageBox wrongpass;
                wrongpass.setText("Błąd logowania");
                wrongpass.setInformativeText("Nieprawidłowe hasło.");
                wrongpass.setIcon(QMessageBox::Critical);
                wrongpass.exec();
                break;
            }
        }
    }
    if(czy_istnieje_user == false) {
        /*!< Komunikat informujący o braku istnieniu danego użytkownika */
        QMessageBox bad_user;
        bad_user.setText("Błąd logowania");
        bad_user.setInformativeText("Użytkownik o takiej nazwie nie istnieje!");
        bad_user.setIcon(QMessageBox::Critical);
        bad_user.exec();
    }
    exit(0);
}


bool login_form::odczyt_uzytkownikow(QFile &user_file)
{
    user_file.setFileName("Users.txt"); //!< Ustawia nazwę pliku
    if(user_file.exists()==false) //!< Sprawdza istnienie pliku
    {
        user_file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append);
        user_file.close();
        qDebug()<<"Pierwsze uruchomienie - stworzono archiwum uzytkownikow"<<endl;
    }


    if(user_file.open(QIODevice::Text | QIODevice::ReadOnly)==false)
    {
        QMessageBox error; //!< Informacja o niepowodzeniu w otwarciu pliku
        error.setText("Fatal error!");
        error.setInformativeText("Niepowodzenie w otwarciu pliku do odczytu.");
        QApplication::exit(-1);
    }
    else
    {
        /*!
         * \brief Pobieranie danych do pliku
         */
        UserData buffer;
        QDataStream plik(&user_file);
        while(!plik.atEnd())
        {
            plik>>buffer;
            users.push_back(buffer);

        }
        user_file.close();
        return 1;
    }

    return 0;
}
/*!
 * \brief Operator definiujący przypisywanie struktury.
 *
 */
QDataStream &operator<<(QDataStream &in, UserData &buffer)
{
    in<<buffer.login<<buffer.password;
    return in;
}
bool login_form::writeFile(QFile &user_file)
{
    user_file.setFileName("Users.txt");
    if(user_file.open(QIODevice::Text | QIODevice::WriteOnly)==false) //!< Sprawdzenie, czy plik udało się otworzyć
    {
        QMessageBox error;
        error.setText("Fatal error!");
        error.setInformativeText("Niepowodzenie w otwarciu pliku do zapisu.");
        QApplication::exit(-1);
    }
    else
    {
        //! Zapis danych do pliku
        UserData buffer;
        QDataStream plik(&user_file);
        for(int i=0; i<users.size(); i++)
        {
            buffer=users[i];
            plik<<buffer;
        }
    }
    user_file.close();
    return 1;
}
void login_form::on_buttonBox_accepted()
{
    zaloguj();
}

void login_form::on_addUser_clicked()
{
    UserData buffer;
    buffer.login=ui->EditLogin->text();
    buffer.password=ui->EditPassword->text();

    bool user_exists=false;
    for(int i=0; i<users.size(); i++)
    {
        if(users[i].login==buffer.login)
        {
            user_exists=true;
            QMessageBox add_error;
            add_error.setText("Błąd rejestracji");
            add_error.setInformativeText("Użytkownik o takiej nazwie już istnieje!");
            add_error.setIcon(QMessageBox::Critical);
            add_error.exec();
            exit(0);
        }
    }
    if(user_exists==false)
    {
        users.push_back(buffer);
        writeFile(user_file);
        QMessageBox info_success;
        info_success.setText("Rejestracja");
        info_success.setInformativeText("Zarejestrowano poprawnie.");
        info_success.setIcon(QMessageBox::Information);
        info_success.exec();
    }
}

void login_form::on_buttonBox_rejected()
{
    exit(0);
}
