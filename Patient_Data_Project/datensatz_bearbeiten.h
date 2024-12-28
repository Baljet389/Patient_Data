#ifndef DATENSATZ_BEARBEITEN_H
#define DATENSATZ_BEARBEITEN_H

#include <QDialog>
#include <QVBoxLayout>
#include <QAbstractButton>
#include "io_data.h"
#include "database.h"
#include "mainwindow.h"
namespace Ui {
class Datensatz_bearbeiten;
}

class Datensatz_bearbeiten : public QDialog
{
    Q_OBJECT

public:
    io_data* loadPatient;
    int id;
    Database* database;
    MainWindow* mainwindow;
    std::vector<io_data> PatientFound;
    explicit Datensatz_bearbeiten(QWidget *parent = nullptr,int id=-1,Database* database=nullptr);
    ~Datensatz_bearbeiten();

protected:
    void closeEvent(QCloseEvent *event) override;  // closeEvent überschreiben

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_Datensatz_bearbeiten_rejected();

    void on_Datensatz_bearbeiten_finished(int result);

private:
    Ui::Datensatz_bearbeiten *ui;
};

#endif // DATENSATZ_BEARBEITEN_H
