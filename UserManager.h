#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>

class UserManager : public QObject {
    Q_OBJECT

public:
    explicit UserManager(QObject *parent = nullptr);
    bool openDatabase(const QString &dbPath);
    bool registerUser(const QString &username, const QString &password);
    bool authenticateUser(const QString &username, const QString &password);
    QString getAvatarPath(const QString &username);

private:
    QSqlDatabase db;
    QString hashPassword(const QString &password);
    void initializeDatabase();
};

#endif // USERMANAGER_H
