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
    io_data data(query.value(0).toInt(),query.value(1).toString(),query.value(2).toString(),query.value(3).toString(),
                    query.value(4).toString(),query.value(5).toString(),query.value(6).toString(),
                    query.value(7).toString(),query.value(8).toString(),query.value(9).toString(),
                    query.value(10).toString());
    PatientList.push_back(data);
    }
    return PatientList;
}



