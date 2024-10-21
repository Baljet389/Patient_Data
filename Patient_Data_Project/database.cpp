#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include <string>
#include <QString>
#include "io_data.h"
using namespace std;
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
io_data Database::getPatientbyID(int ID){
    QSqlQuery query;
    query.prepare("SELECT * FROM Patienten WHERE PatientID=?");
    query.addBindValue(QString::number(ID));
    bool successful=query.exec();

    if(!successful){
        qDebug() << "Error: Could not retrieve Patient with this ID!" << db.lastError().text();
    }
    query.first();
    io_data data(ID,ToStdString(query.value(1)),ToStdString(query.value(2)),ToStdString(query.value(3)),
                    ToStdString(query.value(4)),ToStdString(query.value(5)),ToStdString(query.value(6)),
                    ToStdString(query.value(7)),ToStdString(query.value(8)),ToStdString(query.value(9)),
                    ToStdString(query.value(10)));
    return data;
}
string Database::ToStdString(const QVariant& value) {
    return value.toString().toStdString();
}


