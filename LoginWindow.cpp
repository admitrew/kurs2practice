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

    this->setStyleSheet(R"(
    QWidget {
        background-color: #f4f6f8;
        font-family: Segoe UI, sans-serif;
        font-size: 12pt;
    }

    QLineEdit {
        background-color: #ffffff;
        border: 1px solid #c0c0c0;
        border-radius: 5px;
        padding: 6px;
    }

    QPushButton {
        background-color: #0078d7;
        color: white;
        border: none;
        border-radius: 5px;
        padding: 8px 16px;
    }

    QPushButton:hover {
        background-color: #005fa1;
    }

    QPushButton:pressed {
        background-color: #004578;
    }

    QLabel {
        color: #333333;
    })");

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
