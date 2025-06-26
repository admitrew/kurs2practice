#pragma once

#include <QtTest>
#include "ChatClient.h"

class ChatClientTest : public QObject {
    Q_OBJECT

private slots:
    void testSendReceive();
};
