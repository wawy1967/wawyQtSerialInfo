#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort;
    arduino_is_available = false;
    serialPort = QSerialPortInfo::availablePorts();
    qDebug() << "Available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {

        qDebug() << serialPortInfo.portName();
        if (serialPortInfo.hasVendorIdentifier())
            qDebug() << "Vendor ID :" << serialPortInfo.vendorIdentifier();
        //Arduino ID: 9025
        if (serialPortInfo.hasProductIdentifier())
            qDebug() << "Product ID :" << serialPortInfo.productIdentifier();
        //Uno ID: 67
    }
    static const quint16 uno_vendor_id = 9025;
    static const quint16 uno_product_id = 67;
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {

        if (serialPortInfo.hasVendorIdentifier() &&
            serialPortInfo.hasProductIdentifier())
        {
            serialPortName = serialPortInfo.portName();
            ui->textBrowser->append(serialPortName);
            if ((serialPortInfo.vendorIdentifier()==uno_vendor_id) &&
                 (serialPortInfo.productIdentifier()==uno_product_id))
            {
                arduino_is_available = true;
            }
        }
    }
    if (arduino_is_available)
    {
        serial->setPortName(serialPortName);
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setParity(QSerialPort::NoParity);
        if (!serial->open(QIODevice::ReadWrite))
            QMessageBox::information(0,"Error","Cannot open port");
    }
    else
    {
        QMessageBox::information(0,"Error","No Arduino");
    }

}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
        serial->close();
    delete ui;
}
