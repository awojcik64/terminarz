#ifndef LOGIN_FORM_H
#define LOGIN_FORM_H
#include <iostream>
#include <QDialog>
#include <unistd.h>
#include <fstream>
#include <QtGui>
#include <QtWidgets>
#include <QFile>

struct UserData
{
    QString login;
    QString password;
};

namespace Ui {
class login_form;
}

class login_form : public QDialog
{
    Q_OBJECT

public:
    explicit login_form(QWidget *parent = 0);
    ~login_form();
    QFile user_file;
    QString zaloguj();
    QString session_data;
private slots:
    void on_buttonBox_accepted();

    void on_addUser_clicked();

    void on_buttonBox_rejected();

private:
    Ui::login_form *ui;
    QVector<UserData> users;
    bool writeFile(QFile &user_file);
    bool odczyt_uzytkownikow(QFile &user_file);
};
#endif // LOGIN_FORM_H
