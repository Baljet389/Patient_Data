#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include <string>
#include <QString>
Database::Database() {
    db=QSqlDatabase::addDatabase("QSQLITE");
    std::string file=__FILE__;
    std::string directory {file.substr(0, file.rfind("/"))};
    std::string filename="/PatientDataDatabase.db";

    db.setDatabaseName(QString::fromStdString(directory+filename));
    if(!db.open()){
        qDebug() << "Error: Could not open the database!" << db.lastError().text();
    }
}
void Database::createTable(){
    QSqlQuery query;
    QString create="CREATE TABLE IF NOT EXISTS Patients("
    "id INTEGER PRIMARY KEY,"
    "Vorname CHAR NOT NULL,"
    "Nachname CHAR NOT NULL,"
    "Geburtsdatum CHAR NOT NULL,"
    "Geschlecht CHAR NOT NULL,"
    "Adresse CHAR NOT NULL,"
    "Telefonnummer CHAR NOT NULL,"
    "Email CHAR NOT NULL,"
    "Aufnahmedatum CHAR NOT NULL,"
    "Diagnose CHAR NOT NULL,"
    "Behandlung CHAR NOT NULL)" ;
    bool abc=query.exec(create);
    int abs=0;
}
