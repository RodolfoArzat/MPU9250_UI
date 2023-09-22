#include "mainwindow.h"
#include "ui_mainwindow.h"


bool centigrade=false;
bool farenheit=false;
bool tempmodepb=false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->IMU6500GB->resize(770,271);
    ui->AKGB->resize(370,271);

    //--

    ui->TempModePB->setVisible(false);

    StopUI();

    ui->lcd_AccelerometerX->display("-----");
    ui->lcd_AccelerometerY->display("-----");
    ui->lcd_AccelerometerZ->display("-----");
    ui->lcd_GyroscopeX->display("-----");
    ui->lcd_GyroscopeY->display("-----");
    ui->lcd_GyroscopeZ->display("-----");
    ui->lcd_MagnetometerX->display("-----");
    ui->lcd_MagnetometerY->display("-----");
    ui->lcd_MagnetometerZ->display("-----");

    serialBuffer="";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StopUI()
{
    ui->PlotAccelerometerPB->setEnabled(false);
    ui->PlotGyroscopePB->setEnabled(false);
    ui->PlotMagnometerPB->setEnabled(false);
    ui->TempModePB->setEnabled(false);
    //--
    ui->BaudCB->setEnabled(false);
    ui->AccelerometerCB->setEnabled(false);
    ui->GyroscopeCB->setEnabled(false);
    ui->DelayCB->setEnabled(false);
    //---
}

void MainWindow::PlayUI()
{
    ui->PlotAccelerometerPB->setEnabled(true);
    ui->PlotGyroscopePB->setEnabled(true);
    ui->PlotMagnometerPB->setEnabled(true);
    ui->TempModePB->setEnabled(true);
    //--
    ui->BaudCB->setEnabled(true);
    ui->AccelerometerCB->setEnabled(true);
    ui->GyroscopeCB->setEnabled(true);
    ui->DelayCB->setEnabled(true);
    //---
}

void MainWindow::on_ConnectArduinoPB_clicked()
{
    serial = new QSerialPort(this); //Inicializa la variable Serial
    arduino_available = false;

    foreach (const QSerialPortInfo &serial_Info, QSerialPortInfo::availablePorts()) {//Lee la información de cada puerto serial
        //qDebug()<<"Puerto: "<<serial_Info.portName();
        portname = serial_Info.portName();
        //qDebug()<<"Vendor Id: "<<serial_Info.vendorIdentifier();
        vendorId = serial_Info.vendorIdentifier();
        //qDebug()<<"Product Id: "<<serial_Info.productIdentifier();
        productId = serial_Info.productIdentifier();
        arduino_available = true;
    }
    if(arduino_available){
        ui->TempModePB->setVisible(true);
        PlayUI();
        centigrade=true;
        ui->label_TemperatureMode->setText("°c");

        ui->label_arduinostatus->setText("Arduino in "+portname);
        ui->ConnectArduinoPB->setText("Connected Succesfully");
        ui->ConnectArduinoPB->setEnabled(false);
        arduino_init();
    }
}

void MainWindow::on_TempModePB_clicked()
{
    if(tempmodepb){ // change to centigrade
        ui->TempModePB->setText("Change to Farenheit");
        ui->label_TemperatureMode->setText("°c");
            centigrade=true;
        farenheit=false;
        tempmodepb=false;
    }else{ //change to farenheit
        ui->TempModePB->setText("Change to Centigrade");
        ui->label_TemperatureMode->setText("°f");
            centigrade=false;
        farenheit=true;
        tempmodepb=true;
    }
}

void MainWindow::UpdateGyro(const QString string1, const QString string2, const QString string3)
{
    ui->lcd_GyroscopeX->display(string1);
    ui->lcd_GyroscopeY->display(string2);
    ui->lcd_GyroscopeZ->display(string3);
}

void MainWindow::UpdateAccelerometer(const QString string1, const QString string2, const QString string3)
{
    ui->lcd_AccelerometerX->display(string1);
    ui->lcd_AccelerometerY->display(string2);
    ui->lcd_AccelerometerZ->display(string3);
}

void MainWindow::UpdateMagnometer(const QString string1, const QString string2, const QString string3)
{
    ui->lcd_MagnetometerX->display(string1);
    ui->lcd_MagnetometerY->display(string2);
    ui->lcd_MagnetometerZ->display(string3);
}

void MainWindow::UpdateTemp(const QString string1)
{
    if(centigrade){

    }else if(farenheit){

    }
}

void MainWindow::arduino_init()
{
    serial->setPortName(portname);
    switch (ui->BaudCB->currentIndex()) {
    case 0: //9600
        serial->setBaudRate(QSerialPort::Baud9600);
        break;
    case 1: //19200
        serial->setBaudRate(QSerialPort::Baud19200);
        break;
    case 2://38400
        serial->setBaudRate(QSerialPort::Baud38400);
        break;
    case 3: //57600 best setting for sensor
        serial->setBaudRate(QSerialPort::Baud57600);
        break;
    case 4: //115200
        serial->setBaudRate(QSerialPort::Baud115200);
        break;
    }
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);
    connect(serial, SIGNAL(readyRead()),this,SLOT(Serial_Read()));
}

void MainWindow::Serial_Read()
{
    //qDebug() << serialBuffer;
    if(serial->isOpen()){
        serialData = serial->readAll();
        if(serialData.at(0)!=char(42)){
            QStringList bufferSplit = serialBuffer.split(",");
            if(bufferSplit.length()<4){
                serialBuffer += QString::fromStdString(serialData.toStdString());
            }else{
                //qDebug() << bufferSplit;
                UpdateAccelerometer(bufferSplit[0],bufferSplit[1],bufferSplit[2]);
                UpdateGyro(bufferSplit[3],bufferSplit[4],bufferSplit[5]);
                UpdateMagnometer(bufferSplit[6],bufferSplit[7],bufferSplit[8]);
                UpdateTemp(bufferSplit[9]);
                serialBuffer="";
            }
        }else{
            serial->flush();
        }
    }


}

void MainWindow::on_GyroscopeCB_currentTextChanged(const QString &arg1)
{
    serial->write(("*Gyroscope," +arg1).toLatin1());
    serial->flush();
}

void MainWindow::on_AccelerometerCB_currentTextChanged(const QString &arg1)
{
    serial->write(("*Accelerometer," +arg1).toLatin1());
    serial->flush();
}

