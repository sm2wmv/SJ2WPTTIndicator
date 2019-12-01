#include "udpserver.h"
#include <QXmlStreamReader>
#include <QNetworkInterface>

UDPServer::UDPServer(QObject *parent) : QObject(parent)
{

}

void UDPServer::initSocket(int port)
{
    udpSocket = new QUdpSocket(this);
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);

    QString addr = "127.0.0.1";

    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
            addr = address.toString();
    }

    udpSocket->bind(QHostAddress(addr), port);


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
