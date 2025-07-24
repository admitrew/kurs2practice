#ifndef VIDEOCALLWINDOW_H
#define VIDEOCALLWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>

class VideoCallWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit VideoCallWindow(QWidget *parent = nullptr);

private:
    QWebEngineView *view;
};

#endif // VIDEOCALLWINDOW_H
