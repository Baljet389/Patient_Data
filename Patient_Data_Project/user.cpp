#include "user.h"
#include <QCryptographicHash>
//#include <stdio.h>
#include <iostream>
//using namespace std;


user::user(int UID, QString username) {
    //this->UID=UID;
    //this->username=username;

    //Debbuging
    this->UID=1;
    this->username="DrHouse";
    QString input = "secretPassword123456";
    qDebug()<<"Input:  "<<input;
    qDebug()<<"Hashed: "<<encrypt_pw(input, "salt12345678");
}

//fehlt: create db connection and create query

bool user::checkPW(QString pw_input){                 //compares entered PW-Hash with DB PW-Hash
    //QString pw_hashed = encrypt_pw(pw_input);

    //qDebug().nospace()<<"UID:"<<UID;
    //qDebug().nospace()<<"username: "<<this->username;

    return false;
}

QString user::encrypt_pw(QString password, QString salt){
    //hash input max 32byte =^ 32 chars
    //Hash Format chars: 32 total = 20 PW + 12 salt

    if(password.length()>20 || salt.length()>12){
        qDebug()<<"ERROR pw or salt length too long";
        return 0;
    }
    password += salt;
    if(password.length()>32){ return 0; /* ERROR */}

    QByteArray hashed = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha3_256);
    //hashing pw+salt
    //ggf. toLocal8bit statt to toUtf8
    //password.toUtf8()  //convert QString to QBA

    return hashed.toHex();                               //convert Encrypted QBA hash to Hex
}
