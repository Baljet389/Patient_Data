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
private slots:

    void on_suche_btn_clicked();

    void on_suche_txt_edit_returnPressed();




private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
