#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include "dataStructs.h"
#include <QDebug>
#include <QStatusBar>


const int structSize = sizeof(nameMassive)/sizeof(nameMassive[0]);

QLabel *nameCells[structSize];
QLabel *valueCells[structSize];


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createTable(700,"black","white");
    receiver = new UdpReceiver(this);
    connect(receiver, &UdpReceiver::receivedData, this, &MainWindow::fillTable);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createTable(int width,QString backgroundColor,QString color)
{
    static int chartScopeCounter = 0;

    for(int i=0;i<structSize;++i)
    {
        nameCells[i] = new QLabel(ui->structChart);
        nameCells[i]->setText(nameMassive[i]);
        nameCells[i]->setGeometry(QRect(0,0,width,36));
        nameCells[i]->move(0,chartScopeCounter);
        nameCells[i]->setStyleSheet("QLabel{font-size:16px;font:bold; border: 1px solid "+backgroundColor+"; color: "+backgroundColor+"}\
                                                       QLabel::hover{background-color: "+backgroundColor+"; color: "+color+"}");

        valueCells[i] = new QLabel(ui->structChart);
        valueCells[i]->setNum(0);
        valueCells[i]->setGeometry(QRect(0,0,250,36));
        valueCells[i]->move(width-1,chartScopeCounter);
        valueCells[i]->setStyleSheet("QLabel{font-size:16px;font:bold; border: 1px solid "+backgroundColor+"; color: "+backgroundColor+"}\
                                    QLabel::hover{background-color: "+backgroundColor+"; color: "+color+"}");

        chartScopeCounter+=35;
    }
    ui->structChart->resize(width+250,chartScopeCounter+1);

    ui->datagramCounter->resize(250,36);
    ui->datagramCounter->move(width+9,chartScopeCounter+10);
    ui->datagramCounter->setStyleSheet("QLabel{font-size:16px;font:bold; border: 1px solid "+backgroundColor+"; color: "+backgroundColor+"}\
                                    QLabel::hover{background-color: "+backgroundColor+"; color: "+color+"}");

    ui->portPlace->move(10,chartScopeCounter+10);
    ui->portPlace->resize(150,36);
    ui->portPlace->setStyleSheet("QLabel{font-size:16px;font:bold; border: 1px solid "+backgroundColor+"; color: "+backgroundColor+"}\
                                    QLabel::hover{background-color: "+backgroundColor+"; color: "+color+"}");

    ui->connectButton->move(160,chartScopeCounter+10);
    ui->connectButton->setStyleSheet("QPushButton{font-size:16px;font:bold; border: 1px solid "+backgroundColor+"; color: "+backgroundColor+"}\
                                    QPushButton::hover{background-color: "+backgroundColor+"; color: "+color+"}");
    ui->connectButton->resize(150,36);

}


int trueSpeed(unsigned int speed){
    int summ = 0;
    int MSb = 512;
    while(speed){
        if(speed & 1){
            summ+=MSb;
        }
        MSb/=2;
        speed >>= 1;
    }
    return summ;
}


void MainWindow::fillTable(const QByteArray &data)
{
    DataStruct dataUnpack{};
    if (!unpackData(data.constData(), data.size(), dataUnpack))
        return;
    const DataStruct *pDataUnpack = &dataUnpack;

    valueCells[0]->setNum(pDataUnpack->hour);
    valueCells[1]->setNum(pDataUnpack->min);
    valueCells[2]->setNum(pDataUnpack->sec);
    valueCells[3]->setNum(pDataUnpack->fuelWeight);
    valueCells[4]->setText(QString::number(pDataUnpack->dataUnion.word,16));
    valueCells[5]->setText(QString::number(pDataUnpack->dataUnion.data.addr,16));
    valueCells[6]->setNum(pDataUnpack->dataUnion.data.reserve1);
    valueCells[7]->setNum(pDataUnpack->dataUnion.data.dataOk);
    valueCells[8]->setNum(pDataUnpack->dataUnion.data.onLand);
    valueCells[9]->setNum(trueSpeed(pDataUnpack->dataUnion.data.speed));
    valueCells[10]->setNum(pDataUnpack->dataUnion.data.reserve2);
    valueCells[11]->setText(QString::number(pDataUnpack->dataUnion.data.matrix,2));
    valueCells[12]->setNum(pDataUnpack->dataUnion.data.ctrlBit);
    valueCells[13]->setNum(pDataUnpack->isCrashed);

    static int datagramCounter = 1;
    ui->datagramCounter->setNum(datagramCounter);
    ++datagramCounter;
}

void MainWindow::on_connectButton_clicked()
{
    bool valid = false;
    const int port = ui->portPlace->toPlainText().trimmed().toInt(&valid);
    if (!valid || port < 1 || port > 65535) {
        statusBar()->showMessage(tr("Enter a port from 1 to 65535."));
        return;
    }
    if (!receiver->newConnect(port)) {
        statusBar()->showMessage(tr("Cannot bind UDP port: %1").arg(receiver->errorString()));
        return;
    }
    statusBar()->showMessage(tr("Listening on UDP port %1").arg(port));
}

