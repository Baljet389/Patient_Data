#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <fstream>
#include <vector>

// Includes für Funktoinen zum Einlesen und Ausgeben der Daten:
#include "io_data.h"
#include "database.h"
#include <string>
#include <sstream>
#include <QString>
#include <regex> // Musterabgleich zum CSV einlesen Daten verifizieren
#include <QRegularExpression> // Q Musterabgleich

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

        QFile datei(pfad); // QFile verwenden für UTF-8
        if (!datei.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw runtime_error("Datei konnte nicht geöffnet werden");
        }
        qDebug() << "Datei geöffnet:" << pfad;

        QTextStream in(&datei); // Kein Setzen des Codecs nötig, UTF-8 ist der Standard
        //in.setCodec("UTF-8"); // Entfernen, da es nicht mehr nötig ist

        QString zeile;
        bool skipUeberschrift = true; // Überschrift beim Einlesen überspringen

        // QRegularExpression für Validierungen
        QRegularExpression zahlenRegex("^[0-9]+$");                                // Nur Zahlen
        QRegularExpression nameRegex("^[a-zA-ZäöüÄÖÜß]+(-[a-zA-ZäöüÄÖÜß]+)*$");    // Nur Buchstaben und Trennstrich
        QRegularExpression datumRegex("^\\d{2}\\.\\d{2}\\.\\d{4}$");               // Format DD.MM.YYYY
        QRegularExpression geschlechtRegex("^[mwMWdD]$");                          // Einzelbuchstabe (z. B. m/w/d)

        Database database; // Instanz der Datenbankklasse

        while (!in.atEnd()) {
            zeile = in.readLine(); // Zeile einlesen
            if (skipUeberschrift) {
                qDebug() << "Überspringe Überschrift:" << zeile;
                skipUeberschrift = false;
                continue; // Header-Zeile überspringen
            }

            QStringList werte = zeile.split(',');

            // Vollständigkeitscheck
            if (werte.size() >= 11) {
                try {
                    // Validierung der einzelnen Felder
                    if (!zahlenRegex.match(werte[0]).hasMatch()) {
                        throw invalid_argument("Ungültige ID: nur Zahlen erlaubt");
                    }
                    if (!nameRegex.match(werte[1]).hasMatch()) {
                        throw invalid_argument("Ungültiger Vorname: nur Buchstaben und Trennstrich erlaubt");
                    }
                    if (!nameRegex.match(werte[2]).hasMatch()) {
                        throw invalid_argument("Ungültiger Nachname: nur Buchstaben und Trennstrich erlaubt");
                    }
                    if (!datumRegex.match(werte[3]).hasMatch()) {
                        throw invalid_argument("Ungültiges Geburtsdatum: Format DD.MM.YYYY erwartet");
                    }
                    if (!geschlechtRegex.match(werte[4]).hasMatch()) {
                        throw invalid_argument("Ungültiges Geschlecht: Ein Buchstabe m/w/d erwartet");
                    }
                    if (!datumRegex.match(werte[8]).hasMatch()) {
                        throw invalid_argument("Ungültiges Eintrittsdatum: Format DD.MM.YYYY erwartet");
                    }

                    // Wenn alle Tests bestanden sind, Patient erstellen
                    io_data patient(
                        werte[0].toInt(),  // ID
                        werte[1],          // Vorname
                        werte[2],          // Nachname
                        werte[3],          // Geburtsdatum
                        werte[4],          // Geschlecht
                        werte[5],          // Adresse
                        werte[6],          // Telefonnummer
                        werte[7],          // E-Mail
                        werte[8],          // Eintrittsdatum
                        werte[9],          // Diagnose
                        werte[10]          // Behandlung
                        );

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
