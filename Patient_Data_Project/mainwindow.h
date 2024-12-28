#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "database.h"
#include <QLineEdit>
#include <QDebug>
#include <QDateTime>
#include "user.h"

class datensatz_anzeigen;
class Datensatz_bearbeiten;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, Database* db=nullptr,user *akt_user=nullptr);
    ~MainWindow();
    Database* db;
    user *akt_user;
    QStringList SpaltenNamen;
    std::vector<io_data> PatientsFound;
    QString UserInputColumn;
    bool checked;
    int berechtigung;//1=admin; 2=Schreibberechtigung; 3=Leseberechtigung
    bool darkmode;
    QString akt_mode;
    // Teständerung für CSV im/export
    //QString pfad = open_btn.clicked()
    //qDebug() << pfad;
    //void io_data.CSVeinlesen(QString pfad, db);
    //void io_data.CSVerstellen(QString pfad, &db);

    void on_suche_btn_clicked();

    void disableWindow();  // Fenster deaktivieren
    void enableWindow();   // Fenster reaktivieren

    Datensatz_bearbeiten* Datensatz_bearbeiten_fenster = nullptr;
    datensatz_anzeigen* datensatz_anzeigen_fenster = nullptr;

protected:
    void closeEvent(QCloseEvent *event) override;  // closeEvent überschreiben

private slots:

    void on_suche_txt_line_returnPressed();
    void onSearchTextChanged(const QString &text);

    //Details Ausgabe bei Auswahl einer Zelle bzw. Zeile in TextEdit
    void on_data_table_itemClicked(QTableWidgetItem *item);
    void on_data_table_rowSelected(const QModelIndex &current, const QModelIndex &previous);
    void on_data_table_itemChanged(QTableWidgetItem *item);

    //void on_radioButton_clicked();

    void on_darkmode_btn_toggled(bool checked);
    QString lightmode_on();
    QString darkmode_on();

    void on_open_btn_clicked();
    void on_speicher_btn_clicked();
    void on_pushButton_clicked();
    void on_details_btn_clicked();
    void on_bearbeiten_btn_clicked();
    void on_logout_btn_clicked();

    void on_filter_box_currentIndexChanged(int index);

    void on_add_user_btn_clicked();

    void on_add_user_btn_pressed();

private:
    Ui::MainWindow *ui;

    QString date;
    QString time;
    int selectedID = -1;
};
#endif // MAINWINDOW_H
