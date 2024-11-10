#include "datensatz_bearbeiten.h"
#include "ui_datensatz_bearbeiten.h"

Datensatz_bearbeiten::Datensatz_bearbeiten(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Datensatz_bearbeiten)
{
    ui->setupUi(this);
}

Datensatz_bearbeiten::~Datensatz_bearbeiten()
{
    delete ui;
}
