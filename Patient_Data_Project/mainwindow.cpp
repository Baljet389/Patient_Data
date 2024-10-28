#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QStyle>
#include <QToolTip>


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

}

//Bei Betätigung der Enter-Taste
void MainWindow::on_suche_txt_edit_returnPressed()
{

}

