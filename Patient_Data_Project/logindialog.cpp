#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "user.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->name_edit->setFocus();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::on_login_btn_clicked()
{
    QString username = ui->name_edit->text();
    QString password = ui->passw_edit->text();

    user temp_user(0,username);
    if(temp_user.checkPW(password)){
        loggedIn = true;
        // QMessageBox::information(this, "Login erfolgreich", "'Ok' um zum Mainwindow zu gelangen");
        qDebug()<<"Login successful";
        QMessageBox::information(this, "Login erfolgreich", "'Ok' um zum Mainwindow zu gelangen");
        accept();
    }
    else {
        loggedIn = false;
        QMessageBox::warning(this, "Login fehlgeschlagen", "Ungültiger Benutzername oder Passwort.");
    }
}

