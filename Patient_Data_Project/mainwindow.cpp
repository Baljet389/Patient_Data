#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QPushButton>
#include <QStyle>
#include <QToolTip>
#include "io_data.h"
#include "database.h"
#include <QString>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include "datensatz_bearbeiten.h"
#include <QFileDialog>

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
    UserInputColumn="PatientID";

    //Lightmode ist zu Beginn
    lightmode_on();
     connect(ui->darkmode_btn, &QRadioButton::toggled, this, &MainWindow::on_darkmode_btn_toggled);

    //logoutbutton connect
     connect(ui->logout_btn, &QPushButton::clicked, this, &MainWindow::on_logout_btn_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_suche_btn_clicked()
{
    ui->data_table->setRowCount(0);
    QString UserInput=ui->suche_txt_line->text();

    try{
    PatientsFound=db.getPatientbyColumn(UserInputColumn,UserInput);
    }
    catch(std::runtime_error &e){
        qDebug(e.what());
        QMessageBox::warning(this,"Datenbankabfrage fehlgeschlagen",e.what());
        return;
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

//Funktion für die Einstellung der Widgets im Lightmode
void MainWindow::lightmode_on()
{
    QString lightStyle = R"(

QWidget {
    background-color: #F8F9FA;
    color: #333333;
}

QPushButton {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #D1E3F4, stop:1 #AFCDE7);
    color: #333333;
    border: 1px solid #AFCDE7;
    border-radius: 6px;
    padding: 5px 12px;
    font-weight: bold;
}

QPushButton:hover {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #AFCDE7, stop:1 #91BEDC);
}

QPushButton:pressed {
    background-color: #A2C2D9;
}

QLineEdit {
    background-color: #FFFFFF;
    color: #333333;
    border: 1px solid #B0B0B0;
    border-radius: 4px;
    padding: 5px;
}

QLineEdit:focus {
    border-color: #91BEDC;
}

QTableWidget {
    background-color: #FFFFFF;
    color: #333333;
    border: 1px solid #DDDDDD;
    gridline-color: #DDDDDD;
    selection-background-color: #D1E3F4;
}

QHeaderView::section {
    background-color: #E8EEF5;
    color: #333333;
    padding: 8px;
    border: 1px solid #DDDDDD;
    font-weight: bold;
}

QComboBox {
    background-color: #FFFFFF;
    color: #333333;
    border: 1px solid #B0B0B0;
    border-radius: 5px;
    padding: 5px;
}

QComboBox::drop-down {
    background-color: #FFFFFF;
    border: 1px solid #B0B0B0;
}

QComboBox QAbstractItemView {
    background-color: #F8F9FA;
    selection-background-color: #D1E3F4;
    color: #333333;
}

QRadioButton {
    background-color: transparent;
    color: #333333;
}

QRadioButton::indicator {
    border: 2px solid #B0B0B0;
    border-radius: 8px;
    width: 15px;
    height: 15px;
    background-color: #FFFFFF;
}

QRadioButton::indicator:checked {
    background-color: #91BEDC;
}

QCheckBox {
    background-color: transparent;
    color: #333333;
}

QCheckBox::indicator {
    border: 2px solid #B0B0B0;
    width: 15px;
    height: 15px;
    background-color: #FFFFFF;
}

QCheckBox::indicator:checked {
    background-color: #91BEDC;
}

QLabel {
    color: #333333;
    font-weight: bold;
}

)";
    this->setStyleSheet(lightStyle);


}

//Funktion für die Einstellung der Widgets im Darkmode
void MainWindow::darkmode_on()
{
    //Änderung des Designs mit Raw String
    QString darkStyle = R"(
QMainWindow {
    background-color: #2E2E2E;
}

QPushButton {
    background-color: #4A4A4A;
    color: #FFFFFF;
    border-radius: 5px;
    border: 1px solid #666666;
}
QPushButton:hover {
    background-color: #666666;
}
QPushButton:pressed {
    background-color: #2B2B2B;
}

QRadioButton {
    color: #FFFFFF;
}
QRadioButton::indicator {
    border: 2px solid #666666;
    background-color: #2E2E2E;
}
QRadioButton::indicator:checked {
    background-color: #007ACC;
}

QTextEdit {
    background-color: #333333;
    color: #FFFFFF;
    border: 1px solid #555555;
    border-radius: 5px;
}

QTableWidget {
    background-color: #444444;
    color: #FFFFFF;
}
QTableWidget::item {
    background-color: #444444;
}
QTableWidget::item:alternate {
    background-color: #555555;
}
QTableWidget::item:selected {
    background-color: #007ACC;
}

QComboBox {
    background-color: #444444;
    color: #FFFFFF;
    border: 1px solid #555555;
    border-radius: 5px;
}

QComboBox::drop-down {
    background-color: #333333;
    border: none;
}

QComboBox::item {
    background-color: #444444;
    color: #FFFFFF;
}

QComboBox::item:selected {
    background-color: #007ACC;
    color: #FFFFFF;
}

QComboBox::item:hover {
    background-color: #555555;
}


QLabel {
    color: #FFFFFF;
}

QLineEdit {
    background-color: #333333;
    color: #FFFFFF;
    border: 1px solid #555555;
    border-radius: 5px;
}
QLineEdit {
    background-color: #333333;
    color: #FFFFFF;
    border: 1px solid #555555;
    border-radius: 5px;
}

QLineEdit:focus {
    border: 1px solid #007ACC;
}

QLineEdit::placeholder {
    color: #AAAAAA;
}

    )";
    this->setStyleSheet(darkStyle);
}

void MainWindow::on_speicher_btn_clicked()
{
    qDebug() << "on_speicher_btn_clicked";
    // Dialog öffnen und den Dateipfad in einer QString-Variable speichern
    QString dateipfad = QFileDialog::getOpenFileName(
        this,
        "Datei öffnen",         // Dialogtitel
        QDir::homePath(),       // Startverzeichnis
        "Alle Dateien (*.*);;Textdateien (*.txt);;Bilder (*.png *.jpg)" // Filter
        );

    // Überprüfen, ob ein Dateipfad ausgewählt wurde
    if (!dateipfad.isEmpty()) {
        qDebug() << "Ausgewählte Datei:" << dateipfad;
        // CSV-Datei lesen und in die Datenbank laden
        io_data::CSVeinlesen(dateipfad, db);
    } else {
        qDebug() << "Keine Datei ausgewählt.";
    }
}


void MainWindow::on_pushButton_clicked()
{
    auto datensatz_bearbeiten=new Datensatz_bearbeiten(nullptr,-1,&db);
    datensatz_bearbeiten->show();
    qDebug() << "on_pushButton_clicked";
}


void MainWindow::on_details_btn_clicked()
{
    qDebug() << "on_details_btn_clicked";
}


void MainWindow::on_bearbeiten_btn_clicked()
{
    qDebug() << "on_bearbeiten_btn_clicked";
}



void MainWindow::on_open_btn_clicked()
{
    // io_data
    qDebug() << "on_open_btn_clicked";

    // Dialog öffnen und den Dateipfad in einer QString-Variable speichern
    QString dateipfad = QFileDialog::getOpenFileName(
        this,
        "Datei öffnen",         // Dialogtitel
        QDir::homePath(),       // Startverzeichnis
        "Alle Dateien (*.*);;Textdateien (*.txt);;Bilder (*.png *.jpg)" // Filter
        );

    // Überprüfen, ob ein Dateipfad ausgewählt wurde
    if (!dateipfad.isEmpty()) {
        qDebug() << "Ausgewählte Datei:" << dateipfad;
        io_data::CSVeinlesen(dateipfad, db);
    } else {
        qDebug() << "Keine Datei ausgewählt.";
    }
}
void MainWindow::on_logout_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Bestätigung", "Möchten Sie sich wirklich abmelden?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        qDebug("Benutzer hat sich abgemeldet.");
        close();
    }
}


void MainWindow::on_filter_box_currentIndexChanged(int index)
{
    switch(index){
    case(0):
        UserInputColumn="PatientID";
        break;
    case(1):
    UserInputColumn="PatientID";
        break;
    case(2):
        UserInputColumn="Vorname";
        break;
    case(3):
        UserInputColumn="Nachname";
        break;
    }
}

