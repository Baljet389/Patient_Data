#include "datensatz_anzeigen.h"
#include "ui_datensatz_anzeigen.h"

datensatz_anzeigen::datensatz_anzeigen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::datensatz_anzeigen)
{
    ui->setupUi(this);
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

