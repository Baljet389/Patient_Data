#ifndef USER_H
#define USER_H
//#include <string>         //Better use Qstring instead of string because of QT!
#include <QString>

//libs for Database
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class user
{
public:
    int UID;                //User ID, not to be confused with patient-ID(io_data)
    QString username;
    // user();
    user(int UID, QString name);
    bool checkPW(QString pw_login);             //compares login PW-Hash with DB PW-Hash
    private : QString encrypt_pw(QString password, QString salt); //with Hashing
};

#endif // USER_H
