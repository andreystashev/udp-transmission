#include "udpreceiver.h"
#include "../common/data.h"


UdpReceiver::UdpReceiver(QObject *parent)
    : QUdpSocket{parent}
{
    connect(this, &QUdpSocket::readyRead, this, &UdpReceiver::readMessage);
}

bool UdpReceiver::newConnect(int port)
{
    if (port < 1 || port > 65535)
        return false;
    close();
    return bind(QHostAddress::Any, static_cast<quint16>(port));
}


void UdpReceiver::readMessage()
{
    while (hasPendingDatagrams()) {
        if (pendingDatagramSize() != static_cast<qint64>(sizeof(DataStruct))) {
            readDatagram(nullptr, 0); // Discard the whole malformed datagram.
            continue;
        }
        QByteArray datagram(sizeof(DataStruct), '\0');
        if (readDatagram(datagram.data(), datagram.size()) != datagram.size())
            continue;
        DataStruct decoded{};
        if (unpackData(datagram.constData(), datagram.size(), decoded))
            emit receivedData(datagram);
    }
}


