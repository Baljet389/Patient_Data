#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

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

    if (username == "" && password == "") {
        loggedIn = true;
        accept();
    }
    else {
        loggedIn = false;
        QMessageBox::warning(this, "Login fehlgeschlagen", "Ungültiger Benutzername oder Passwort.");
    }
}

