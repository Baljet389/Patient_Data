#ifndef DATENSATZ_ANZEIGEN_H
#define DATENSATZ_ANZEIGEN_H

#include <QDialog>
#include "io_data.h"
#include "database.h"
#include "mainwindow.h"
#include "user.h"
namespace Ui {
class datensatz_anzeigen;
}

class datensatz_anzeigen : public QDialog
{
    Q_OBJECT

public:
    Database *db;
    MainWindow *mw;
    int selectID;
    explicit datensatz_anzeigen(QWidget *parent = nullptr, Database* db=nullptr,int selectID=-1);
    ~datensatz_anzeigen();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::datensatz_anzeigen *ui;
};

#endif // DATENSATZ_ANZEIGEN_H
