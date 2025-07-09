#include "UserManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

UserManager::UserManager(QObject *parent) : QObject(parent) {}

bool UserManager::openDatabase(const QString &dbPath) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }
    initializeDatabase();
    return true;
}

void UserManager::initializeDatabase() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT UNIQUE NOT NULL,"
               "password_hash TEXT NOT NULL,"
               "avatar_path TEXT)");

    QString createFriendsTable = "CREATE TABLE IF NOT EXISTS friends ("
                                 "user_id INTEGER NOT NULL,"
                                 "friend_username TEXT NOT NULL,"
                                 "FOREIGN KEY(user_id) REFERENCES users(id))";
    if (!query.exec(createFriendsTable)) {
        qDebug() << "Ошибка создания таблицы друзей:" << query.lastError().text();
    }

}

QString UserManager::hashPassword(const QString &password) {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool UserManager::registerUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password_hash) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(hashPassword(password));
    return query.exec();
}

bool UserManager::authenticateUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password_hash FROM users WHERE username = ?");
    query.addBindValue(username);
    if (query.exec() && query.next()) {
        return query.value(0).toString() == hashPassword(password);
    }
    return false;
}

QString UserManager::getAvatarPath(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT avatar_path FROM users WHERE username = ?");
    query.addBindValue(username);
    if (query.exec() && query.next())
        return query.value(0).toString();
    return {};
}

int UserManager::getUserId(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = ?");
    query.addBindValue(username);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

bool UserManager::addFriend(const QString &username, const QString &friendUsername) {
    int userId = getUserId(username);
    if (userId == -1) return false;

    QSqlQuery query;
    query.prepare("INSERT INTO friends (user_id, friend_username) VALUES (?, ?)");
    query.addBindValue(userId);
    query.addBindValue(friendUsername);
    return query.exec();
}

QStringList UserManager::getFriends(const QString &username) {
    QStringList friends;
    int userId = getUserId(username);
    if (userId == -1) return friends;

    QSqlQuery query;
    query.prepare("SELECT friend_username FROM friends WHERE user_id = ?");
    query.addBindValue(userId);
    if (query.exec()) {
        while (query.next()) {
            friends << query.value(0).toString();
        }
    }
    return friends;
}

