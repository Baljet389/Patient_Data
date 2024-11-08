#include "mainwindow.h"

#include <QApplication>
#include <fstream>
#include <vector>

// Includes für Funktoinen zum Einlesen und Ausgeben der Daten:
#include "io_data.h"
#include "database.h"
#include <string>
#include <sstream>
#include <QString>
#include <regex> // Musterabgleich zum CSV einlesen Daten verifizieren

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



void io_data::CSVeinlesen(QString pfad) {
    try {
        qDebug() << "io_data::CSVeinlesen()";

        ifstream datei(pfad.toStdString()); // Open file
        if (!datei.is_open()) {
            throw runtime_error("Could not open file");
        }
        qDebug() << "File opened:" << pfad;

        string zeile;
        bool ersteZeile = true; // Flag, um die erste Zeile zu überspringen

        // Regex für Validierungen
        regex zahlenRegex("^[0-9]+$");                          // Nur Zahlen
        regex nameRegex("^[a-zA-ZäöüÄÖÜß\\-]+$");               // Nur Buchstaben
        regex datumRegex("^\\d{2}\\.\\d{2}\\.\\d{4}$");         // Format DD.MM.YYYY
        regex geschlechtRegex("^[mwMWdD]$");                    // Einzelbuchstabe (z. B. m/w/d)

        while (getline(datei, zeile)) { // Read each line
            if (ersteZeile) {
                qDebug() << "Skipping header:" << QString::fromStdString(zeile);
                ersteZeile = false;
                continue; // Überspringe die Header-Zeile
            }

            qDebug() << "Read line:" << QString::fromStdString(zeile);

            stringstream gelesene_zeile(zeile);
            string wert;
            vector<QString> werte;

            // Parse the line into individual values separated by commas
            while (getline(gelesene_zeile, wert, ',')) {
                werte.push_back(QString::fromStdString(wert));
                qDebug() << "Parsed value:" << QString::fromStdString(wert);
            }

            // Überprüfe, ob es mindestens 11 Werte gibt
            if (werte.size() >= 11) {
                try {
                    // Validierung der einzelnen Felder
                    if (!regex_match(werte[0].toStdString(), zahlenRegex)) {
                        throw invalid_argument("Invalid ID: must contain only numbers");
                    }
                    if (!regex_match(werte[1].toStdString(), nameRegex)) {
                        throw invalid_argument("Invalid first name: must contain only letters");
                    }
                    if (!regex_match(werte[2].toStdString(), nameRegex)) {
                        throw invalid_argument("Invalid last name: must contain only letters");
                    }
                    if (!regex_match(werte[3].toStdString(), datumRegex)) {
                        throw invalid_argument("Invalid birthdate: must follow DD.MM.YYYY format");
                    }
                    if (!regex_match(werte[4].toStdString(), geschlechtRegex)) {
                        throw invalid_argument("Invalid gender: must be a single letter (m/w/d)");
                    }
                    if (!regex_match(werte[8].toStdString(), datumRegex)) {
                        throw invalid_argument("Invalid admission date: must follow DD.MM.YYYY format");
                    }

                    // Wenn alle Tests bestanden sind, Patient erstellen
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

                    qDebug() << "Patient valid:" << QString::number(patient.ID) << patient.vorname << patient.nachname;

                    // Save the patient data to the database
                    // Database.addPatient(patient); // Assuming Database has an addPatient function
                } catch (const invalid_argument &e) {
                    qDebug() << "Invalid data in line, skipping. Error:" << e.what();
                }
            } else {
                qDebug() << "Incomplete line, expected 11 values but got:" << werte.size();
            }
        }

    } catch (const exception &e) {
        qDebug() << "An error occurred:" << e.what();
    }
}
