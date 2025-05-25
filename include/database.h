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
    ~Database();
    void createTable();
    std::vector<io_data> getPatientbyColumn(const QString& column,const QString& input);
    void insertPatient(const io_data& patient);
    void editPatient(const io_data& patient);
    static std::vector<QString> getICD_CODE_Information(const QString &icd_code);
};

#endif // DATABASE_H
