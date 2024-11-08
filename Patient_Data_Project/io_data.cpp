#include "mainwindow.h"

#include <QApplication>
#include <fstream>
#include <vector>

// Includes für Funktoinen zum Einlesen und Ausgeben der Daten:
#include "io_data.h"
#include <string>
#include <sstream>
#include <QString>
using namespace std;

io_data::io_data(int ID,QString vorname,QString nachname,QString geburt,QString geschlecht,QString adresse,QString tel_nummer,QString mail,QString datum,QString diagnose,QString behandlung){
    this->ID=ID;
    this->vorname=vorname;
    this->nachname=nachname;
    this->geburt=geburt;
    this->geschlecht=geschlecht;
    this->adresse=adresse;
    this->tel_nummer=tel_nummer;
    this->mail=mail;
    this->datum=datum;
    this->diagnose=diagnose;
    this->behandlung=behandlung;
}

int io_data::returnAge(){
    // Zerlege String aus TT.MM.JJJJ in Tag, Monat, Jahr

    char trennzeichen = '.';
    string teilstr;
    istringstream stream(geburt.toStdString());

    // Tag bestimmen
        getline(stream, teilstr, trennzeichen);
        int tag = stoi(teilstr);

        // Monat bestimmen
        getline(stream, teilstr, trennzeichen);
        int monat = stoi(teilstr);

        // Jahr bestimmen
        getline(stream, teilstr, trennzeichen);
        int jahr = stoi(teilstr);

    // Berechne Alter

        // Aktuelles Datum erhalten
        time_t jetzt = time(0);
        tm* ltm = localtime(&jetzt);

        int aktuellesJahr = 1900 + ltm->tm_year;
        int aktuellerMonat = 1 + ltm->tm_mon;
        int aktuellerTag = ltm->tm_mday;

        // Alter berechnen
        int alter = aktuellesJahr - jahr;

        // Berücksichtigen, ob Geburtstag in diesem Jahr schon war
        if (monat > aktuellerMonat || (monat == aktuellerMonat && tag > aktuellerTag)) {
            alter--;  // Eins abziehen, wenn Geburtstag noch nicht war
            }
        alterFinal=alter;
        return alter;

}
    // while(getline(datei,zeile))
    // {
    //     stringstream zeilenpuffer(zeile);
    //     zeilenpuffer >> nummer >> vorname >> nachname >> geburt >> geschlecht >> adresse >> telnummer >> mail >> datum >> diagnose >> behandlung;
    //     cout << nummer << "\n" << vorname << "\n" << nachname << "\n" << geburt << "\n" << geschlecht << "\n" << adresse << "\n" << telnummer << "\n" << mail << "\n" << datum << "\n" << diagnose << "\n" << behandlung << "\n";
    // }


#include <QDebug>
#include <fstream>
#include <sstream>
#include <vector>
#include <QString>
#include "io_data.h"
#include "database.h"

using namespace std;

void io_data::CSVeinlesen(QString pfad) {
    try {
        qDebug() << "io_data::CSVeinlesen()";

        ifstream datei(pfad.toStdString()); // Open file
        if (!datei.is_open()) {
            throw runtime_error("Could not open file");
        }
        qDebug() << "File opened:" << pfad;

        string zeile;
        while (getline(datei, zeile)) { // Read each line
            qDebug() << "Read line:" << QString::fromStdString(zeile);

            stringstream gelesene_zeile(zeile);
            string wert;
            vector<QString> werte;

            // Parse the line into individual values separated by commas
            while (getline(gelesene_zeile, wert, ',')) {
                werte.push_back(QString::fromStdString(wert));
                qDebug() << "Parsed value:" << QString::fromStdString(wert);
            }

            // Check if there are enough values before creating a `patient` object
            if (werte.size() >= 11) {
                io_data patient(
                    stoi(werte[0].toStdString()), // ID
                    werte[1],                     // First name
                    werte[2],                     // Last name
                    werte[3],                     // Date of birth
                    werte[4],                     // Gender
                    werte[5],                     // Address
                    werte[6],                     // Phone number
                    werte[7],                     // Email
                    werte[8],                     // Date
                    werte[9],                     // Diagnosis
                    werte[10]                     // Treatment
                    );

                qDebug() << "Patient:" << patient.vorname << patient.nachname;

                // Save the patient data to the database
//                Database.addPatient(patient); // Assuming Database has an addPatient function
            } else {
                qDebug() << "Incomplete line, expected 11 values but got:" << werte.size();
            }
        }

    } catch (const exception &e) {
        qDebug() << "An error occurred:" << e.what();
    }
}
