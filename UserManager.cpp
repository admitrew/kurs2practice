#include "UserManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QCryptographicHash>
#include <QDebug>

UserManager::UserManager(QObject *parent) : QObject(parent) { }

bool UserManager::openDatabase(const QString &dbPath) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    initializeDatabase();
    return true;
}

void UserManager::initializeDatabase() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT UNIQUE NOT NULL, "
               "password TEXT NOT NULL, "
               "avatar TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS friends ("
               "user_id INTEGER NOT NULL, "
               "friend_id INTEGER NOT NULL, "
               "UNIQUE(user_id, friend_id))");
}

QString UserManager::hashPassword(const QString &password) {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool UserManager::registerUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(hashPassword(password));
    if (!query.exec()) {
        qWarning() << "Register failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool UserManager::authenticateUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);
    if (query.exec() && query.next()) {
        return query.value(0).toString() == hashPassword(password);
    }
    return false;
}

QString UserManager::getAvatarPath(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT avatar FROM users WHERE username = ?");
    query.addBindValue(username);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return QString();
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
    int friendId = getUserId(friendUsername);

    qDebug() << "addFriend:" << username << "=>" << friendUsername << "IDs:" << userId << "->" << friendId;

    if (userId == -1 || friendId == -1 || userId == friendId) {
        qWarning() << "addFriend failed: invalid user IDs or same user";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO friends (user_id, friend_id) VALUES (?, ?)");
    query.addBindValue(userId);
    query.addBindValue(friendId);

    bool ok = query.exec();
    if (!ok)
        qWarning() << "Insert failed:" << query.lastError().text();

    if (username.isEmpty() || friendUsername.isEmpty()) {
        qWarning() << "addFriend: пустое имя";
        return false;
    }
    return ok;
}


QStringList UserManager::getFriends(const QString &username) {
    QStringList list;
    int userId = getUserId(username);
    if (userId == -1) return list;

    QSqlQuery query;
    query.prepare("SELECT u.username FROM friends f "
                  "JOIN users u ON f.friend_id = u.id "
                  "WHERE f.user_id = ?");
    query.addBindValue(userId);
    if (query.exec()) {
        while (query.next()) {
            list.append(query.value(0).toString());
        }
    }
    return list;
}
