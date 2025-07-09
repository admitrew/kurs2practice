#include "ChatWindow.h"
#include "ChatClient.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QListWidget>
#include <QTime>

ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent)
{
    chatHistory = new QTextEdit(this);
    chatHistory->setReadOnly(true);

    inputMessage = new QLineEdit(this);
    sendButton = new QPushButton("Отправить", this);

    modeComboBox = new QComboBox(this);
    modeComboBox->addItem("Клиент");
    modeComboBox->addItem("Сервер");

    inputIP = new QLineEdit(this);
    inputIP->setPlaceholderText("IP-адрес");

    inputPort = new QLineEdit(this);
    inputPort->setPlaceholderText("Порт");

    connectButton = new QPushButton("Подключиться", this);
    statusLabel = new QLabel("Отключено", this);

    friendsListWidget = new QListWidget(this);
    friendNameLineEdit = new QLineEdit(this);
    friendNameLineEdit->setPlaceholderText("Имя друга");

    addFriendButton = new QPushButton("Добавить", this);

    chatClient = new ChatClient(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *modeLayout = new QHBoxLayout;
    QHBoxLayout *connLayout = new QHBoxLayout;
    QHBoxLayout *sendLayout = new QHBoxLayout;
    QHBoxLayout *friendLayout = new QHBoxLayout;

    modeLayout->addWidget(new QLabel("Режим:", this));
    modeLayout->addWidget(modeComboBox);

    connLayout->addWidget(inputIP);
    connLayout->addWidget(inputPort);
    connLayout->addWidget(connectButton);

    sendLayout->addWidget(inputMessage);
    sendLayout->addWidget(sendButton);

    friendLayout->addWidget(friendNameLineEdit);
    friendLayout->addWidget(addFriendButton);

    mainLayout->addLayout(modeLayout);
    mainLayout->addLayout(connLayout);
    mainLayout->addWidget(chatHistory);
    mainLayout->addLayout(sendLayout);
    mainLayout->addWidget(statusLabel);

    mainLayout->addWidget(new QLabel("Друзья:", this));
    mainLayout->addWidget(friendsListWidget);
    mainLayout->addLayout(friendLayout);

    setLayout(mainLayout);
    setWindowTitle("Простой чат");

    // Сигналы и слоты
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendClicked);
    connect(connectButton, &QPushButton::clicked, this, &ChatWindow::onConnectClicked);
    connect(modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChatWindow::onModeChanged);

    connect(chatClient, &ChatClient::messageReceived, this, &ChatWindow::onMessageReceived);
    connect(chatClient, &ChatClient::connected, this, &ChatWindow::onConnected);
    connect(chatClient, &ChatClient::disconnected, this, &ChatWindow::onDisconnected);
    connect(chatClient, &ChatClient::errorOccurred, this, &ChatWindow::onErrorOccurred);

    connect(addFriendButton, &QPushButton::clicked, this, &ChatWindow::on_addFriendButton_clicked);

    // Применяем начальный режим
    onModeChanged(modeComboBox->currentIndex());
}

void ChatWindow::onModeChanged(int index)
{
    bool isClient = (index == 0);
    inputIP->setEnabled(isClient);
    inputIP->setPlaceholderText(isClient ? "IP-адрес" : "— сервер —");
}

void ChatWindow::onConnectClicked()
{
    QString ip = inputIP->text().trimmed();
    quint16 port = inputPort->text().toUShort();

    if (port == 0) {
        statusLabel->setText("Введите корректный порт");
        return;
    }

    bool isClient = (modeComboBox->currentIndex() == 0);

    if (isClient) {
        if (ip.isEmpty()) {
            statusLabel->setText("Введите IP-адрес");
            return;
        }
        chatClient->connectToServer(ip, port);
        statusLabel->setText("Подключение...");
    } else {
        chatClient->startServer(port);
        statusLabel->setText("Ожидание подключения...");
    }
}

void ChatWindow::onSendClicked()
{
    QString message = inputMessage->text();
    if (!message.isEmpty()) {
        chatClient->sendMessage(message);
        QString time = QTime::currentTime().toString("HH:mm:ss");
        chatHistory->append("[" + time + "] Я: " + message);
        inputMessage->clear();
    }
}

void ChatWindow::onMessageReceived(const QString &message)
{
    QString time = QTime::currentTime().toString("HH:mm:ss");
    chatHistory->append("[" + time + "] Собеседник: " + message);
}

void ChatWindow::onConnected()
{
    statusLabel->setText("Подключено");
}

void ChatWindow::onDisconnected()
{
    statusLabel->setText("Отключено");
}

void ChatWindow::onErrorOccurred(const QString &error)
{
    statusLabel->setText("Ошибка: " + error);
}

void ChatWindow::setCurrentUser(const QString &username) {
    currentUsername = username;
    setWindowTitle("Чат — " + username);
    loadFriends();
}

void ChatWindow::loadFriends() {
    friendsListWidget->clear();
    QStringList friends = userManager.getFriends(currentUsername);
    for (const QString &friendName : friends) {
        friendsListWidget->addItem(friendName);
    }
}

void ChatWindow::on_addFriendButton_clicked() {
    QString friendName = friendNameLineEdit->text().trimmed();
    if (friendName.isEmpty() || friendName == currentUsername) return;

    if (userManager.addFriend(currentUsername, friendName)) {
        friendsListWidget->addItem(friendName);
        friendNameLineEdit->clear();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить друга");
    }
}
