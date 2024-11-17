#include "mainwindow.h"
#include <QMainWindow>
#include <QApplication>
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    LoginDialog l;
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
