#include "mainwindow.h"

#include <QApplication>
#include <fstream>
#include <vector>

// Includes für Funktionen zum Einlesen und Ausgeben der Daten:
#include "io_data.h"
#include "database.h"
#include <string>
#include <sstream>
#include <QString>
#include <regex> // Musterabgleich zum CSV einlesen Daten verifizieren
#include <QDate>
#include <QDebug>

#include <fstream>
#include <vector>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError> // Für Fehlerbehandlung
#include <QMessageBox> // Für Benutzerwarnungen und Meldungen

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

QDate io_data::convertQStringToQDate(const QString datumString) {
    // Konvertiere den QString in ein QDate
    QDate datum = QDate::fromString(datumString, "dd.MM.yyyy");

    // Überprüfe, ob das Datum gültig ist
    if (!datum.isValid()) {
        qWarning() << "Ungültiges Datum im Format TT.MM.JJJJ erwartet:" << datumString;
        return QDate();  // Gibt ein ungültiges QDate zurück, falls das Format fehlerhaft ist
    }
    return datum;
}

QString io_data::convertQDateToQString(const QDate datum) {
    // Konvertiere das QDate in einen QString im Format "dd.MM.yyyy" und gib es zurück
    return datum.toString("dd.MM.yyyy");
}

void io_data::CSVeinlesen(QString pfad,Database &database) {
    try {
        qDebug() << "io_data::CSVeinlesen()";

        ifstream datei(pfad.toStdString()); // Datei öffnen
        if (!datei.is_open()) {
            throw runtime_error("Datei konnte nicht geöffnet werden");
        }
        qDebug() << "Datei geöffnet:" << pfad;

        string zeile;
        bool skipUeberschrift = true; // Überschrift beim Einlesen überspringen

        // Regex für Validierungen
        regex zahlenRegex("^[0-9]+$");                                  // Nur Zahlen
        regex nameRegex("^[^0-9]*$");                                   // Erlaubt alles außer Zahlen
        regex datumRegex("^\\d{2}\\.\\d{2}\\.\\d{4}$");                 // Format DD.MM.YYYY
        regex geschlechtRegex("^[mwMWdD]$");                            // Einzelbuchstabe (z. B. m/w/d)

        while (getline(datei, zeile)) { // Jede Zeile lesen
            if (skipUeberschrift) {
                qDebug() << "Überspringe Überschrift:" << QString::fromStdString(zeile);
                skipUeberschrift = false;
                continue; // Header-Zeile überspringen
            }

            // Konvertiere die Zeile von std::string zu QString unter Verwendung von UTF-8
            QString zeileQString = QString::fromUtf8(zeile.c_str());
            //qDebug() << "QString: " << zeileQString;

            // Parsen der gelesenen Zeile am Komma mit QString
            QStringList werteListe = zeileQString.split(',');

            // Vollständigkeitscheck
            if (werteListe.size() >= 11) {
                try {
                    // Validierung der einzelnen Felder
                    if (!regex_match(werteListe[0].toStdString(), zahlenRegex)) {
                        throw invalid_argument("Ungültige ID: nur Zahlen erlaubt");
                    }
                    if (!regex_match(werteListe[1].toStdString(), nameRegex)) {
                        throw invalid_argument("Ungültiger Vorname: nur Buchstaben und Trennstrich erlaubt");
                    }
                    if (!regex_match(werteListe[2].toStdString(), nameRegex)) {
                        throw invalid_argument("Ungültiger Nachname: nur Buchstaben und Trennstrich erlaubt");
                    }
                    if (!regex_match(werteListe[3].toStdString(), datumRegex)) {
                        throw invalid_argument("Ungültiges Geburtsdatum: Format DD.MM.YYYY erwartet");
                    }
                    if (!regex_match(werteListe[4].toStdString(), geschlechtRegex)) {
                        throw invalid_argument("Ungültiges Geschlecht: Ein Buchstabe m/w/d erwartet");
                    }
                    if (!regex_match(werteListe[8].toStdString(), datumRegex)) {
                        throw invalid_argument("Ungültiges Eintrittsdatum: Format DD.MM.YYYY erwartet");
                    }

                    // Wenn alle Tests bestanden sind, Patient erstellen
                    io_data patient(
                        stoi(werteListe[0].toStdString()), // ID
                        werteListe[1],                     // Vorname
                        werteListe[2],                     // Nachname
                        werteListe[3],                     // Geburtsdatum
                        werteListe[4],                     // Geschlecht
                        werteListe[5],                     // Adresse
                        werteListe[6],                     // Telefonnummer
                        werteListe[7],                     // E-Mail
                        werteListe[8],                     // Eintrittsdatum
                        werteListe[9],                     // Diagnose
                        werteListe[10]                     // Behandlung
                        );

                    // qDebug() << "Patient valide:" << QString::number(patient.ID) << patient.vorname << patient.nachname;

                    // Patient in Datenbank speichern
                    database.insertPatient(patient);

                } catch (const invalid_argument &e) {
                    qDebug() << "Ungültige Daten in Zeile, überspringe. Fehler:" << e.what();
                    QMessageBox::warning(nullptr, "Speichern", "Keine Patientendaten in der Datenbank vorhanden.");
                }
            } else {
                qDebug() << "Unvollständige Zeile, erwartet 11 Werte, erhalten:" << werteListe.size();
                QMessageBox::information(nullptr, "Speichern", "Die Patientendaten wurden erfolgreich in die CSV-Datei exportiert.");
            }
        }

        qDebug() << "Datei fertig eingelesen";

        // Aufräumen und Datei schließen
        datei.close();
        qDebug() << "Datei wieder geschlossen";

    } catch (const exception &e) {
        qDebug() << "Ein Fehler ist aufgetreten:" << e.what();
    }
}

void io_data::CSVerstellen(QString pfad, Database &database) {
    try {
        qDebug() << "io_data::CSVerstellen()";

        // CSV-Datei zum Schreiben öffnen
        ofstream datei(pfad.toStdString(), ios::out);
        if (!datei.is_open()) {
            throw runtime_error("Datei konnte nicht geöffnet werden");
        }
        qDebug() << "Datei zum Schreiben geöffnet:" << pfad;

        // CSV-Header schreiben
        datei << "PatientID,Vorname,Nachname,Geburtsdatum,Geschlecht,Adresse,Telefonnummer,Email,Aufnahmedatum,Diagnose,Behandlung\n";

        // Datenbankabfrage vorbereiten
        QSqlQuery query;
        QString queryString = QString("SELECT PatientID, Vorname, Nachname, Geburtsdatum,"
                                      " Geschlecht, Adresse, Telefonnummer, Email, Aufnahmedatum,"
                                      " Diagnose, Behandlung FROM Patienten");

        if (!query.exec(queryString)) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        // Patienten nacheinander auslesen
        int patientCount = 0; // Zähler für gelesene Patienten
        while (query.next()) {
            io_data patient(
                query.value(0).toInt(),   // PatientID
                query.value(1).toString(), // Vorname
                query.value(2).toString(), // Nachname
                query.value(3).toString(), // Geburtsdatum
                query.value(4).toString(), // Geschlecht
                query.value(5).toString(), // Adresse
                query.value(6).toString(), // Telefonnummer
                query.value(7).toString(), // Email
                query.value(8).toString(), // Aufnahmedatum
                query.value(9).toString(), // Diagnose
                query.value(10).toString() // Behandlung
                );

            // Zeile im CSV-Format erstellen
            QString csvZeile = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11")
                                   .arg(patient.ID)
                                   .arg(patient.vorname)
                                   .arg(patient.nachname)
                                   .arg(patient.geburt)
                                   .arg(patient.geschlecht)
                                   .arg(patient.adresse)
                                   .arg(patient.tel_nummer)
                                   .arg(patient.mail)
                                   .arg(patient.datum)
                                   .arg(patient.diagnose)
                                   .arg(patient.behandlung);

            // Zeile in Datei schreiben
            datei << csvZeile.toStdString() << "\n";
            patientCount++;
        }

        if (patientCount == 0) {
            qDebug() << "Keine Patientendaten in der Datenbank vorhanden.";
        } else {
            qDebug() << "Alle Patientendaten wurden in die CSV-Datei geschrieben.";
        }

        // Datei schließen
        datei.close();
        qDebug() << "CSV-Datei geschlossen";

    } catch (const std::exception &e) {
        qDebug() << "Ein Fehler ist aufgetreten beim Erstellen der CSV-Datei:" << e.what();
    }
}
