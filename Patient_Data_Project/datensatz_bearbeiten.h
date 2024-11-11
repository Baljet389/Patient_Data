#ifndef DATENSATZ_BEARBEITEN_H
#define DATENSATZ_BEARBEITEN_H

#include <QDialog>

#include <QVBoxLayout>

#include "io_data.h"
#include "database.h"

namespace Ui {
class Datensatz_bearbeiten;
}

class Datensatz_bearbeiten : public QDialog
{
    Q_OBJECT

public:
    io_data* loadPatient;
    Database db;
    std::vector<io_data> PatientFound;
    explicit Datensatz_bearbeiten(QWidget *parent = nullptr,int id=-1);
    ~Datensatz_bearbeiten();


private:
    Ui::Datensatz_bearbeiten *ui;
};

#endif // DATENSATZ_BEARBEITEN_H
