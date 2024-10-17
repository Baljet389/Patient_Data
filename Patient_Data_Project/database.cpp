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
    std::string filename="/PatientenDaten/PatientDataDatabase.db";
    db.setDatabaseName(QString::fromStdString(directory+filename));
    if(!db.open()){
        qDebug() << "Error: Could not open the database!" << db.lastError().text();
    }
}
void Database::createTable(){
    QSqlQuery query;
    QString create="CREATE TABLE IF NOT EXISTS Patienten("
    "PatientID INTEGER PRIMARY KEY,"
    "Vorname TEXT ,"
    "Nachname TEXT ,"
    "Geburtsdatum TEXT ,"
    "Geschlecht TEXT ,"
    "Adresse TEXT ,"
    "Telefonnummer TEXT ,"
    "Email TEXT ,"
    "Aufnahmedatum TEXT ,"
    "Diagnose TEXT ,"
    "Behandlung TEXT )" ;
    bool successful=query.exec(create);
    if(!successful){
        qDebug() << "Error: Could not create the database!" << db.lastError().text();
    }
}
