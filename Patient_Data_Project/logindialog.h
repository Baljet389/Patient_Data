#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "user.h"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    bool loginSuccessful() const;
    user *akt_user;
private slots:

    void on_login_btn_clicked();
    void design();


    void on_auge_btn_clicked();

private:
    Ui::LoginDialog *ui;
    bool loggedIn;
};

#endif // LOGINDIALOG_H
