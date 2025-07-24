#include "VideoCallWindow.h"
#include <QFile>
#include <QUrl>

VideoCallWindow::VideoCallWindow(QWidget *parent)
    : QMainWindow(parent), view(new QWebEngineView(this)) {

    setWindowTitle("Video Call");
    resize(900, 600);

    view->load(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/webrtc.html"));
    setCentralWidget(view);
}
