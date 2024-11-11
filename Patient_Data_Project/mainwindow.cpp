#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QStyle>
#include <QToolTip>
#include "io_data.h"
#include "database.h"
#include <QString>
#include <QTableWidgetItem>
#include <QHeaderView>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowTitle("Mainwindow");
    ui->setupUi(this);
    ui->speicher_btn->setToolTip("Datensatz speichern");
    ui->pushButton->setToolTip("Daten hinzufügen");
    ui->suche_btn->setToolTip("Suche eingegebenen Text");
    ui->suche_txt_line->setToolTip("Gebe hier den Nachnamen ein");
    ui->suche_txt_line->setFocusPolicy(Qt::StrongFocus);



    //verbindet Suchfeld mit der Funktion onsearchTextChanged
    connect(ui->suche_txt_line, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    ui->data_table->setColumnCount(10);
    SpaltenNamen << "ID" << "Name" << "Vorname"<<"Geburtsdatum"<<"Geschlecht"<<"Adresse"<<"Telefonnummer"<<"Email"<<"Aufnahmedatum"<<"Diagnose"<<"Behandlung";
    ui->data_table->setHorizontalHeaderLabels(SpaltenNamen);
    ui->data_table->horizontalHeader()->setStretchLastSection(true);


    //Lightmode zu Beginn anschalten
    lightmode_on();
     connect(ui->darkmode_btn, &QRadioButton::toggled, this, &MainWindow::on_darkmode_btn_toggled);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_suche_btn_clicked()
{
    ui->data_table->setRowCount(0);
    QString UserInputColumn="Nachname";
    QString UserInput=ui->suche_txt_line->text();

    try{
    PatientsFound=db.getPatientbyColumn(UserInputColumn,UserInput);
    }
    catch(std::runtime_error &e){
        qDebug(e.what());
    }

    for(const io_data &i:PatientsFound){
        int currentRow = ui->data_table->rowCount();
        ui->data_table->insertRow(currentRow);
        ui->data_table->setItem(currentRow, 0, new QTableWidgetItem(QString::number(i.ID)));
        ui->data_table->setItem(currentRow, 1, new QTableWidgetItem(i.nachname));
        ui->data_table->setItem(currentRow, 2, new QTableWidgetItem(i.vorname));
        ui->data_table->setItem(currentRow, 3, new QTableWidgetItem(i.geburt));
        ui->data_table->setItem(currentRow, 4, new QTableWidgetItem(i.geschlecht));
        ui->data_table->setItem(currentRow, 5, new QTableWidgetItem(i.adresse));
        ui->data_table->setItem(currentRow, 6, new QTableWidgetItem(i.tel_nummer));
        ui->data_table->setItem(currentRow, 7, new QTableWidgetItem(i.mail));
        ui->data_table->setItem(currentRow, 8, new QTableWidgetItem(i.datum));
        ui->data_table->setItem(currentRow, 9, new QTableWidgetItem(i.diagnose));
        ui->data_table->setItem(currentRow, 10, new QTableWidgetItem(i.behandlung));

    }
    qDebug("Daten der gesuchten Person sind in der Tabelle zu sehen");
}

//Bei Betätigung der Enter-Taste

void MainWindow::on_suche_txt_line_returnPressed()
{
    on_suche_btn_clicked();
}

void MainWindow::onSearchTextChanged(const QString &text) {
    //wenn Textfeld leer ist leert sich die Tabelle
    if (text.isEmpty()) {
        ui->data_table->setRowCount(0);
    }
}


//void MainWindow::on_open_btn_clicked()
//    connect(ui->open_btn, &QPushButton::clicked, this, &MainWindow::io_data.CSVeinlesen());



void MainWindow::on_darkmode_btn_toggled(bool checked)
{
    if (checked)
    {
        darkmode_on();
    }
    else
    {
        lightmode_on();
    }
}

void MainWindow::lightmode_on()
{
    QString lightStyle = R"(
QString lightModeStyle = R"(
    QMainWindow {
        background-color: rgb(255, 255, 255);
    }

    QLabel {
        color: #333333;
        font-size: 14px;
        font-family: Arial, sans-serif;
    }

    QPushButton {
        background-color: #FFFFFF;
        color: #333333;
        border: 1px solid #CCCCCC;
        border-radius: 8px;
        padding: 8px 15px;
        font-size: 14px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: #E0E0E0;
    }

    QPushButton:pressed {
        background-color: #CCCCCC;
    }

)";
      this->setStyleSheet(lightStyle);


}

void MainWindow::darkmode_on()
{
    //Änderung des Designs mit Raw String
    QString darkStyle = R"(
        QMainWindow
        {
            background-color: #2E2E2E;
            color: #FFFFFF;
        }
        QLabel, QRadioButton, QPushButton
        {
            color: #FFFFFF;
        }
        QPushButton
        {
            background-color: #555555;
            border: 1px solid #AAAAAA;
            border-radius: 5px;
            padding: 5px;
        }
    )";
    this->setStyleSheet(darkStyle);
}

void MainWindow::on_speicher_btn_clicked()
{

}


void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_details_btn_clicked()
{

}


void MainWindow::on_bearbeiten_btn_clicked()
{

}


