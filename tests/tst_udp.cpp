#include <QtTest>
#include "../data-sender/udpsender.h"
#include "../data-receiver/udpreceiver.h"

class UdpTest : public QObject
{
    Q_OBJECT

private slots:
    void packetRoundTrip()
    {
        UdpSender sender;
        const QByteArray packet = sender.packedStruct();
        QCOMPARE(packet.size(), static_cast<int>(sizeof(DataStruct)));
        DataStruct decoded{};
        QVERIFY(unpackData(packet.constData(), packet.size(), decoded));
        QVERIFY(decoded.hour >= 0 && decoded.hour < 24);
        QVERIFY(decoded.min >= 0 && decoded.min < 60);
        QVERIFY(decoded.sec >= 0 && decoded.sec < 60);

        // The trailing flag must survive serialization after the union word.
        DataStruct original{};
        original.dataUnion.word = 0x12345678;
        original.isCrashed = true;
        QVERIFY(unpackData(reinterpret_cast<const char *>(&original), sizeof(original), decoded));
        QCOMPARE(decoded.dataUnion.word, original.dataUnion.word);
        QVERIFY(decoded.isCrashed);
    }

    void malformedPackets()
    {
        UdpSender sender;
        QByteArray packet = sender.packedStruct();
        DataStruct decoded{};
        QVERIFY(!unpackData(packet.constData(), packet.size() - 1, decoded));
        packet[static_cast<int>(offsetof(DataStruct, isCrashed))] = '\x7f';
        QVERIFY(!unpackData(packet.constData(), packet.size(), decoded));
    }

    void receiveBurstAndReconnect()
    {
        UdpReceiver receiver;
        QVERIFY(receiver.bind(QHostAddress(QHostAddress::LocalHost), 0));
        const quint16 port = receiver.localPort();
        QSignalSpy received(&receiver, &UdpReceiver::receivedData);
        UdpSender sender;
        const QByteArray packet = sender.packedStruct();
        QCOMPARE(sender.writeDatagram("bad", 3, QHostAddress::LocalHost, port), qint64(3));
        QByteArray invalid = packet;
        invalid[static_cast<int>(offsetof(DataStruct, isCrashed))] = '\x7f';
        QCOMPARE(sender.writeDatagram(invalid, QHostAddress::LocalHost, port), qint64(invalid.size()));
        const QByteArray oversized = packet + 'x';
        QCOMPARE(sender.writeDatagram(oversized, QHostAddress::LocalHost, port), qint64(oversized.size()));
        for (int i = 0; i < 3; ++i)
            QCOMPARE(sender.writeDatagram(packet, QHostAddress::LocalHost, port), qint64(packet.size()));
        QTRY_COMPARE(received.count(), 3);
        QCOMPARE(received.at(0).at(0).toByteArray(), packet);

        QVERIFY(receiver.newConnect(port));
        QCOMPARE(sender.writeDatagram(packet, QHostAddress::LocalHost, port), qint64(packet.size()));
        QTRY_COMPARE(received.count(), 4);
        QVERIFY(!receiver.newConnect(-1));
        QVERIFY(!receiver.newConnect(65536));
    }

    void bindFailure()
    {
        QUdpSocket occupied;
        QVERIFY(occupied.bind(QHostAddress(QHostAddress::Any), 0, QUdpSocket::DontShareAddress));
        UdpReceiver receiver;
        QVERIFY(!receiver.newConnect(occupied.localPort()));
    }

    void senderUsesEventLoop()
    {
        QUdpSocket receiver;
        QVERIFY(receiver.bind(QHostAddress(QHostAddress::LocalHost), 0));
        UdpSender sender;
        QVERIFY(!sender.newConnect("invalid", receiver.localPort()));
        QVERIFY(sender.newConnect("127.0.0.1", receiver.localPort()));
        QTRY_VERIFY(receiver.hasPendingDatagrams());
        QCOMPARE(receiver.pendingDatagramSize(), qint64(sizeof(DataStruct)));
    }
};

QTEST_GUILESS_MAIN(UdpTest)
#include "tst_udp.moc"
