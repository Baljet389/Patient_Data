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

private:
    Ui::datensatz_anzeigen *ui;
};

#endif // DATENSATZ_ANZEIGEN_H
