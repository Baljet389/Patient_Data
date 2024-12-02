#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "user.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Login");
    ui->name_edit->setFocus();
    design();
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
        accept();
    }
    else {
        loggedIn = false;
        QMessageBox::warning(this, "Login fehlgeschlagen", "Ungültiger Benutzername oder Passwort.");
    }
}

void LoginDialog::design()
{
    QString design = R"(
QWidget {
    background-color: #2E2E2E;
    color: #E0E0E0;


QPushButton {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #4A6A87, stop:1 #3A556D);
    color: #E0E0E0;
    border: 1px solid #3A556D;
    border-radius: 6px;
    padding: 5px 12px;
    font-weight: bold;
}

QPushButton:hover {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #3A556D, stop:1 #2E3E4F);
}

QPushButton:pressed {
    background-color: #3E4F60;
}

QLineEdit {
    background-color: #3C3C3C;
    color: #E0E0E0;
    border: 1px solid #5A5A5A;
    border-radius: 4px;
    padding: 5px;
}

QLineEdit:focus {
    border-color: #7294AA;
}

QLabel {
    color: #E0E0E0;
    font-weight: bold;
}

     )";

    this->setStyleSheet(design);

}

