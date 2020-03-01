#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QXmlStreamReader>
#include <QSettings>
#include <udpserver.h>

#define STATION_COUNT 4

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    UDPServer *logServer;
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    QString displayName[STATION_COUNT];
    QString networkName[STATION_COUNT];
    QString PTTStatus[STATION_COUNT];
    QString logSoftware;
    quint16 networkPort;
    QString localIP;
private slots:
    void radioDataAvailable(char *data, qint64 size, QHostAddress *fromAddr, quint16 *port);
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H
