#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class ChatClient : public QObject {
    Q_OBJECT

public:
    explicit ChatClient(QObject *parent = nullptr);
    void startServer(quint16 port);
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &message);

signals:
    void messageReceived(const QString &message);
    void connected();
    void disconnected();
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();
    void onNewConnection();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket = nullptr;
    QTcpServer *server = nullptr;
};
