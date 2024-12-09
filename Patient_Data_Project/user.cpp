#include "user.h"
#include <QCryptographicHash>
using namespace std;

user::user(int UID, QString username) { //set standard UID=0!, is UID needed?
    this->UID=UID;
    this->username=username;
    this->permission=3; //3 ist leseberechtigung
    //insertUserDB("admin2","1234");
    //qDebug()<<"Debug: salt_generator()"<<salt_generator();

    //2do
    //add inserUserDB rechte param!
    //add checkPW return value + user rights (int)

}

int user::checkPW(QString input_pw){
    //return values exlained:
    //0 == error/wrong PW, 1==admin/root, 2==arzt(lesen+schreiben), 3==normal user(nur lesen), ...

    //add extra function vector<query.value> searchUser(QString username)
    //consider qDebugs as comments

    if(input_pw.length()>=21){
        qDebug()<<"Entered Password too long! Exceeds limit of 20 chars.";
        return 0;
    }

    //compares entered PW-Hash with DB PW-Hash
    QString DB_salt;
    QString DB_pw="";
    int DB_permission=0;

    qDebug()<<Qt::endl<<"checkPW(); Read from DB-Table: Users";
    {
        QSqlQuery query;
        QString queryString = QString("SELECT User_ID, Login_Name, PW_Hashed, Salt, Rechte "
                                      "FROM Users WHERE %1 = '%2'").arg("Login_Name").arg(this->username);
        qDebug()<<queryString;  //check for correct SQL Query Syntax
        query.prepare(queryString);
        bool successful=query.exec();

        if(!successful){
            throw std::runtime_error("Column does not exist, or database connection is not available");
        }
        if(query.next()){
            this->UID      = query.value(0).toInt();
            this->username = query.value(1).toString();
            DB_pw          = query.value(2).toString();
            DB_salt        = query.value(3).toString();
            DB_permission  = query.value(4).toInt();

            qDebug()<<" this->UID=      "<<this->UID<<Qt::endl<<
                " this->username= "<<this->username<<Qt::endl<<
                " DB_PW=          "<<DB_pw<<Qt::endl<<
                " DB_SALT=        "<<DB_salt<<Qt::endl<<
                " DB_permission=  "<<DB_permission<<Qt::endl;
        }else{
            qDebug()<<"No user found with this->username="<<this->username;
            qDebug()<<Qt::endl;
        }

    }

    QString input_pw_hashed = encrypt_pw(input_pw, DB_salt);
    if(input_pw_hashed == DB_pw){
        return DB_permission;
    }
    return 0;
}

QString user::changePW(QString old_pw, QString new_pw){
    if(!(checkPW(old_pw)>=1)){
        //return "Falsches Passwort oder Benutzername.";
    }


    QSqlQuery query;
    QString salt=salt_generator(username);
    QString queryString = QString("UPDATE Users SET PW_Hashed='%1', Salt='%2' "
                                  "WHERE Login_Name='%3' ").arg(encrypt_pw(new_pw, salt)).arg(salt).arg(username);
    qDebug()<<queryString;
    query.prepare(queryString);

    bool success=query.exec();
    if(!success){
        throw std::runtime_error("Database connection is not available");
    }

    return "Passwort erfolgreich geändert";

}

QString user::salt_generator(QString username){     //works, add random function
    QString seed=username;
    //QString salt="12randomSalt";
    QString salt="";

    qDebug()<<"username="<<username;
    for(int i=0; i<12 && i<username.length(); i++){
        qDebug()<<"username["<<i<<"]="<<username[i];
        salt+=username[i];
    }
    qDebug()<<"salt"<<salt;
    return salt;
}

QString user::encrypt_pw(QString password, QString salt){ //not DAU safe ->max pw length runtime error
    //Hash input max 32byte =^ 32 chars
    //Hash format in chars: 32 total = 20 pw + 12 salt

    if(password.length()>20 || salt.length()>12){
        qDebug()<<"ERROR pw or salt length too long";
        throw std::runtime_error("ERROR pw or salt length too long");
        return 0;
    }

    password += salt;
    if(password.length()>32){
        throw std::runtime_error("Error while hashing password.");
        return 0; /* ERROR */}

    QByteArray hashed = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha3_256);
    //hashing pw+salt
    //password.toUtf8()  //convert QString to QBA
    //ggf. toLocal8bit statt to toUtf8

    return hashed.toHex();                             //convert Encrypted QByteArray hash to Hex
}

QString user::insertUserDB(QString username, QString password, int permission){ //not DAU safe? max length!, //multiple Username error
    //return type QString -->GUI?
    //or as int and intern

    QString error="";
    if(username.length()>=33){
        error="Neuer Benutzername ist zu lang. Die maximale Länge beträgt 32 Zeichen!";
        qDebug()<<error;
        return error;
    }else if(password.length()>=21){
        error="Neues Passwort ist zu lang. Die maximale Länge beträgt 20 Zeichen!";
        qDebug()<<error;
        return error;
    }


    /*|| Check and search: Is username is already used in DB?*/
    bool vergeben = true; //default true!

    {   QSqlQuery query;
        QString queryString = QString("SELECT Login_Name, Count(*) AS c "
                                      "FROM Users WHERE %1 = '%2'").arg("Login_Name").arg(username);
        qDebug()<<queryString;  //show QUERY to check for correct SQL Syntax
        query.prepare(queryString);
        bool successful=query.exec();

        if(!successful){
            throw std::runtime_error("Column does not exist, or database connection is not available");
        }
        if(query.next()){
            QString temp_uName = query.value(0).toString();
            int c = query.value(1).toInt();
            qDebug()<<" Login_Name="<<temp_uName<<Qt::endl<<
                " c="<<c<<Qt::endl;

            if(c==0){
                vergeben=false;
            }else{
                vergeben=true;
            }

        }else{
            error="DB Query Error.";
            qDebug()<<error;
            return error;
        }

    }

    if(vergeben){
        error="Benutzername '"+username+"' bereits vergeben.";
        qDebug()<<error;
        return error;
    }else{
        //Write to database

        qDebug()<<"Benutzername "<<username<<" ist verfügbar.";

        QSqlQuery query;
        query.prepare("INSERT INTO Users(Login_Name,PW_Hashed, Salt, Rechte)"
                      "VALUES(?,?,?,?)");

        QString salt=salt_generator(username);
        query.addBindValue(username);
        query.addBindValue(encrypt_pw(password, salt));
        query.addBindValue(salt);
        query.addBindValue(permission);

        bool success=query.exec();
        if(!success){
            throw std::runtime_error("Database connection is not available");
        }
        error="Neuer Benutzer erfolgreich angelegt."; //var name error may be confusing
        return error;
    }
}

