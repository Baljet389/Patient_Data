#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "user.h"
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Login");
    ui->name_edit->setFocus();
    design();

    QIcon icon(":/new/prefix1/auge.png");
    ui->auge_btn->setIcon(icon);
    ui->auge_btn->setIconSize(QSize(20, 20));
    ui->auge_btn->setToolTip("Passwort anzeigen");

}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::on_login_btn_clicked()
{
    QString username = ui->name_edit->text();
    QString password = ui->passw_edit->text();
    try{
    akt_user=new user(0,username);
    akt_user->permission=akt_user->checkPW(password);
    if(akt_user->permission!=0){
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
    catch(std::runtime_error &e){
        QMessageBox::warning(this,"Fehler",e.what());
    }
}

void LoginDialog::design()
{
    QString design = R"(
QDialog {
    background-color: #2E2E2E;
    color: #E0E0E0;
}

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
    // )";
this->setStyleSheet(design);

}



//Funktion zum Anzeigen/Verdecken des Passworts mithilfe eines Buttons
void LoginDialog::on_auge_btn_clicked()
{
    if (ui->passw_edit->echoMode() == QLineEdit::Password)
    {

        ui->passw_edit->setEchoMode(QLineEdit::Normal);
        QIcon icon(":/new/prefix1/auge_zu.png");
        ui->auge_btn->setIcon(icon);
        ui->auge_btn->setIconSize(QSize(20, 20));
        ui->auge_btn->setToolTip("Passwort verdecken");


    } else
    {
        ui->passw_edit->setEchoMode(QLineEdit::Password);
        QIcon icon(":/new/prefix1/auge.png");
        ui->auge_btn->setIcon(icon);
        ui->auge_btn->setIconSize(QSize(20, 20));
        ui->auge_btn->setToolTip("Passwort anzeigen");
    }
}

