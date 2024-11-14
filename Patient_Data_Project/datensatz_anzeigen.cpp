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
