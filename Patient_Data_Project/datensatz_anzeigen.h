#ifndef DATENSATZ_ANZEIGEN_H
#define DATENSATZ_ANZEIGEN_H

#include <QDialog>

namespace Ui {
class datensatz_anzeigen;
}

class datensatz_anzeigen : public QDialog
{
    Q_OBJECT

public:
    explicit datensatz_anzeigen(QWidget *parent = nullptr);
    ~datensatz_anzeigen();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::datensatz_anzeigen *ui;
};

#endif // DATENSATZ_ANZEIGEN_H
