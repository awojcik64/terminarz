#include "mainwindow.h"
#include <QApplication>

struct event_log
{
    QDate term_data;
    QTime term_time;
    std::string description;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
