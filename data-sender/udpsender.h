#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QtNetwork/QUdpSocket>
#include <QTimer>
#include "../common/data.h"

class UdpSender : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpSender(QObject *parent = nullptr);
    bool newConnect(const QString &address, int port);
    QByteArray packedStruct();

private:
    QTimer timer;
    QHostAddress destination;
    quint16 destinationPort = 0;
};

#endif // UDPSENDER_H
