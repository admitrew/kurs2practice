#include <QApplication>
#include "ChatWindow.h"
#include "UserManager.h"

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

    ChatWindow window;
    window.resize(500, 400);
    window.show();

    return app.exec();
}
