#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include <string>
#include <QString>
#include <vector>
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
vector<io_data> Database::getPatientbyColumn(QString column,QString input){
    vector<io_data> PatientList;
    QSqlQuery query;
    QString queryString = QString("SELECT PatientID, Vorname, Nachname, Geburtsdatum,"
                                  " Geschlecht, Adresse, Telefonnummer, Email, Aufnahmedatum,"
                                  " Diagnose, Behandlung FROM Patienten WHERE %1 = ?").arg(column);

    query.prepare(queryString);
    query.addBindValue(input);
    bool successful=query.exec();

    if(!successful){
        throw "Column does not exist, or database connection is not available";
    }
    while(query.next()){
    io_data data(query.value(0).toInt(),ToStdString(query.value(1)),ToStdString(query.value(2)),ToStdString(query.value(3)),
                    ToStdString(query.value(4)),ToStdString(query.value(5)),ToStdString(query.value(6)),
                    ToStdString(query.value(7)),ToStdString(query.value(8)),ToStdString(query.value(9)),
                    ToStdString(query.value(10)));
    PatientList.push_back(data);
    }
    return PatientList;
}
string Database::ToStdString(const QVariant& value) {
    return value.toString().toStdString();
}


