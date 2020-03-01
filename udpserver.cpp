#include "udpserver.h"
#include <QXmlStreamReader>
#include <QNetworkInterface>

UDPServer::UDPServer(QObject *parent) : QObject(parent)
{

}

void UDPServer::initSocket(QString localIP, quint16 port)
{
    udpSocket = new QUdpSocket(this);

    udpSocket->bind(QHostAddress(localIP), port);

    connect(udpSocket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
}

void UDPServer::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        emit dataAvailable(datagram.data(), datagram.size(), &sender, &senderPort);
    }
}
