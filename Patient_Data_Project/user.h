#ifndef USER_H
#define USER_H
#include <QString>

//libs for Database
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class user
{
public:
    int UID;                //User ID, not to be confused with patient-ID (from io_data)
    QString username;
    QSqlDatabase db;

    user(int UID, QString name);
    public : int checkPW(QString pw_login);                                 //compares login PW-Hash with DB PW-Hash
    private : QString salt_generator(QString username);                             //seed with extra parameter or this->username?
    private : QString encrypt_pw(QString password, QString salt);   //Hash function with SHA256
    public : QString insertUserDB(QString username, QString password, int permission);
    public : QString changePW(QString old_pw, QString new_pw);
};

#endif // USER_H
