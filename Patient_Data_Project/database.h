#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include "io_data.h"
#include <vector>
#include <QString>

class Database
{
public:
    QSqlDatabase db;
    Database();
    void createTable();
    std::vector<io_data> getPatientbyColumn(QString column,QString input);

};

#endif // DATABASE_H
