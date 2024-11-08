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

    QString dateipfad = "E:/DHBW/InformatikProgrammentwurf/CSV_Einlesen/CSVData/ShortPatientData.csv";

    io_data patient(50001, "Test", "Test", "", "", "", "", "", "", "", ""); // Leerer Patient
    patient.CSVeinlesen(dateipfad);

    return a.exec();
    }
    else
    {
    return 0;
    }
}
