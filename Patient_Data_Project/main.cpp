#include "mainwindow.h"
#include <QMainWindow>
#include <QApplication>
#include "logindialog.h"
#include "database.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database database;
    while (true) {
        LoginDialog l;
        if (l.exec() == QDialog::Accepted) {
            MainWindow w(nullptr, &database);
            w.showMaximized();


            // Ereignisschleife für MainWindow
            if (a.exec() != 0) {
                // Bei Beenden des Mainwindows auf Abmeldung prüfen
                continue; // Zurück zur Login-Schleife
            }
        } else {
            qDebug() << "Login abgebrochen. Anwendung beendet.";
            break;
        }
    }
    return 0;
}



