#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QStyle>
#include <QToolTip>
#include "io_data.h"
#include "database.h"
#include <QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->speicher_btn->setToolTip("Datensatz speichern");
    ui->bearbeiten_btn->setToolTip("Daten bearbeiten");
    ui->pushButton->setToolTip("Daten hinzufügen");
    ui->details_btn->setToolTip("Details der Daten betrachten");
}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_suche_btn_clicked()
{
    QString UserInput=ui->suche_txt_line->text();
    std::vector<io_data> PatientsFound=db.getPatientbyColumn("Nachname",UserInput);
    ui->data_table->setColumnCount(3);
    QStringList SpaltenNamen;
    SpaltenNamen << "ID" << "Name" << "Vorname";
    ui->data_table->setHorizontalHeaderLabels(SpaltenNamen);
    for(const io_data i:PatientsFound){
        int currentRow = ui->data_table->rowCount();
        ui->data_table->insertRow(currentRow);
        ui->data_table->setItem(currentRow, 0, new QTableWidgetItem(QString::number(i.ID)));
        ui->data_table->setItem(currentRow, 1, new QTableWidgetItem(i.vorname));
        ui->data_table->setItem(currentRow, 2, new QTableWidgetItem(i.nachname));

    }
}

//Bei Betätigung der Enter-Taste
void MainWindow::on_suche_txt_edit_returnPressed()
{
    on_suche_btn_clicked();
}

