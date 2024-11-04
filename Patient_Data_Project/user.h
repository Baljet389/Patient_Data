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
    int UID;                //User ID, not to be confused with patient/io_data ID
    QString username;
    // user();
    user(int UID, QString name);
    bool checkPW(QString pw_login);             //compares login PW-Hash with DB PW-Hash
    private : QString encrypt_pw(QString password); //with Hashing
};

#endif // USER_H
