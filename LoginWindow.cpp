#include "LoginWindow.h"
#include <QMessageBox>
#include "ui_LoginWindow.h"
#include "ChatWindow.h"
#include "UserManager.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    if (!userManager.openDatabase("users.db")) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть базу данных");
        close();
    }
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked() {
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    if (userManager.authenticateUser(username, password)) {
        ChatWindow *chatWindow = new ChatWindow();
        chatWindow->setCurrentUser(username); // ВАЖНО: здесь передаётся имя
        chatWindow->show();
        this->close();
    } else {
        ui->statusLabel->setText("Неверный логин или пароль.");
    }
}

void LoginWindow::on_registerButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (userManager.registerUser(username, password)) {
        ui->statusLabel->setText("Регистрация прошла успешно");
    } else {
        ui->statusLabel->setText("Пользователь уже существует");
    }
}
