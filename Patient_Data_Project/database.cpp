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
    db.setDatabaseName("PatientDataDatabase.db");
    if(!db.open()){
       throw std::runtime_error((db.lastError().text().toStdString()));
    }
}
Database::~Database(){
    db.close();
}

void Database::createTable(){
    QSqlQuery query;
    QString create="CREATE TABLE IF NOT EXISTS Patienten("
    "PatientID INTEGER PRIMARY KEY AUTOINCREMENT,"
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
vector<io_data> Database::getPatientbyColumn(const QString& column,const QString& input){
    vector<io_data> PatientList;
    QSqlQuery query;
    QString queryString;
        if(column!="PatientID"){
            queryString= QString("SELECT * FROM Patienten WHERE %1 LIKE ?").arg(column);
            query.prepare(queryString);
            query.addBindValue(input+"%");
        }
        else{
            queryString= "SELECT * FROM Patienten WHERE PatientID = ?";
            query.prepare(queryString);
            query.addBindValue(input);
        }

    if(!query.exec()){
        throw std::runtime_error((query.lastError().text().toStdString()));
    }
    while(query.next()){
    io_data data(query.value(0).toInt(),query.value(1).toString(),query.value(2).toString(),query.value(3).toString(),
                    query.value(4).toString(),query.value(5).toString(),query.value(6).toString(),
                    query.value(7).toString(),query.value(8).toString(),query.value(9).toString(),
                    query.value(10).toString());
    PatientList.push_back(std::move(data));
    }
    return PatientList;
}
void Database::insertPatient(const io_data& patient){
    QSqlQuery query;
    query.prepare("INSERT INTO Patienten(Vorname,Nachname,Geburtsdatum,Geschlecht"
                  ",Adresse,Telefonnummer,Email,Aufnahmedatum,Diagnose,Behandlung)"
                  "VALUES(?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(patient.vorname);
    query.addBindValue(patient.nachname);
    query.addBindValue(patient.geburt);
    query.addBindValue(patient.geschlecht);
    query.addBindValue(patient.adresse);
    query.addBindValue(patient.tel_nummer);
    query.addBindValue(patient.mail);
    query.addBindValue(patient.datum);
    query.addBindValue(patient.diagnose);
    query.addBindValue(patient.behandlung);
    if(!query.exec()){
        throw std::runtime_error((query.lastError().text().toStdString()));
    }
}
void Database::editPatient(const io_data &patient){
    QSqlQuery query;
    query.prepare("UPDATE Patienten SET Vorname=?, Nachname=?,Geburtsdatum=?"
                  "Geschlecht=?,Adresse=?,Telefonnummer=?,Email=?,Aufnahmedatum=?"
                  ",Diagnose=?,Behandlung=? WHERE PatientID=?");
    query.addBindValue(patient.vorname);
    query.addBindValue(patient.nachname);
    query.addBindValue(patient.geburt);
    query.addBindValue(patient.geschlecht);
    query.addBindValue(patient.adresse);
    query.addBindValue(patient.tel_nummer);
    query.addBindValue(patient.mail);
    query.addBindValue(patient.datum);
    query.addBindValue(patient.diagnose);
    query.addBindValue(patient.behandlung);
    query.addBindValue(patient.ID);

    if(!query.exec()){
        throw std::runtime_error((query.lastError().text().toStdString()));
    }

}
std::vector<QString> Database::getICD_CODE_Information(const QString &icd_code){
    QSqlQuery query;
    std::vector<QString> result;
    query.prepare("SELECT * FROM icd_codes_kurz WHERE Code=?");
    query.addBindValue(icd_code);
    if(!query.exec()){
        throw std::runtime_error((query.lastError().text().toStdString()));
    }
    while(query.next()){
        result.push_back(std::move(query.value(0).toString()));
        result.push_back(std::move(query.value(2).toString()));
    }
    return result;
}



