#include "user.h"
#include <QCryptographicHash>
using namespace std;

user::user(int UID, QString username) { //set standard UID=0!, is UID needed?
    this->UID=UID;
    this->username=username;

    qDebug()<<"Establish DB Connection for User Table";
    db=QSqlDatabase::addDatabase("QSQLITE");
    std::string file=__FILE__;
    std::string directory {file.substr(0, file.rfind("/"))};
    std::string filename="/PatientenDaten/PatientDataDatabase.db";
    db.setDatabaseName(QString::fromStdString(directory+filename));
    if(!db.open()){
        qDebug() << "Error: Could not open the database!" << db.lastError().text();
    }
    //insertUserDB("admin","1234");
    //qDebug()<<salt_generator();
}

bool user::checkPW(QString input_pw) //works, qDebugs are comments
{   //compares entered PW-Hash with DB PW-Hash
    QString DB_salt;
    QString DB_pw="";

    qDebug()<<Qt::endl<<"checkPW(); Read from DB-Table: Users";
    {
        QSqlQuery query;
        QString queryString = QString("SELECT User_ID, Login_Name, PW_Hashed, Salt "
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

            qDebug()<<" this->UID=      "<<this->UID<<Qt::endl<<
                      " this->username= "<<this->username<<Qt::endl<<
                      " DB_PW=          "<<DB_pw<<Qt::endl<<
                      " DB_SALT=        "<<DB_salt<<Qt::endl;
        }else{
            qDebug()<<"No user found with this->username="<<this->username;
        }

    }

    QString input_pw_hashed = encrypt_pw(input_pw, DB_salt);
    if(input_pw_hashed == DB_pw){
        return true;
    }
    return false;
}

QString user::salt_generator(){     //unfinished
    QString seed=username;
    //QString salt="12randomSalt";
    QString salt="";

    qDebug()<<"username="<<username;
    for(int i=0; i<12; i++){
        qDebug()<<"username["<<i<<"]"<<username[i];
        salt+=username[i];
    }
    qDebug()<<"salt"<<salt;
    return salt;
}

QString user::encrypt_pw(QString password, QString salt){ //not DAU safe ->max pw length crash
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

void user::insertUserDB(QString username, QString password){ //not DAU safe? max length!

    //Write to database
    QSqlQuery query;
    query.prepare("INSERT INTO Users(Login_Name,PW_Hashed, Salt)"
                  "VALUES(?,?,?)");

    QString salt=salt_generator();
    query.addBindValue(username);
    query.addBindValue(encrypt_pw(password, salt));
    query.addBindValue(salt);

    bool success=query.exec();
    if(!success){
        throw std::runtime_error("Database connection is not available");
    }
}

