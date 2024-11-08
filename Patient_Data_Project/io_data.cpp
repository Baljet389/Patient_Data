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

        ifstream datei(pfad.toStdString()); // Datei öffnen
        if (!datei.is_open()) {
            throw runtime_error("Datei konnte nicht geöffnet werden");
        }
        qDebug() << "Datei geöffnet:" << pfad;

        string zeile;
        bool ersteZeile = true; // Flag, um die erste Zeile zu überspringen

        // Regex für Validierungen
        regex zahlenRegex("^[0-9]+$");                                  // Nur Zahlen
        regex nameRegex("^[a-zA-ZäöüÄÖÜß]+(-[a-zA-ZäöüÄÖÜß]+)*$");      // Nur Buchstaben und Trennstrich
        regex datumRegex("^\\d{2}\\.\\d{2}\\.\\d{4}$");                 // Format DD.MM.YYYY
        regex geschlechtRegex("^[mwMWdD]$");                            // Einzelbuchstabe (z. B. m/w/d)

        Database database; // Instanz der Datenbankklasse

        while (getline(datei, zeile)) { // Jede Zeile lesen
            if (ersteZeile) {
                qDebug() << "Überspringe Header:" << QString::fromStdString(zeile);
                ersteZeile = false;
                continue; // Header-Zeile überspringen
            }

            // qDebug() << "Lese Zeile:" << QString::fromStdString(zeile);

            stringstream gelesene_zeile(zeile);
            string wert;
            vector<QString> werte;

            // Zeile in einzelne Werte parsen, getrennt durch Kommas
            while (getline(gelesene_zeile, wert, ',')) {
                werte.push_back(QString::fromStdString(wert));
                // qDebug() << "Parsed value:" << QString::fromStdString(wert);
            }

            // Überprüfen, ob es mindestens 11 Werte gibt
            if (werte.size() >= 11) {
                try {
                    // Validierung der einzelnen Felder
                    if (!regex_match(werte[0].toStdString(), zahlenRegex)) {
                        throw invalid_argument("Ungültige ID: nur Zahlen erlaubt");
                    }
                    if (!regex_match(werte[1].toStdString(), nameRegex)) {
                        throw invalid_argument("Ungültiger Vorname: nur Buchstaben und Trennstrich erlaubt");
                    }
                    if (!regex_match(werte[2].toStdString(), nameRegex)) {
                        throw invalid_argument("Ungültiger Nachname: nur Buchstaben und Trennstrich erlaubt");
                    }
                    if (!regex_match(werte[3].toStdString(), datumRegex)) {
                        throw invalid_argument("Ungültiges Geburtsdatum: Format DD.MM.YYYY erwartet");
                    }
                    if (!regex_match(werte[4].toStdString(), geschlechtRegex)) {
                        throw invalid_argument("Ungültiges Geschlecht: Ein Buchstabe m/w/d erwartet");
                    }
                    if (!regex_match(werte[8].toStdString(), datumRegex)) {
                        throw invalid_argument("Ungültiges Eintrittsdatum: Format DD.MM.YYYY erwartet");
                    }

                    // Wenn alle Tests bestanden sind, Patient erstellen
                    io_data patient(
                        stoi(werte[0].toStdString()), // ID
                        werte[1],                     // Vorname
                        werte[2],                     // Nachname
                        werte[3],                     // Geburtsdatum
                        werte[4],                     // Geschlecht
                        werte[5],                     // Adresse
                        werte[6],                     // Telefonnummer
                        werte[7],                     // E-Mail
                        werte[8],                     // Eintrittsdatum
                        werte[9],                     // Diagnose
                        werte[10]                     // Behandlung
                        );

                    qDebug() << "Patient valide:" << QString::number(patient.ID) << patient.vorname << patient.nachname;

                    // Patient in Datenbank speichern
                    database.insertPatient(patient);

                } catch (const invalid_argument &e) {
                    qDebug() << "Ungültige Daten in Zeile, überspringe. Fehler:" << e.what();
                }
            } else {
                qDebug() << "Unvollständige Zeile, erwartet 11 Werte, erhalten:" << werte.size();
            }
        }

    } catch (const exception &e) {
        qDebug() << "Ein Fehler ist aufgetreten:" << e.what();
    }
}
