#include "mainwindow.h"
#include <QMainWindow>
#include <QApplication>
#include "logindialog.h"
#include "database.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Database database;
    LoginDialog l;
    if (l.exec() == QDialog::Accepted) {
    MainWindow w(nullptr,&database);
    w.showMaximized();
    w.show();


    return a.exec();
    }
    else
    // {
    return 0;
    }

