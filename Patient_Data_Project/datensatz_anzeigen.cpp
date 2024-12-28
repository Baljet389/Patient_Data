#include "datensatz_anzeigen.h"
#include "ui_datensatz_anzeigen.h"
#include "io_data.h"
#include "database.h"
#include "QTableWidgetItem"
#include "QMessageBox"
#include "datensatz_bearbeiten.h"
#include "mainwindow.h"
#include "user.h"
#include <QCloseEvent>
datensatz_anzeigen::datensatz_anzeigen(QWidget *parent, Database* db,int selectID)
    : QDialog(parent)
    , ui(new Ui::datensatz_anzeigen)
{
    ui->setupUi(this);
    this->db=db;//Attribute werden gesetzt
    this->selectID=selectID;
    io_data selectPatient=db->getPatientbyColumn("PatientID",QString::number(selectID)).at(0);
    ui->tableWidget->setItem(0,1, new QTableWidgetItem(selectPatient.nachname));//Ui-File wird ausgefüllt
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(selectPatient.vorname));

    ui->tableWidget->setItem(2,1,new QTableWidgetItem(selectPatient.geburt));
    ui->tableWidget->setItem(3,1,new QTableWidgetItem(selectPatient.geschlecht));
    ui->tableWidget->setItem(5,1,new QTableWidgetItem(selectPatient.adresse));
    ui->tableWidget->setItem(6,1,new QTableWidgetItem(selectPatient.tel_nummer));
    ui->tableWidget->setItem(7,1,new QTableWidgetItem(selectPatient.mail));
    ui->tableWidget->setItem(9,1,new QTableWidgetItem(selectPatient.datum));
    ui->tableWidget->setItem(10,1,new QTableWidgetItem(selectPatient.diagnose));
    ui->tableWidget->setItem(11,1,new QTableWidgetItem(selectPatient.behandlung));
    //ICD-Code Informationen und Alter werden hinzugefügt
    std::vector<QString> result=Database::getICD_CODE_Information(selectPatient.diagnose);
    ui->textBrowser->append("ICD-Code: "+selectPatient.diagnose);
    for(const QString &res:result){
    ui->textBrowser->append(res);
    }
    ui->textBrowser->append("Alter: "+QString::number(selectPatient.returnAge()));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->textBrowser->setEnabled(false);
}

void datensatz_anzeigen::on_pushButton_2_clicked()
{
    close();
}

void datensatz_anzeigen::on_pushButton_4_clicked()
{
    //Berechtigung wird überprüft
    if(mw->akt_user!=nullptr && mw->akt_user->permission==3){
        QMessageBox::warning(this,"Fehler","Sie haben nur eine Leseberechtigung");
        return;
    }

    auto datensatz=new Datensatz_bearbeiten(nullptr,selectID,db);
    datensatz->show();
    datensatz->mainwindow=mw;
    datensatz->setStyleSheet(mw->akt_mode);
    // Hier Aufruf des Fensters: "datensatz_bearbeiten", mit entsprechendem Datensatz
    close(); // Aktuelles Fenster schießen
}

void datensatz_anzeigen::closeEvent(QCloseEvent *event)
{
    event->accept();
    qDebug() << "Das Fenster datensatz_anzeigen wurde geschlossen!";
    delete this;
}
    datensatz_anzeigen::~datensatz_anzeigen()
{
    delete ui;
    mw->datensatz_anzeigen_fenster=nullptr;
    qDebug() << "datensatz_anzeigen Destruktor";
}

void datensatz_anzeigen::on_datensatz_anzeigen_rejected()
{
    delete this;
}

