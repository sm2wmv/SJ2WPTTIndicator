#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

class UDPServer : public QObject
{
    Q_OBJECT
public:
    explicit UDPServer(QObject *parent = nullptr);
    void initSocket(int port);

private:
    QUdpSocket *udpSocket;
signals:
    void dataAvailable(char *data, qint64 size, QHostAddress *fromAddr, quint16 *port);
public slots:
    void readPendingDatagrams();
};

#endif // UDPSERVER_H
