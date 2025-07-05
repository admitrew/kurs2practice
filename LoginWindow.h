#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "UserManager.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccess(const QString &username);

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

private:
    Ui::LoginWindow *ui;
    UserManager userManager;
};

#endif // LOGINWINDOW_H
