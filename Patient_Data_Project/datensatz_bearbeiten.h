#ifndef DATENSATZ_BEARBEITEN_H
#define DATENSATZ_BEARBEITEN_H

#include <QDialog>
#include <QVBoxLayout>
#include <QAbstractButton>
#include <QResizeEvent> // Für die resizeEvent-Methode
#include <QScrollBar>
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

    explicit Datensatz_bearbeiten(QWidget *parent = nullptr, int id = -1, Database* database = nullptr);
    ~Datensatz_bearbeiten();

protected:
    // Deklaration der resizeEvent-Methode für die dynamische Anpassung
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Datensatz_bearbeiten *ui;
};

#endif // DATENSATZ_BEARBEITEN_H
