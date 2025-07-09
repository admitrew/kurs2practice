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

    bool registerUser(const QString &username, const QString &password);
    bool authenticateUser(const QString &username, const QString &password);
    int getUserId(const QString &username); // <-- ДОБАВИТЬ
    bool addFriend(const QString &username, const QString &friendUsername); // <-- ДОБАВИТЬ
    QStringList getFriends(const QString &username); // <-- ДОБАВИТЬ

private:
    QSqlDatabase db;
    void initializeDatabase();
};

#endif // USERMANAGER_H
