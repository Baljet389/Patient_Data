#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "io_data.h"
class Database
{
public:
    QSqlDatabase db;
    Database();
    void createTable();
    io_data getPatientbyID(int ID);
    string  ToStdString(const QVariant& value);
};

#endif // DATABASE_H
