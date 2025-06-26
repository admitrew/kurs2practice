#include <QApplication>
#include "ChatWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ChatWindow window;
    window.resize(500, 400);
    window.show();

    return app.exec();
}
