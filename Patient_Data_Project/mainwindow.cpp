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
#include <QTimer>
#include "datensatz_bearbeiten.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Elektronische Patientenakte");
    ui->speicher_btn->setToolTip("Datensatz speichern");
    ui->pushButton->setToolTip("Datensatz hinzufügen");
    ui->suche_btn->setToolTip("Suche eingegebenen Text");
    ui->suche_txt_line->setFocusPolicy(Qt::StrongFocus);

    //fügt Datum und realtime ins QTLineEdit
    QTimer *timer = new QTimer(this);

    //Timer und LineEdit wird verbunden
    connect(timer, &QTimer::timeout, this, [&]() {
        // Datum und Uhrzeit berechnen
        date = QDate::currentDate().toString("dd.MM.yyyy");
        time = QTime::currentTime().toString("hh:mm:ss");

        ui->date_time_lbl->setText(date + "\n" + time);
    });

    //Datum und Zeit werden am Anfang initialisiert
    QString initialDate = QDate::currentDate().toString("dd.MM.yyyy");
    QString initialTime = QTime::currentTime().toString("hh:mm:ss");
    ui->date_time_lbl->setText(initialDate + "\n" + initialTime);

    // startet Timer
    timer->start(1000);





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
     ui->logout_btn->setToolTip("Logout");
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
    //ändert Text des Labels
    ui->darkmode_btn->setText("Darkmode");
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

//ändert Text des Labels
    ui->darkmode_btn->setText("Lightmode");
    //Änderung des Designs mit Raw String
    QString darkStyle = R"(
QWidget {
    background-color: #2E2E2E; /* Dunkler Hintergrund */
    color: #E0E0E0; /* Helle Schrift für besseren Kontrast */
}

QPushButton {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #4A6A87, stop:1 #3A556D);
    color: #E0E0E0; /* Helle Schriftfarbe */
    border: 1px solid #3A556D;
    border-radius: 6px;
    padding: 5px 12px;
    font-weight: bold;
}

QPushButton:hover {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #3A556D, stop:1 #2E3E4F);
}

QPushButton:pressed {
    background-color: #3E4F60; /* Etwas dunklerer Farbton */
}

QLineEdit {
    background-color: #3C3C3C; /* Dunkler Hintergrund */
    color: #E0E0E0; /* Helle Schrift */
    border: 1px solid #5A5A5A;
    border-radius: 4px;
    padding: 5px;
}

QLineEdit:focus {
    border-color: #7294AA; /* Akzentfarbe */
}

QTableWidget {
    background-color: #3C3C3C;
    color: #E0E0E0;
    border: 1px solid #5A5A5A;
    gridline-color: #5A5A5A;
    selection-background-color: #4A6A87; /* Akzentfarbe für Auswahl */
}

QHeaderView::section {
    background-color: #4A4A4A; /* Dunklerer Bereich */
    color: #E0E0E0;
    padding: 8px;
    border: 1px solid #5A5A5A;
    font-weight: bold;
}

QComboBox {
    background-color: #3C3C3C;
    color: #E0E0E0;
    border: 1px solid #5A5A5A;
    border-radius: 5px;
    padding: 5px;
}

QComboBox::drop-down {
    background-color: #3C3C3C;
    border: 1px solid #5A5A5A;
}

QComboBox QAbstractItemView {
    background-color: #2E2E2E;
    selection-background-color: #4A6A87; /* Akzentfarbe für Auswahl */
    color: #E0E0E0;
}

QRadioButton {
    background-color: transparent;
    color: #E0E0E0;
}

QRadioButton::indicator {
    border: 2px solid #5A5A5A;
    border-radius: 8px;
    width: 15px;
    height: 15px;
    background-color: #3C3C3C;
}

QRadioButton::indicator:checked {
    background-color: #7294AA; /* Akzentfarbe */
}

QCheckBox {
    background-color: transparent;
    color: #E0E0E0;
}

QCheckBox::indicator {
    border: 2px solid #5A5A5A;
    width: 15px;
    height: 15px;
    background-color: #3C3C3C;
}

QCheckBox::indicator:checked {
    background-color: #7294AA; /* Akzentfarbe */
}

QLabel {
    color: #E0E0E0; /* Helle Schrift */
    font-weight: bold;
}

    // )";
    this->setStyleSheet(darkStyle);
}

void MainWindow::on_speicher_btn_clicked()
{
    qDebug() << "on_speicher_btn_clicked";
}


void MainWindow::on_pushButton_clicked()
{
    auto datensatz_bearbeiten=new Datensatz_bearbeiten(nullptr,-1,&db);
    datensatz_bearbeiten->show();
    datensatz_bearbeiten->setWindowTitle("Datensatz hinzufügen");
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
}
void MainWindow::on_logout_btn_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Bestätigung", "Möchten Sie sich wirklich abmelden?",
    QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        qDebug("Benutzer hat sich abgemeldet.");
        auto l=new LoginDialog;
        l->show();

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

