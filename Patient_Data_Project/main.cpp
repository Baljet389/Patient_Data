#include "mainwindow.h"
#include <QMainWindow>
#include <QApplication>
#include "logindialog.h"
#include "user.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginDialog l;
    user u(1, "DoctorHouse");
    if (l.exec() == QDialog::Accepted) {        
    MainWindow w;
    w.showMaximized();
    w.show();

    return a.exec();
    }
    else
    {
    return 0;
    }
}
