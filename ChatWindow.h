#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include "UserManager.h"

class QTextEdit;
class QLineEdit;
class QPushButton;
class QLabel;
class QComboBox;
class QListWidget;
class ChatClient;

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

public slots:
    void setCurrentUser(const QString &username);

private slots:
    void onSendClicked();
    void onConnectClicked();
    void onModeChanged(int index);

    void onMessageReceived(const QString &message);
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(const QString &error);

    void on_addFriendButton_clicked();
    void loadFriends();

private:
    // Элементы интерфейса чата
    QTextEdit *chatHistory;
    QLineEdit *inputMessage;
    QPushButton *sendButton;

    // Подключение
    QComboBox *modeComboBox;
    QLineEdit *inputIP;
    QLineEdit *inputPort;
    QPushButton *connectButton;

    QLabel *statusLabel;

    // Элементы интерфейса друзей
    QListWidget *friendsListWidget;
    QLineEdit *friendNameLineEdit;
    QPushButton *addFriendButton;

    // Логика
    QString currentUsername;
    UserManager userManager;
    ChatClient *chatClient;
};

#endif // CHATWINDOW_H
