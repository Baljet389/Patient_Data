#ifndef NUTZER_ANLEGEN_H
#define NUTZER_ANLEGEN_H

#include <QDialog>
namespace Ui {
class nutzer_anlegen;
}

class nutzer_anlegen : public QDialog
{
    Q_OBJECT

public:
    explicit nutzer_anlegen(QWidget *parent = nullptr, nutzer_anlegen **ptr_nutzer_anlegen_window = nullptr);
    ~nutzer_anlegen();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_nutzer_anlegen_rejected();

    void on_nutzer_anlegen_accepted();

    void on_nutzer_anlegen_finished(int result);

private:
    Ui::nutzer_anlegen *ui;
    nutzer_anlegen **ptr_nutzer_anlegen_window;
};

#endif // NUTZER_ANLEGEN_H
