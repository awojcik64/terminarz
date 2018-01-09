#include "mainwindow.h"
#include <QApplication>
/*!
 * \brief Funkcja main
 *
 * Rdzeń całego programu, jednakże cała jej rola sprowadza się do utworzenia okna i doprowadzenia do jego działania.
 *
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
