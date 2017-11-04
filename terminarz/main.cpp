#include "mainwindow.h"
#include <QApplication>

class eventmgr
{
protected:
    QVector<event_log> storage;
public:
    event_log buffer;
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
