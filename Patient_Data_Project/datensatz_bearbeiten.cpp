#include "datensatz_bearbeiten.h"
#include "ui_datensatz_bearbeiten.h"
#include "io_data.h"
#include "database.h"
#include "QString"
#include "QMessageBox"
Datensatz_bearbeiten::Datensatz_bearbeiten(QWidget *parent,int id)
    : QDialog(parent)
    , ui(new Ui::Datensatz_bearbeiten)
{
    ui->setupUi(this);
    try{
    PatientFound=db.getPatientbyColumn("PatientID",QString::number(id));
    }
    catch(std::runtime_error &e){
        QMessageBox::warning(this,"Warning",e.what());
    }
    if(PatientFound.size()==0){
        loadPatient=new io_data(-1,"","","","","","","","","","");
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
    }
}

Datensatz_bearbeiten::~Datensatz_bearbeiten()
{
    delete ui;
    delete loadPatient;
}
