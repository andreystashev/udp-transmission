#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QtNetwork/QUdpSocket>


class UdpReceiver : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpReceiver(QObject *parent = nullptr);
    bool newConnect(int port);

 signals:
    void receivedData(const QByteArray &datagram);

 public slots:
    void readMessage();
};

#endif // UDPRECEIVER_H

