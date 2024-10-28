#include "mainwindow.h"
#include <QApplication>
#include "io_data.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    io_data Testobject(1, "Max", "Mustermann", "29.10.2005", "männlich", "Musterstraße 1", "0123456789", "max@example.com", "2023-10-28", "Diagnose A", "Behandlung A");
    int abc=Testobject.returnAge();
    int z;
    w.show();



    return a.exec();
}
