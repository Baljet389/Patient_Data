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
#include <QSqlError>

#include <QProgressDialog>
#include <QMessageBox>

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

// Konvertiere den QString in ein QDate
QDate io_data::convertQStringToQDate(const QString datumString) {
    QDate datum = QDate::fromString(datumString, "dd.MM.yyyy");

    // Überprüfe, ob das Datum gültig ist
    if (!datum.isValid()) {
        qWarning() << "Ungültiges Datum im Format TT.MM.JJJJ erwartet:" << datumString;
        return QDate();  // Gibt ein ungültiges QDate zurück, falls das Format fehlerhaft ist
    }
    return datum;
}

// Konvertiere das QDate in einen QString im Format "dd.MM.yyyy" und gib es zurück
QString io_data::convertQDateToQString(const QDate datum) {
    return datum.toString("dd.MM.yyyy");
}

void io_data::CSVeinlesen(QString pfad,Database &database) {
    // qDebug() << "io_data::CSVeinlesen()";
    try {
        // CSV-Datei zum einlesen öffnen
        ifstream datei(pfad.toStdString());
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
        regex geschlechtRegex("^[mwMWdD]$");                            // Nur Einzelbuchstaben für Geschlecht

        // Anzahl der Zeilen bestimmen
        int zeilenAnzahl = 0;
        while (getline(datei, zeile)) {
            if (skipUeberschrift) {
                skipUeberschrift = false;
                continue; // Header überspringen
            }
            zeilenAnzahl++;
        }

        int daten_lesen = QMessageBox::question(
            nullptr,
            "CSV einlesen",
            "Die CSV-Datei enthält " + QString::number(zeilenAnzahl) + " Zeilen!\nDiese werden jetzt der Datenbank hinzugefügt, bitte bestätigen.\nWarnung: Je nach Systemleistung kann dies einige Zeit in anspruch nehmen!",
            QMessageBox::Yes | QMessageBox::No
            );

        if (daten_lesen == QMessageBox::Yes) {
            qDebug() << "Benutzer hat 'Ja' gewählt.";
            // Fortfahren
        } else {
            qDebug() << "Benutzer hat 'Nein' gewählt.";
            QMessageBox::warning(nullptr, "Abbruch", "CSV einlesen abgebrochen");

            return; // Aktion abbrechen
        }

        datei.clear(); // Lösche den EOF-Status
        datei.seekg(0); // Setze den Lesezeiger an den Anfang der Datei

        // QProgressDialog initialisieren
        QProgressDialog progress("CSV-Datei wird eingelesen...", "Abbrechen", 0, zeilenAnzahl, nullptr);
        progress.setWindowModality(Qt::WindowModal);
        progress.setValue(0);  // Startwert
        progress.show();

        int zeilenGelesen = 0;

        while (getline(datei, zeile)) {
            if (skipUeberschrift) {
                qDebug() << "Überspringe Überschrift:" << QString::fromStdString(zeile);
                skipUeberschrift = false;
                continue; // Header überspringen
            }

            // Konvertiere Zeile von std::string zu QString aus UTF-8
            QString zeileQString = QString::fromUtf8(zeile.c_str());
            //qDebug() << "QString: " << zeileQString;

            // Parse Zeile am Komma mit QString
            QStringList werteListe = zeileQString.split(',');

            // Prüfe, ob Daten valide
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

                    // Wenn valide, dann Patient erstellen
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

                     qDebug() << "Patient valide:" << QString::number(patient.ID) << patient.vorname << patient.nachname;

                     /*
                    qDebug() << "Patientendaten:"
                             << "ID:" << werteListe[0]
                             << "Vorname:" << werteListe[1]
                             << "Nachname:" << werteListe[2]
                             << "Geburtsdatum:" << werteListe[3]
                             << "Geschlecht:" << werteListe[4]
                             << "Adresse:" << werteListe[5]
                             << "Telefonnummer:" << werteListe[6]
                             << "E-Mail:" << werteListe[7]
                             << "Eintrittsdatum:" << werteListe[8]
                             << "Diagnose:" << werteListe[9]
                             << "Behandlung:" << werteListe[10];
                     */
                    // Patient in Datenbank speichern
                    database.insertPatient(patient);
                    qDebug() << "Datei geöffnet:" << pfad;
                } catch (const invalid_argument &e) {
                    qDebug() << "Ungültige Daten in Zeile, überspringe. Fehler:" << e.what();
                }
            } else {
                qDebug() << "Unvollständige Zeile, erwartet 11 Werte, erhalten:" << werteListe.size();

                zeilenGelesen++;
                progress.setValue(zeilenGelesen);  // Fortschritt aktualisieren

                // Abbrechen prüfen
                if (progress.wasCanceled()) {
                    qDebug() << "CSV Einlesen abgebrochen.";
                    break;
                }
            }
        }

        // qDebug() << "Datei fertig eingelesen";

        // Aufräumen und Datei schließen
        datei.close();
        // qDebug() << "Datei wieder geschlossen";

    } catch (const exception &e) {
        qDebug() << "Ein Fehler ist aufgetreten:" << e.what();
    }
}

void io_data::CSVerstellen(QString pfad, Database &database) {
    try {
        // qDebug() << "io_data::CSVerstellen()";

        // CSV-Datei zum Schreiben öffnen
        ofstream datei(pfad.toStdString(), ios::out);
        if (!datei.is_open()) {
            throw runtime_error("Datei konnte nicht geöffnet werden");
        }
        // qDebug() << "Datei zum Schreiben geöffnet:" << pfad;

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
                query.value(0).toInt(),     // PatientID
                query.value(1).toString(),  // Vorname
                query.value(2).toString(),  // Nachname
                query.value(3).toString(),  // Geburtsdatum
                query.value(4).toString(),  // Geschlecht
                query.value(5).toString(),  // Adresse
                query.value(6).toString(),  // Telefonnummer
                query.value(7).toString(),  // Email
                query.value(8).toString(),  // Aufnahmedatum
                query.value(9).toString(),  // Diagnose
                query.value(10).toString()  // Behandlung
                );

            // Zeile im CSV-Format erstellen
            QString csvZeile = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11")
                                   .arg(QString::number(patient.ID),
                                        patient.vorname,
                                        patient.nachname,
                                        patient.geburt,
                                        patient.geschlecht,
                                        patient.adresse,
                                        patient.tel_nummer,
                                        patient.mail,
                                        patient.datum,
                                        patient.diagnose,
                                        patient.behandlung);


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
