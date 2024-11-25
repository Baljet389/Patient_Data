#ifndef DATENSATZ_ANZEIGEN_H
#define DATENSATZ_ANZEIGEN_H

#include <QDialog>
#include "io_data.h"
#include "database.h"

namespace Ui {
class datensatz_anzeigen;
}

class datensatz_anzeigen : public QDialog
{
    Q_OBJECT

public:
    io_data *selectPatient;
    explicit datensatz_anzeigen(QWidget *parent = nullptr, io_data *selectPatient=nullptr);
    ~datensatz_anzeigen();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::datensatz_anzeigen *ui;
};

#endif // DATENSATZ_ANZEIGEN_H
