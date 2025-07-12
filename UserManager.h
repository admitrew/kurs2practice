#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSqlDatabase>

class UserManager : public QObject {
    Q_OBJECT

public:
    explicit UserManager(QObject *parent = nullptr);

    bool openDatabase(const QString &dbPath);                      // открыть БД
    bool registerUser(const QString &username, const QString &password); // регистрация
    bool authenticateUser(const QString &username, const QString &password); // авторизация
    QString getAvatarPath(const QString &username);               // путь к аватару

    int getUserId(const QString &username);                       // получить ID по имени
    bool addFriend(const QString &username, const QString &friendUsername); // добавить друга
    QStringList getFriends(const QString &username);              // список друзей

private:
    QSqlDatabase db;

    void initializeDatabase();                                    // создать таблицы, если нужно
    QString hashPassword(const QString &password);                // хеш пароля
};

#endif // USERMANAGER_H
