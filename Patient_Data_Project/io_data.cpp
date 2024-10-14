#include "mainwindow.h"

#include <QApplication>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Einlesen
{
private:
    int ID;
    string vorname;
    string nachname;
    float geburt;
    char geschlecht;
    string adresse;
    float tel_nummer;
    string mail;
    float datum;
    string diagnose;
    string behandlung;
    string zeile;

public:

    // Einlesen();
    // ifstream datei("C:/workspace/Studium/3. Semester/Informatik/patientendaten50000.csv");



    // while(getline(datei,zeile))
    // {
    //     stringstream zeilenpuffer(zeile);
    //     zeilenpuffer >> nummer >> vorname >> nachname >> geburt >> geschlecht >> adresse >> telnummer >> mail >> datum >> diagnose >> behandlung;
    //     cout << nummer << "\n" << vorname << "\n" << nachname << "\n" << geburt << "\n" << geschlecht << "\n" << adresse << "\n" << telnummer << "\n" << mail << "\n" << datum << "\n" << diagnose << "\n" << behandlung << "\n";
    // }
};
