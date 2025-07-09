#include <QApplication>
#include "ChatWindow.h"
#include "UserManager.h"
#include "LoginWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // проверка подключения БД
    UserManager userManager;
    if (userManager.openDatabase("users.db")) {
        qDebug() << "База подключена и инициализирована";
    } else {
        qDebug() << "Не удалось подключиться к базе данных";
    }

    // окно регистрации
    LoginWindow login;
    ChatWindow chat;

    QObject::connect(&login, &LoginWindow::loginSuccess, [&](const QString &username){
        chat.setWindowTitle("Чат - " + username);
        chat.show();
    });

    login.show();
    return app.exec();
}
