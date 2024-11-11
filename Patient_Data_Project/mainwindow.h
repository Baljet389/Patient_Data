#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Database db;
    QStringList SpaltenNamen;
    std::vector<io_data> PatientsFound;

private slots:

    void on_suche_btn_clicked();

    void on_suche_txt_line_returnPressed();
    void onSearchTextChanged(const QString &text);

    void on_radioButton_clicked();

    void on_darkmode_btn_toggled(bool checked);
    void lightmode_on();
    void darkmode_on();

    void on_speicher_btn_clicked();

    void on_pushButton_clicked();

    void on_details_btn_clicked();

    void on_bearbeiten_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
