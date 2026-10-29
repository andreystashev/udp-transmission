#include "udpsender.h"
#include <QRandomGenerator>
#include <QTime>
#include <QDebug>


UdpSender::UdpSender(QObject *parent)
    : QUdpSocket{parent}
{
    timer.setInterval(50);
    connect(&timer, &QTimer::timeout, this, [this] {
        const QByteArray datagram = packedStruct();
        if (writeDatagram(datagram, destination, destinationPort) < 0)
            qWarning() << "UDP send failed:" << errorString();
    });
}

bool UdpSender::newConnect(const QString &address, int port)
{
    const QHostAddress hostAddress(address);
    if (hostAddress.isNull() || port < 1 || port > 65535)
        return false;
    destination = hostAddress;
    destinationPort = static_cast<quint16>(port);
    timer.start();
    return true;
}

QByteArray UdpSender::packedStruct(){
    QTime currentTime = QTime::currentTime();
    DataStruct data{};
    static double fuel = 1000.0;
    data.hour=currentTime.hour();
    data.min=currentTime.minute();
    data.sec=currentTime.second();
    data.fuelWeight = fuel;
    fuel-=0.001;
    data.dataUnion.data.addr=QRandomGenerator::global()->bounded(0, 256);
    data.dataUnion.data.reserve1=QRandomGenerator::global()->bounded(0, 64);
    data.dataUnion.data.dataOk=QRandomGenerator::global()->bounded(0, 2);
    data.dataUnion.data.onLand=QRandomGenerator::global()->bounded(0, 2);
    data.dataUnion.data.speed=QRandomGenerator::global()->bounded(1, 5);
    data.dataUnion.data.reserve2=QRandomGenerator::global()->bounded(0, 8);
    data.dataUnion.data.matrix=QRandomGenerator::global()->bounded(0, 4);
    data.dataUnion.data.ctrlBit=QRandomGenerator::global()->bounded(0, 2);
    data.isCrashed=QRandomGenerator::global()->bounded(0, 2);

    return QByteArray(reinterpret_cast<const char*>(&data), sizeof(data));
}
