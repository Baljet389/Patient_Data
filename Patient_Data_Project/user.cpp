#include "user.h"
#include <QCryptographicHash>

user::user(int UID, QString username) {
    this->UID=UID;
    this->username=username;
}

bool user::checkPW(QString pw_input){                 //compares login PW-Hash with DB PW-Hash
    QString pw_hashed = encrypt_pw(pw_input);

    //create db connection and create query

    return 0;
}

QString user::encrypt_pw(QString password){       //encrypt with Hashing
    return 0;
}

//, QString pw_hashed
