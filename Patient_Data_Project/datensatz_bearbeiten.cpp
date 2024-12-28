#include "datensatz_bearbeiten.h"
#include "ui_datensatz_bearbeiten.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "io_data.h"
#include "database.h"
#include "QString"
#include "QMessageBox"
#include "QDate"
#include <QAbstractButton>
#include <QDialog>
#include <QDialogButtonBox>
Datensatz_bearbeiten::Datensatz_bearbeiten(QWidget *parent,int id, Database* database)
    : QDialog(parent)
    , ui(new Ui::Datensatz_bearbeiten)
{
    ui->setupUi(this);
    this->database=database;
    PatientFound=database->getPatientbyColumn("PatientID",QString::number(id));
    //patient wird dynamisch erstellt
    if(PatientFound.size()==0){
        loadPatient=new io_data(-1,"","","","","","","","","","");
        this->id=-1;
    }
    else{
    loadPatient =new io_data(PatientFound.at(0).ID,
                            PatientFound.at(0).vorname,
                            PatientFound.at(0).nachname,
                            PatientFound.at(0).geburt,
                            PatientFound.at(0).geschlecht,
                            PatientFound.at(0).adresse,
                            PatientFound.at(0).tel_nummer,
                            PatientFound.at(0).mail,
                            PatientFound.at(0).datum,
                            PatientFound.at(0).diagnose,
                            PatientFound.at(0).behandlung);
        this->id=id;
    }
    //UI File wird ausgefüllt
    ui->Eingabe_Vorname->setText(loadPatient->vorname);
    ui->Eingabe_Name->setText(loadPatient->nachname);
    ui->Eingabe_Geburtsdatum->setDate(io_data::convertQStringToQDate(loadPatient->geburt));
    ui->Eingabe_Geschlecht->setCurrentText(loadPatient->geschlecht);
    ui->Eingabe_Anschrift->setText(loadPatient->adresse);
    ui->Eingabe_Telefonnummer->setText(loadPatient->tel_nummer);
    ui->Eingabe_Email->setText(loadPatient->mail);
    ui->Eingabe_Aufnahmedatum->setDate(io_data::convertQStringToQDate(loadPatient->datum));
    ui->lineEdit_9_Diagnose->setText(loadPatient->diagnose);
    ui->Eingabe_Behandlung->setText(loadPatient->behandlung);
}

void Datensatz_bearbeiten::on_buttonBox_clicked(QAbstractButton *button)
{
    //Info aus UI- File wird wieder in Patienten Objekt geladen
    QDialogButtonBox *buttonBox = qobject_cast<QDialogButtonBox*>(sender());
    QDialogButtonBox::ButtonRole role = buttonBox->buttonRole(button);
    switch(role){
        case QDialogButtonBox::RejectRole:
            return;
        case QDialogButtonBox::AcceptRole:
            loadPatient->vorname=ui->Eingabe_Vorname->text();
            loadPatient->nachname=ui->Eingabe_Name->text();
            loadPatient->geburt=io_data::convertQDateToQString(ui->Eingabe_Geburtsdatum->date());
            loadPatient->geschlecht=ui->Eingabe_Geschlecht->currentText();
            loadPatient->adresse=ui->Eingabe_Anschrift->text();
            loadPatient->tel_nummer=ui->Eingabe_Telefonnummer->text();
            loadPatient->mail=ui->Eingabe_Email->text();
            loadPatient->datum=io_data::convertQDateToQString(ui->Eingabe_Aufnahmedatum->date());
            loadPatient->diagnose=ui->lineEdit_9_Diagnose->text();
            loadPatient->behandlung=ui->Eingabe_Behandlung->text();
            //Datenbankanfragen werden ausgeführt
            try{
            if(id==-1){
                database->insertPatient(*loadPatient);
            }
            else{
                database->editPatient(*loadPatient);
            }
            }
            catch(std::runtime_error &e){
                QMessageBox::warning(this,"Fehler",e.what());
            }
            if(mainwindow!=nullptr){
             mainwindow->on_suche_btn_clicked();
            }

        }
}

void Datensatz_bearbeiten::closeEvent(QCloseEvent *event)
{
    // event->accept();
    qDebug() << "Das Fenster Datensatz_bearbeiten wurde geschlossen.";
    delete this;
}

Datensatz_bearbeiten::~Datensatz_bearbeiten()
{
    delete ui;
    //Patient wird wieder gelöscht
    delete loadPatient;
    mainwindow->Datensatz_bearbeiten_fenster=nullptr;
    qDebug() << "Datensatz_bearbeiten Destruktor";
}

void Datensatz_bearbeiten::on_Datensatz_bearbeiten_rejected()
{
    delete this;
}


void Datensatz_bearbeiten::on_Datensatz_bearbeiten_finished(int result)
{
    delete this;
}
