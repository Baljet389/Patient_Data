#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
void MainWindow::on_patient_data_scroll_customContextMenuRequested(const QPoint &pos){

}

