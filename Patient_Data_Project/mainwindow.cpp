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
#include <QFileDialog>
#include "datensatz_anzeigen.h"
#include "nutzer_anlegen.h"
#include "user.h"
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent, Database *db,user *akt_user)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Überschrift des Mainwindows umbenennen
    this->setWindowTitle("Elektronische Patientenakte");
    //Text anzeigen beim Hovern
    ui->speicher_btn->setToolTip("Daten als CSV speichern");
    ui->pushButton->setToolTip("Datensatz hinzufügen");
    ui->suche_btn->setToolTip("Suche eingegebenen Text");
    ui->open_btn->setToolTip("Daten aus CSV einlesen");
    ui->add_user_btn->setToolTip("Neuen Benutzer anlegen");
    //setzt Fokus auf Suchzeile
    ui->suche_txt_line->setFocusPolicy(Qt::StrongFocus);
    this->db=db;
    this->akt_user=akt_user;
    if(akt_user!=nullptr){
    berechtigung=akt_user->permission;
    }
    else{
        berechtigung=3;
    }
    akt_mode=lightmode_on();
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

    // startet Timer für Uhrzeit
    timer->start(1000);

    //deaktiviert Schreibfunktion in data_table
    ui->data_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //verbindet Suchfeld mit der Funktion onsearchTextChanged
    connect(ui->suche_txt_line, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    ui->data_table->setColumnCount(10);
    SpaltenNamen << "ID" << "Nachname" << "Vorname"<<"Geburtsdatum"<<"Geschlecht"<<"Adresse"<<"Telefonnummer"<<"Email"<<"Aufnahmedatum"<<"Diagnose"<<"Behandlung";
    ui->data_table->setHorizontalHeaderLabels(SpaltenNamen);
    ui->data_table->horizontalHeader()->setStretchLastSection(true);
    UserInputColumn="PatientID";
    darkmode=false;
    //data_table Zelle clicked -> Ausgabe in TextEdit Feld
    connect(ui->data_table, &QTableWidget::itemClicked, this, &MainWindow::on_data_table_itemClicked);
    //data_table Zeile clicked -> Ausgabe in TextEdit Feld
    connect(ui->data_table->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::on_data_table_rowSelected);

    //Lightmode ist zu Beginn an
    ui->logout_btn->setStyleSheet(
        "QPushButton {"
        "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #F5A09B, stop:1 #E68A84);"
        "    color: #2E2E2E;"
        "    border: 1px solid #E68A84;"
        "    border-radius: 6px;"
        "    padding: 5px 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #E68A84, stop:1 #D4736C);"
        "}"
        "QPushButton:pressed {"
        "    background-color: #C95D55;"
        "}"
        );

     connect(ui->darkmode_btn, &QRadioButton::toggled, this, &MainWindow::on_darkmode_btn_toggled);

    //logoutbutton connect
     connect(ui->logout_btn, &QPushButton::clicked, this, &MainWindow::on_logout_btn_clicked);
     ui->logout_btn->setToolTip("Logout");

     connect(ui->suche_btn, &QPushButton::clicked, this, &MainWindow::on_suche_btn_clicked);

}

void MainWindow::on_suche_btn_clicked()
{
    ui->data_table->setRowCount(0);
    QString UserInput=ui->suche_txt_line->text();
    //Datenbankanfrage wird ausgeführt
    try{
    PatientsFound=db->getPatientbyColumn(UserInputColumn,UserInput);
    }
    catch(std::runtime_error &e){
        qDebug(e.what());
        QMessageBox::warning(this,"Datenbankabfrage fehlgeschlagen",e.what());
        return;
     }
    //Tabelle wird mit Patienten gefüllt
    ui->data_table->blockSignals(true);
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
    ui->data_table->blockSignals(false);
    if(ui->data_table->rowCount()==0){
        return;
    }
    on_data_table_itemClicked(ui->data_table->item(0,0));

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

void MainWindow::on_data_table_itemClicked(QTableWidgetItem *item)
{
    // Holen der aktuellen Zeile, die angeklickt wurde
    int row = item->row();
    QTableWidgetItem *idItem = ui->data_table->item(row, 0); // Erste Spalte: ID

    if (idItem) {
        selectedID = idItem->text().toInt(); // Speichere die ID
        qDebug() << "Selected ID:" << selectedID;
    }

    // Initialisierung eines QString für die Ausgabe
    QString zeilenDetails;

    // Iteriere durch die Spalten der angeklickten Zeile
    for (int col = 0; col < ui->data_table->columnCount(); ++col) {
        QTableWidgetItem *cellItem = ui->data_table->item(row, col);
        if (cellItem) {
            // Füge Spaltennamen und Zellinhalt hinzu
            zeilenDetails += SpaltenNamen[col] + ": " + cellItem->text() + "\n";
        }
    }

    // Setze den Text im QTextEdit-Widget
    ui->textEdit->setPlainText(zeilenDetails);
}

void MainWindow::on_data_table_rowSelected(const QModelIndex &current, const QModelIndex &previous) {
    // Prüfen, ob ein gültiger Index ausgewählt wurde
    if (!current.isValid()) {
        return;
    }

    // Holen Sie die ausgewählte Zeile
    int row = current.row();

    // ID verwalten für Bearbeitenbutton
    QTableWidgetItem *idItem = ui->data_table->item(row, 0); // Erste Spalte: ID

    if (idItem) {
        selectedID = idItem->text().toInt(); // Speichere die ID
        qDebug() << "Selected ID:" << selectedID;
    }


    // Details aus der Zeile extrahieren
    QString zeilenDetails;
    for (int col = 0; col < ui->data_table->columnCount(); ++col) {
        QString header = ui->data_table->horizontalHeaderItem(col)->text();
        QString value = ui->data_table->item(row, col) ? ui->data_table->item(row, col)->text() : "N/A";
        zeilenDetails += header + ": " + value + "\n";
    }

    // Setzen der Details in das TextEdit-Widget
    ui->textEdit->setPlainText(zeilenDetails);
}

void MainWindow::on_darkmode_btn_toggled(bool checked)
{
    darkmode=checked;
    this->checked=checked;
    if (checked)
    {
        ui->logout_btn->setStyleSheet(
            "QPushButton {"
            "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #872828, stop:1 #6D3A3A);"
            "    color: #E0E0E0;"
            "    border: 1px solid #6D3A3A;"
            "    border-radius: 6px;"
            "    padding: 5px 12px;"
            "    font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #6D3A3A, stop:1 #4F2E2E);"
            "}"
            "QPushButton:pressed {"
            "    background-color: #603E3E;"
            "}"
            );
    akt_mode=darkmode_on();
    }
    else
    {
        ui->logout_btn->setStyleSheet(
            "QPushButton {"
            "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #F5A09B, stop:1 #E68A84);"
            "    color: #2E2E2E;"
            "    border: 1px solid #E68A84;"
            "    border-radius: 6px;"
            "    padding: 5px 12px;"
            "    font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #E68A84, stop:1 #D4736C);"
            "}"
            "QPushButton:pressed {"
            "    background-color: #C95D55;"
            "}"
            );
    akt_mode=lightmode_on();

    }
    //Iterieren und Setzten des aktuellen modus in allen offenen Fenstern
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget* widget : topLevelWidgets) {
        if(widget->isVisible() && widget!=this){
            widget->setStyleSheet(akt_mode);
        }
    }
}

//Funktion für die Einstellung der Widgets im Lightmode
QString MainWindow::lightmode_on()
{
    //ändert Text des Labels
    ui->darkmode_btn->setText("Darkmode");
    QString lightStyle = R"(

QWidget {
    background-color: rgba(248, 249, 250, 1);
    color: rgba(51, 51, 51, 1);
}

QPushButton {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #D1E3F4, stop:1 #AFCDE7);
    color: rgba(51, 51, 51, 1);
    border: 1px solid rgba(175, 205, 231, 1);
    border-radius: 6px;
    padding: 5px 12px;
    font-weight: bold;
}


QPushButton:hover {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(175, 205, 231, 1), stop:1 rgba(145, 190, 220, 1));
}

QPushButton:pressed {
    background-color: rgba(162, 194, 217, 1);
}

QLineEdit {
    background-color: rgba(255, 255, 255, 1);
    color: rgba(51, 51, 51, 1);
    border: 1px solid rgba(176, 176, 176, 1);
    border-radius: 4px;
    padding: 5px;
}

QMenuBar {
    background-color: rgba(248, 249, 250, 1);
    color: rgba(51, 51, 51, 1);
    border: none;
}

QMenuBar::item {
    background: transparent;
    padding: 4px 10px;
}

QMenuBar::item:selected {
    background: rgba(209, 227, 244, 1);
    border-radius: 4px;
}

QMenuBar::item:pressed {
    background: rgba(175, 205, 231, 1);
    border-radius: 4px;
}

QMenu {
    background-color: rgba(255, 255, 255, 1);
    color: rgba(51, 51, 51, 1);
    border: 1px solid rgba(221, 221, 221, 1);
    padding: 5px;
}

QMenu::item {
    background: transparent;
    padding: 5px 20px;
    color: rgba(51, 51, 51, 1);
}

QMenu::item:selected {
    background: rgba(209, 227, 244, 1);
    color: rgba(51, 51, 51, 1);
    border-radius: 4px;
}

QMenu::item:disabled {
    color: rgba(176, 176, 176, 1);
}

QMenu::separator {
    height: 1px;
    background: rgba(221, 221, 221, 1);
    margin: 5px 10px;
}

QLineEdit:focus {
    border-color: rgba(145, 190, 220, 1);
}

QTableWidget {
    background-color: rgba(255, 255, 255, 1);
    color: rgba(51, 51, 51, 1);
    border: none;
    gridline-color: rgba(221, 221, 221, 1);
    selection-background-color: rgba(209, 227, 244, 1);
}

QHeaderView::section {
    background-color: rgba(232, 238, 245, 1);
    color: rgba(51, 51, 51, 1);
    padding: 8px;
    border: 1px solid rgba(221, 221, 221, 1);
    font-weight: bold;
}

QTableWidget::item:selected {
    background-color: rgba(74, 144, 226, 1);
    color: rgba(255, 255, 255, 1);
}

QTableCornerButton::section {
    background-color: rgba(248, 249, 250, 1);
    border: none;
}

QComboBox {
    background-color: rgba(255, 255, 255, 1);
    color: rgba(51, 51, 51, 1);
    border: 1px solid rgba(176, 176, 176, 1);
    border-radius: 5px;
    padding: 5px;
}

QComboBox::drop-down {
    background-color: rgba(255, 255, 255, 1);
    border: 1px solid rgba(176, 176, 176, 1);
}

QComboBox QAbstractItemView {
    background-color: rgba(248, 249, 250, 1);
    selection-background-color: rgba(209, 227, 244, 1);
    color: rgba(51, 51, 51, 1);
}

QRadioButton {
    background-color: transparent;
    color: rgba(51, 51, 51, 1);
}

QRadioButton::indicator {
    border: 2px solid rgba(176, 176, 176, 1);
    border-radius: 8px;
    width: 15px;
    height: 15px;
    background-color: rgba(255, 255, 255, 1);
}

QRadioButton::indicator:checked {
    background-color: rgba(145, 190, 220, 1);
}

QCheckBox {
    background-color: transparent;
    color: rgba(51, 51, 51, 1);
}

QCheckBox::indicator {
    border: 2px solid rgba(176, 176, 176, 1);
    width: 15px;
    height: 15px;
    background-color: rgba(255, 255, 255, 1);
}

QCheckBox::indicator:checked {
    background-color: rgba(145, 190, 220, 1);
}

QLabel {
    color: rgba(51, 51, 51, 1);
    font-weight: bold;
}
/*
QTextEdit {
    line-height: 1.5;
    margin: 10px;
}*/

)";
    this->setStyleSheet(lightStyle);
    return lightStyle;
}

//Funktion für die Einstellung der Widgets im Darkmode
QString MainWindow::darkmode_on()
{

//ändert Text des Labels
    ui->darkmode_btn->setText("Lightmode");
    //Änderung des Designs mit Raw String
    QString darkStyle = R"(
QWidget {
    background-color: rgba(46, 46, 46, 1);
    color: rgba(224, 224, 224, 1);
}

QPushButton {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(74, 106, 135, 1), stop:1 rgba(58, 85, 109, 1));
    color: rgba(224, 224, 224, 1);
    border: 1px solid rgba(58, 85, 109, 1);
    border-radius: 6px;
    padding: 5px 12px;
    font-weight: bold;
}

QPushButton:hover {
    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(58, 85, 109, 1), stop:1 rgba(46, 62, 79, 1));
}

QPushButton:pressed {
    background-color: rgba(62, 79, 96, 1);
}

QLineEdit {
    background-color: rgba(60, 60, 60, 1);
    color: rgba(224, 224, 224, 1);
    border: 1px solid rgba(90, 90, 90, 1);
    border-radius: 4px;
    padding: 5px;
}

QLineEdit:focus {
    border-color: rgba(114, 148, 170, 1);
}

QMenuBar {
    background-color: rgba(30, 30, 30, 1);
    color: rgba(255, 255, 255, 1);
    border: none;
}

QMenuBar::item {
    background: transparent;
    padding: 4px 10px;
}

QMenuBar::item:selected {
    background: rgba(60, 60, 60, 1);
    border-radius: 4px;
}

QMenuBar::item:pressed {
    background: rgba(92, 92, 92, 1);
    border-radius: 4px;
}

QMenu {
    background-color: rgba(42, 42, 42, 1);
    color: rgba(255, 255, 255, 1);
    border: 1px solid rgba(60, 60, 60, 1);
    padding: 5px;
}

QMenu::item {
    background: transparent;
    padding: 5px 20px;
    color: rgba(255, 255, 255, 1);
}

QMenu::item:selected {
    background: rgba(60, 60, 60, 1);
    color: rgba(255, 255, 255, 1);
    border-radius: 4px;
}

QMenu::item:disabled {
    color: rgba(119, 119, 119, 1);
}

QMenu::separator {
    height: 1px;
    background: rgba(68, 68, 68, 1);
    margin: 5px 10px;
}

QTableWidget {
    background-color: rgba(60, 60, 60, 1);
    color: rgba(224, 224, 224, 1);
    border: none;
    gridline-color: rgba(90, 90, 90, 1);
    selection-background-color: rgba(74, 106, 135, 1);
}

QHeaderView::section {
    background-color: rgba(74, 74, 74, 1);
    color: rgba(224, 224, 224, 1);
    padding: 8px;
    border: 1px solid rgba(90, 90, 90, 1);
    font-weight: bold;
}

QTableCornerButton::section {
    background-color: rgba(46, 46, 46, 1);
    border: none;
}

QComboBox {
    background-color: rgba(60, 60, 60, 1);
    color: rgba(224, 224, 224, 1);
    border: 1px solid rgba(90, 90, 90, 1);
    border-radius: 5px;
    padding: 5px;
}

QComboBox::drop-down {
    background-color: rgba(60, 60, 60, 1);
    border: 1px solid rgba(90, 90, 90, 1);
}

QComboBox QAbstractItemView {
    background-color: rgba(46, 46, 46, 1);
    selection-background-color: rgba(74, 106, 135, 1);
    color: rgba(224, 224, 224, 1);
}

QRadioButton {
    background-color: transparent;
    color: rgba(224, 224, 224, 1);
}

QRadioButton::indicator {
    border: 2px solid rgba(90, 90, 90, 1);
    border-radius: 8px;
    width: 15px;
    height: 15px;
    background-color: rgba(60, 60, 60, 1);
}

QRadioButton::indicator:checked {
    background-color: rgba(114, 148, 170, 1);
}

QCheckBox {
    background-color: transparent;
    color: rgba(224, 224, 224, 1);
}

QCheckBox::indicator {
    border: 2px solid rgba(90, 90, 90, 1);
    width: 15px;
    height: 15px;
    background-color: rgba(60, 60, 60, 1);
}

QCheckBox::indicator:checked {
    background-color: rgba(114, 148, 170, 1);
}

QLabel {
    color: rgba(224, 224, 224, 1);
    font-weight: bold;
}

QTextEdit {
    line-height: 1.5;
    margin: 10px;
}


    // )";
    this->setStyleSheet(darkStyle);
    return darkStyle;
}

void MainWindow::on_open_btn_clicked()
{
    if(berechtigung==3){
        QMessageBox::warning(this,"Fehler","Sie haben nur eine Leseberechtigung. ");
        return;
    }
    // io_data
    qDebug() << "on_open_btn_clicked";

    // Fenster öffnen und den Dateipfad in einer QString-Variable speichern
    QString dateipfad = QFileDialog::getOpenFileName(
        this,
        "Datei öffnen",         // Fenstertitel
        QDir::homePath(),       // Startverzeichnis
        "Alle Dateien (*.*);;Textdateien (*.txt);;Bilder (*.png *.jpg)" // Filter
        );

    // Überprüfen, ob Dateipfad ausgewählt wurde
    if (!dateipfad.isEmpty()) {
        qDebug() << "Ausgewählte Datei:" << dateipfad;
        io_data::CSVeinlesen(dateipfad, *db);
    } else {
        qDebug() << "Keine Datei ausgewählt.";
    }
}

void MainWindow::on_speicher_btn_clicked()
{
    qDebug() << "on_speicher_btn_clicked";

    // Fenster öffnen, um Speicherort und Dateinamen festzulegen
    QString speicherpfad = QFileDialog::getSaveFileName(
        this,
        "Datei speichern",      // Fenstertitel
        QDir::homePath() + "/Patientendaten.csv", // Standardverzeichnis und Vorschlag für Dateinamen
        "CSV-Dateien (*.csv);;Alle Dateien (*.*)" // Filter für Dateitypen
        );

    // Überprüfen, Pfad gültig
    if (!speicherpfad.isEmpty()) {
        qDebug() << "Speicherpfad:" << speicherpfad;

        // Patientendaten in Datei schreiben
        io_data::CSVerstellen(speicherpfad, *db);

        // Erfolgsmeldung ausgeben
        QMessageBox::information(this, "Speichern", "Die Datei wurde erfolgreich gespeichert.");
    } else {
        qDebug() << "Speichern abgebrochen. Kein Speicherpfad ausgewählt.";
        QMessageBox::warning(this, "Speichern", "Speichern abgebrochen. Bitte wählen Sie einen gültigen Speicherort.");
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(akt_user->permission==3){
        QMessageBox::warning(this,"Fehler","Sie haben nur eine Leseberechtigung");
        return;
    }

/*    if (Datensatz_bearbeiten_fenster != nullptr)
    {
        qDebug() << "Bereits ein Fenster Datensatz_bearbeiten (oder addPatient) offen!";
        QMessageBox::warning(this, "Warnung", "Bereits ein Fenster Datensatz_bearbeiten (oder addPatient) offen!");
        return;
    }*/

    /*
    if (!offeneFenster.isEmpty())
    {
        qDebug() << "Bereits ein Fenster offen! - Aktion wird torzdem zugelassen";
        QMessageBox::warning(this, "Info", "Bereits ein Fenster offen!");
        // return;
    }*/

    if (offeneFenster.size() > 4)
    {
        QMessageBox::warning(this, "Warnung", "Es sind bereits " + QString::number(offeneFenster.size()) + " Datensatz hinzufügen Fenster offen!");
        return;
    }

    qDebug() << "Fenster add Patient wird erzeugt:";

    try{
    auto datensatz_bearbeiten=new Datensatz_bearbeiten(nullptr,-1,db);
    // Datensatz_bearbeiten_fenster = datensatz_bearbeiten;
    offeneFenster.append(datensatz_bearbeiten);
    datensatz_bearbeiten->show();
    datensatz_bearbeiten->setWindowTitle("Datensatz hinzufügen");
    datensatz_bearbeiten->mainwindow=this;
    datensatz_bearbeiten->setStyleSheet(akt_mode);
    qDebug() << "on_pushButton_clicked";
    }
    catch(std::runtime_error &e){
        QMessageBox::warning(this,"Fehler",e.what());
    }
}

void MainWindow::on_details_btn_clicked()
{
    if (selectedID == -1) {
        QMessageBox::warning(this, "Fehler", "Bitte wählen Sie zuerst einen Datensatz aus.");
        return;
    }

    if (datensatz_anzeigen_fenster != nullptr)
    {
        qDebug() << "Bereits ein Fenster datensatz_anzeigen_fenster offen!";
        QMessageBox::warning(this, "Warnung", "Bereits ein Fenster datensatz_anzeigen_fenster offen!");
        return;
    }

    try{
    auto anzeigen=new datensatz_anzeigen(nullptr,db,selectedID);
    datensatz_anzeigen_fenster = anzeigen;
    anzeigen->show();
    anzeigen->mw=this;
    anzeigen->setStyleSheet(akt_mode);
    anzeigen->setWindowTitle("Datensatz anzeigen");
    }
    catch(std::runtime_error &e){
        QMessageBox::warning(this, "Warnung", e.what());
    }
    qDebug() << "on_details_btn_clicked";
}

void MainWindow::on_bearbeiten_btn_clicked()
{
    if(berechtigung==3){
        QMessageBox::warning(this,"Fehler: ","Sie haben nur eine Leseberechtigung.");
        return;
    }

    if (Datensatz_bearbeiten_fenster != nullptr)
    {
        qDebug() << "Bereits ein Fenster zur Bearbeitung eines Patienten offen!";
        QMessageBox::warning(this, "Warnung", "Bereits ein Fenster zur Bearbeitung eines Patienten offen!");
        return;
    }

    qDebug() << "Fenster Datensatz_bearbeiten wird erzeugt:";

    // Fehlerausgabe bei keiner Auswahl
    if (selectedID == -1) {
        QMessageBox::warning(this, "Fehler", "Bitte wählen Sie zuerst einen Datensatz aus.");
        return;
    }
    try{
    auto datensatz_bearbeiten = new Datensatz_bearbeiten(nullptr, selectedID, db);
    // offeneFenster.append(datensatz_bearbeiten);
    Datensatz_bearbeiten_fenster = datensatz_bearbeiten;
    datensatz_bearbeiten->show();
    datensatz_bearbeiten->mainwindow=this;
    datensatz_bearbeiten->setWindowTitle("Datensatz bearbeiten");
    datensatz_bearbeiten->setStyleSheet(akt_mode);
    qDebug() << "on_pushButton_clicked";
    }
    catch(std::runtime_error &e){
        QMessageBox::warning(this, "Fehler", e.what());
    }
}

void MainWindow::on_logout_btn_clicked()
{
    closeAllFenster();
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget* widget : topLevelWidgets) {
        if (widget != this) {
            widget->close();
        }
    }
    qApp->exit(1);
}


void MainWindow::on_filter_box_currentIndexChanged(int index)
{
    //Nach zu suchender Spalte wird verändert
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

void MainWindow::on_data_table_itemChanged(QTableWidgetItem *item)
{
    on_data_table_itemClicked(item);
}

void MainWindow::on_add_user_btn_clicked()
{

    if(berechtigung==3||berechtigung==2){
        QMessageBox::warning(this,"Fehler","Sie haben keine Admin-Rechte.");
        return;
    }

    if (nutzer_anlegen_fenster != nullptr)
    {
        qDebug() << "Bereits ein Fenster nutzer_anlegen_fenster offen!";
        QMessageBox::warning(this, "Warnung", "Bereits ein Fenster nutzer_anlegen_fenster offen!");
        return;
    }

    try{
        auto nutzer=new nutzer_anlegen(this, &nutzer_anlegen_fenster);
        nutzer_anlegen_fenster = nutzer;
        nutzer->show();
        nutzer->setStyleSheet(akt_mode);
        nutzer->setWindowTitle("Nutzer hinzufügen");
    }
    catch(std::runtime_error &e){
        QMessageBox::warning(this, "Fehler", e.what());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    try {
        qDebug() << "void MainWindow::closeEvent(QCloseEvent *event)";
        closeAllFenster();
        const auto topLevelWidgets = QApplication::topLevelWidgets();
        for (QWidget* widget : topLevelWidgets) {
            if (widget != this) {
                widget->close();
            }
        }
        event->accept(); // Accept the close event

    } catch (const std::exception& e) {
        qDebug() << "Fehler im closeEvent: " << e.what();
    } catch (...) {
        qDebug() << "Unbekannter Fehler im closeEvent";
    }
}

 void MainWindow::on_add_user_btn_pressed()
{

}

void MainWindow::disableWindow()    // Nicht mehr benutzt, da Fenster jetzt gegeneinander verriegelt
{
    this->setDisabled(true);  // Das gesamte Fenster wird deaktiviert
}

void MainWindow::enableWindow()     // Nicht mehr benutzt, da Fenster jetzt gegeneinander verriegelt
{
    this->setDisabled(false);  // Das gesamte Fenster wird wieder aktiviert

}

void MainWindow::on_MainWindow_destroyed()
{
    qDebug() << "void MainWindow::on_MainWindow_destroyed()";
    closeAllFenster();
    // delete this;
}

void MainWindow::closeAllFenster()
{
    qDebug() << "void MainWindow::closeAllFenster()";
    try {
        qDebug() << "Prüfe: datensatz_anzeigen_fenster != nullptr";
        if (datensatz_anzeigen_fenster != nullptr)
        {
            qDebug() << "Schließe datensatz_anzeigen_fenster: " << datensatz_anzeigen_fenster;
            datensatz_anzeigen_fenster->close();
        }
    } catch (const std::exception& e) {
        qDebug() << "Fehler beim Schließen von datensatz_anzeigen_fenster: " << e.what();
    }

    try {
        qDebug() << "Prüfe: Datensatz_bearbeiten_fenster != nullptr";
        if (Datensatz_bearbeiten_fenster != nullptr)
        {
            qDebug() << "Schließe Datensatz_bearbeiten_fenster: " << Datensatz_bearbeiten_fenster;
            Datensatz_bearbeiten_fenster->close();
        }
    } catch (const std::exception& e) {
        qDebug() << "Fehler beim Schließen von Datensatz_bearbeiten_fenster: " << e.what();
    }

    try {
        qDebug() << "Prüfe: offeneFenster.size() > 0";
        if (offeneFenster.size() > 0)
        {
            qDebug() << "Schließe offeneFenster mit Größe: " << offeneFenster.size();
            for(auto fenster : offeneFenster)
            {
                if (fenster != nullptr)
                {
                    qDebug() << "Schließe Fenster: " << fenster;
                    fenster->close();
                }
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Fehler beim Schließen von offenen Fenstern: " << e.what();
    }

    try {
        qDebug() << "Prüfe: nutzer_anlegen_fenster != nullptr";
        if (nutzer_anlegen_fenster != nullptr)
        {
            qDebug() << "Schließe nutzer_anlegen_fenster: " << nutzer_anlegen_fenster;
            nutzer_anlegen_fenster->close();
        }
    } catch (const std::exception& e) {
        qDebug() << "Fehler beim Schließen von nutzer_anlegen_fenster: " << e.what();
    }
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow::~MainWindow()";
    closeAllFenster();
    delete ui;
    delete akt_user;
    qDebug() << "MainWindow::~MainWindow() fertig";
}
