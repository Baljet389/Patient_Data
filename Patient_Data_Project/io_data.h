#ifndef IO_DATA_H
#define IO_DATA_H
#include <QString>
#include <QDate>
class Database;
using namespace std;
class io_data
{
public:
    int ID;
    QString vorname;
    QString nachname;
    QString geburt;
    QString geschlecht;
    QString adresse;
    QString tel_nummer;
    QString mail;
    QString datum;
    QString diagnose;
    QString behandlung;
    QString zeile;
    int alterFinal;
    io_data(int ID,QString vorname,QString nachname,QString geburt,QString geschlecht,QString adresse,QString tel_nummer,QString mail,QString datum,QString diagnose,QString behandlung);
    int returnAge();
    void printData();
    static void CSVeinlesen(QString pfad, Database &database);
    static void CSVerstellen(QString pfad, Database &database);
    static QDate convertQStringToQDate(const QString datumString);
    static QString convertQDateToQString(const QDate datum);
};
#endif // IO_DATA_H
