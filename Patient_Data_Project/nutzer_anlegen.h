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
    explicit nutzer_anlegen(QWidget *parent = nullptr);
    ~nutzer_anlegen();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::nutzer_anlegen *ui;
};

#endif // NUTZER_ANLEGEN_H
