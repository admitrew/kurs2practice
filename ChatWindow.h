#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

class QTextEdit;
class QLineEdit;
class QPushButton;
class QLabel;
class QComboBox;
class ChatClient;

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);

private slots:
    void onSendClicked();
    void onConnectClicked();
    void onModeChanged(int index);

    void onMessageReceived(const QString &message);
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(const QString &error);

private:
    QTextEdit *chatHistory;
    QLineEdit *inputMessage;
    QPushButton *sendButton;

    QComboBox *modeComboBox;    // Режим: Клиент / Сервер
    QLineEdit *inputIP;         // IP-адрес (только для клиента)
    QLineEdit *inputPort;       // Порт
    QPushButton *connectButton;

    QLabel *statusLabel;

    ChatClient *chatClient;
};

#endif // CHATWINDOW_H
