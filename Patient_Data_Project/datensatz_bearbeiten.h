#ifndef DATENSATZ_BEARBEITEN_H
#define DATENSATZ_BEARBEITEN_H

#include <QDialog>
#include <QVBoxLayout>

namespace Ui {
class Datensatz_bearbeiten;
}

class Datensatz_bearbeiten : public QDialog
{
    Q_OBJECT

public:
    explicit Datensatz_bearbeiten(QWidget *parent = nullptr);
    ~Datensatz_bearbeiten();

private:
    Ui::Datensatz_bearbeiten *ui;
};

#endif // DATENSATZ_BEARBEITEN_H
