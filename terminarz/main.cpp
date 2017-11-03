#include "mainwindow.h"
#include <QApplication>
struct event_log
{
    QDate date;
    QTime time;
    std::string description;
};

class eventmgr
{
public:
    eventmgr()
    {
        QDialog startup_error;
        startup_error.show();
        sleep(5);
        exit(-1);
    }
    eventmgr(MainWindow &w)
    {
        w.show();
    }

};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    eventmgr manager(w);
    return a.exec();
}
