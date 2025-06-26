#include "ChatClient.h"
#include <QTextStream>

ChatClient::ChatClient(QObject *parent) : QObject(parent) {}

void ChatClient::startServer(quint16 port) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ChatClient::onNewConnection);
    if (!server->listen(QHostAddress::Any, port)) {
        emit errorOccurred("Ошибка запуска сервера");
    }
}

void ChatClient::connectToServer(const QString &host, quint16 port) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    connect(socket, &QTcpSocket::connected, this, &ChatClient::connected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &ChatClient::onError);
    socket->connectToHost(host, port);
}

void ChatClient::sendMessage(const QString &message) {
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
    }
}

void ChatClient::onNewConnection() {
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    emit connected();
}

void ChatClient::onReadyRead() {
    QTextStream stream(socket);
    QString message = stream.readAll();
    emit messageReceived(message);
}

void ChatClient::onDisconnected() {
    emit disconnected();
}

void ChatClient::onError(QAbstractSocket::SocketError) {
    emit errorOccurred(socket->errorString());
}
