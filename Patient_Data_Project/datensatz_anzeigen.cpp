#include "datensatz_anzeigen.h"
#include "ui_datensatz_anzeigen.h"
#include "io_data.h"
#include "database.h"
#include "QTableWidgetItem"
#include "QMessageBox"
datensatz_anzeigen::datensatz_anzeigen(QWidget *parent, io_data *selectPatient)
    : QDialog(parent)
    , ui(new Ui::datensatz_anzeigen)
{
    ui->setupUi(this);
    this->selectPatient=selectPatient;
    ui->tableWidget->setItem(0,1, new QTableWidgetItem(selectPatient->nachname));
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(selectPatient->vorname));

    ui->tableWidget->setItem(2,1,new QTableWidgetItem(selectPatient->geburt));
    ui->tableWidget->setItem(3,1,new QTableWidgetItem(selectPatient->geschlecht));
    ui->tableWidget->setItem(5,1,new QTableWidgetItem(selectPatient->adresse));
    ui->tableWidget->setItem(6,1,new QTableWidgetItem(selectPatient->tel_nummer));
    ui->tableWidget->setItem(7,1,new QTableWidgetItem(selectPatient->mail));
    ui->tableWidget->setItem(9,1,new QTableWidgetItem(selectPatient->datum));
    ui->tableWidget->setItem(10,1,new QTableWidgetItem(selectPatient->diagnose));
    ui->tableWidget->setItem(11,1,new QTableWidgetItem(selectPatient->behandlung));

    std::vector<QString> result=Database::getICD_CODE_Information(selectPatient->diagnose);
    ui->textBrowser->append("ICD-Code: "+selectPatient->diagnose);
    for(const QString &res:result){
    ui->textBrowser->append(res);
    }
}

datensatz_anzeigen::~datensatz_anzeigen()
{
    delete ui;

}

void datensatz_anzeigen::on_pushButton_2_clicked()
{
    close();
}


void datensatz_anzeigen::on_pushButton_4_clicked()
{
    // Hier Aufruf des Fensters: "datensatz_bearbeiten", mit entsprechendem Datensatz
}

