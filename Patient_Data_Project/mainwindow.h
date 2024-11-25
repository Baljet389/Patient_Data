#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include <QLineEdit>
#include <QDebug>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Database* db=nullptr);
    ~MainWindow();
    Database* db;
    QStringList SpaltenNamen;
    std::vector<io_data> PatientsFound;
    QString UserInputColumn;
    bool checked;
    // Teständerung für CSV im/export
    //QString pfad = open_btn.clicked()
    //qDebug() << pfad;
    //void io_data.CSVeinlesen(QString pfad, db);
    //void io_data.CSVerstellen(QString pfad, &db);

private slots:

    void on_suche_btn_clicked();

    void on_suche_txt_line_returnPressed();
    void onSearchTextChanged(const QString &text);


    // Teständerung für CSV im/export
    //void io_data.CSVeinlesen(QString pfad, db);
    //void io_data.CSVerstellen(QString pfad, &db);

    //void on_radioButton_clicked();

    void on_darkmode_btn_toggled(bool checked);
    void lightmode_on();
    void darkmode_on();

    void on_speicher_btn_clicked();

    void on_pushButton_clicked();

    void on_details_btn_clicked();

    void on_bearbeiten_btn_clicked();


    void on_open_btn_clicked();
    void on_logout_btn_clicked();

    void on_filter_box_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    QString date;
    QString time;
};
#endif // MAINWINDOW_H
