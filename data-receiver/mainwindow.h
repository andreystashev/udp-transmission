#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "udpreceiver.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createTable(int width, QString backgroundColor, QString color);

public slots:
    void fillTable(const QByteArray &data);

private slots:
    void on_connectButton_clicked();

private:
    Ui::MainWindow *ui;
    UdpReceiver *receiver;

};
#endif // MAINWINDOW_H
