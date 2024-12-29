#include "nutzer_anlegen.h"
#include "ui_nutzer_anlegen.h"
#include "user.h"
#include "QMessageBox"

nutzer_anlegen::nutzer_anlegen(QWidget *parent, nutzer_anlegen **ptr_nutzer_anlegen_window)
    : QDialog(parent)
    , ui(new Ui::nutzer_anlegen)
    , ptr_nutzer_anlegen_window(ptr_nutzer_anlegen_window)
{
    ui->setupUi(this);
    connect(ui->Button_Benutzer_anlegen, &QPushButton::clicked, this, &nutzer_anlegen::on_pushButton_clicked);
    connect(ui->Button_abbrechen, &QPushButton::clicked, this, &nutzer_anlegen::on_pushButton_2_clicked);
}

void nutzer_anlegen::on_pushButton_2_clicked()
{

    close();
    delete this;
}


void nutzer_anlegen::on_pushButton_clicked()
{
    QString username = ui->Eingabe_Benutzername->text();
    QString password = ui->Eingabe_Passwort->text();
    QString password_wdh = ui->Eingabe_Passwort_WDH->text();
    QString permission = ui->Eingabe_Nutzerprofil->currentText();

    if(password!=password_wdh){
        QMessageBox::information(this, "Fehler", "Passwort stimmt nicht überein.");
        return;
    }

    int p = 0; //0 ist Fehler

    if(permission=="Administrator"){
        p=1;
    }else if(permission=="Schreibberechtigt"){
        p=2;
    }else if(permission=="Leseberechtigt"){
        p=3;
    }

    user temp_user(0,username);

    QString success = temp_user.insertUserDB(username, password, p);
    if(success == "Neuer Benutzer erfolgreich angelegt."){
        QMessageBox::information(this, "Benutzer hinzufügen",  success);
        close();
    }else{
        QMessageBox::information(this, "Fehler", success);
    }

}

nutzer_anlegen::~nutzer_anlegen()
{
    delete ui;
    if (ptr_nutzer_anlegen_window != nullptr)
    {
        *ptr_nutzer_anlegen_window = nullptr;
    }
}
