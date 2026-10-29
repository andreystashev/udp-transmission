# UDP Transmission

A small C++/Qt demo with two applications: a console sender generates telemetry over UDP, and a desktop receiver displays the decoded fields. The sample includes time, fuel weight, a bit-field status word, and a crash flag.

## Requirements

- Qt 5.12 or newer with Core, Network, GUI and Widgets modules
- qmake, make, and a C++17 compiler
- Qt Test to run the regression tests

On Debian/Ubuntu, install the development tools with:

```sh
sudo apt install build-essential qtbase5-dev qt5-qmake
```

## Build

```sh
git clone https://github.com/andreystashev/udp-transmission.git
cd udp-transmission

mkdir build-receiver
cd build-receiver
qmake ../data-receiver/StructReceiver.pro
make
cd ..

mkdir build-sender
cd build-sender
qmake ../data-sender/dataSender.pro
make
cd ..
```

You can also open either `.pro` file in Qt Creator. The qmake project names determine the executable names: `StructReceiver` and `dataSender` (with platform-specific extensions).

## Run

1. Start `./build-receiver/StructReceiver`.
2. Enter `10100` in the port field and click **Connect**. The status bar reports whether binding succeeded.
3. Start `./build-sender/dataSender` in another terminal. It sends a generated packet to `127.0.0.1:10100` every 50 ms. Stop it with Ctrl+C.

The receiver accepts ports from 1 to 65535. Clicking **Connect** again rebinds the same receiver; an unsuccessful bind leaves it disconnected. Field labels in the desktop interface remain in Russian.

To change the sender's destination, edit the `newConnect("127.0.0.1", 10100)` call in `data-sender/main.cpp` and rebuild. It expects a numeric IP address. The receiver listens on all local interfaces, so packets from other machines may reach it when allowed by the firewall.

## Packet format and limitations

Both applications share `common/data.h` and send one packed `DataStruct` per datagram. On the tested Linux/GCC build, this is 25 bytes: three native integers, a double, a status word, and a boolean. The union is transmitted once, and the receiver rejects incorrect packet sizes and invalid representations of the trailing boolean.

This is a native-memory format for a learning demo. Peers need compatible compiler/ABI, integer sizes, byte order, floating-point and bit-field layouts. It is not a portable wire protocol. The corrected sender and receiver should be updated together; the original sender accidentally appended the union twice. For communication across platforms, define a versioned format with explicit field widths and byte order.

UDP delivery and ordering are not guaranteed. The demo has no authentication, encryption, retransmission, or duplicate detection. It displays generated values and does not connect to real telemetry hardware.

## Tests

```sh
mkdir build-tests
cd build-tests
qmake ../tests/tests.pro
make
./udp-tests
```

The Qt Test suite checks the packet format, malformed inputs, loopback packet bursts, reconnecting the receiver, port validation, and timer-driven sending without blocking the event loop. It does not require a display server.

## Screenshot

<img width="1440" alt="Telemetry fields in the Qt receiver" src="https://github.com/andreystashev/StructReceiver/assets/70208784/276c2857-df54-4c6c-b1b1-397e290c14ef">
