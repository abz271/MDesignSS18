#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <string>

QString InitStateID = "initState";
QString NextPointID = "nextPoint";
QString TurnToTargetAngleID = "turnToTargetAngle";
QString StartUpID = "startUp";
QString DriveStraightRegulatedID = "driveStraightRegulated";
QString StopMotorID = "stopMotor";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap gamefield("C:/MDesign/Spielfeld.png");
    ui->gamefield_label->setPixmap(gamefield);
    arduino = new QSerialPort;
    arduino_is_available = false;
    arduino_port_name = "";
    serialBuffer = "";

    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << serialPortInfo.portName();
    }
    arduino_is_available = true;
    arduino_port_name = "COM7";
    /*foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << serialPortInfo.portName();
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }*/
    if(arduino_is_available){
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readDataFromArduino()));
    }
    else{
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
    }
}

MainWindow::~MainWindow()
{
    if(arduino->isOpen()){
        arduino->close();
    }
    delete ui;
}

void MainWindow::readDataFromArduino(){
    serialData = arduino->readAll();
    serialBuffer = QString::fromStdString(serialData.toStdString());

    serialBuffer.remove("\r");
    serialBuffer.remove("\n");

    if (serialBuffer == InitStateID){
        ui->initState->setStyleSheet("QTextBrowser { background-color: rgb(0, 255, 0);}");

        ui->nextPoint->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->turnToTargetAngle->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->startUp->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->driveStraightRegulated->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->stopMotor->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
    }
    else if(serialBuffer == NextPointID){
        ui->nextPoint->setStyleSheet("QTextBrowser { background-color: rgb(0, 255, 0);}");

        ui->initState->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->turnToTargetAngle->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->startUp->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->driveStraightRegulated->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->stopMotor->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
    }
    else if(serialBuffer == TurnToTargetAngleID){
        ui->turnToTargetAngle->setStyleSheet("QTextBrowser { background-color: rgb(0, 255, 0);}");

        ui->nextPoint->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->initState->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->startUp->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->driveStraightRegulated->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->stopMotor->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
    }
    else if(serialBuffer == StartUpID){
        ui->startUp->setStyleSheet("QTextBrowser { background-color: rgb(0, 255, 0);}");

        ui->nextPoint->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->initState->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->turnToTargetAngle->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->driveStraightRegulated->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->stopMotor->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
    }
    else if(serialBuffer == DriveStraightRegulatedID){
        ui->driveStraightRegulated->setStyleSheet("QTextBrowser { background-color: rgb(0, 255, 0);}");

        ui->nextPoint->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->initState->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->turnToTargetAngle->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->startUp->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->stopMotor->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
    }
    else if(serialBuffer == StopMotorID){
        ui->stopMotor->setStyleSheet("QTextBrowser { background-color: rgb(0, 255, 0);}");

        ui->nextPoint->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->initState->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->turnToTargetAngle->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->startUp->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
        ui->driveStraightRegulated->setStyleSheet("QTextBrowser { background-color: rgb(255, 255, 255);}");
    }

    qDebug() << serialBuffer;
}
