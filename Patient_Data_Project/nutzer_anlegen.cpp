#include "nutzer_anlegen.h"
#include "ui_nutzer_anlegen.h"

nutzer_anlegen::nutzer_anlegen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::nutzer_anlegen)
{
    ui->setupUi(this);
}

nutzer_anlegen::~nutzer_anlegen()
{
    delete ui;
}

void nutzer_anlegen::on_pushButton_2_clicked()
{
    close();
}


void nutzer_anlegen::on_pushButton_clicked()
{
    close();
}

