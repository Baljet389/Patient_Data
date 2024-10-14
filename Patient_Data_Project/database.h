#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Database
{
public:
    QSqlDatabase db;
    Database();
    void createTable();
};

#endif // DATABASE_H
