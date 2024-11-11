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

    // Teständerung für CSV im/export

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
