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
