#include "ChatClientTest.h"

void ChatClientTest::testSendReceive() {
    ChatClient sender, receiver;
    receiver.startServer(12345);
    QSignalSpy connectedSpy(&receiver, &ChatClient::connected);

    sender.connectToServer("127.0.0.1", 12345);
    QTRY_VERIFY(connectedSpy.count() > 0);

    QSignalSpy messageSpy(&receiver, &ChatClient::messageReceived);
    sender.sendMessage("Test message");
    QTRY_VERIFY(messageSpy.count() > 0);
    QCOMPARE(messageSpy[0][0].toString(), QString("Test message"));
}

QTEST_MAIN(ChatClientTest)
