#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void StopUI();

    void PlayUI();

    void Serial_Read();

    void UpdateGyro(const QString string1, const QString string2, const QString string3);

    void UpdateAccelerometer(const QString string1, const QString string2, const QString string3);

    void UpdateMagnometer(const QString string1, const QString string2, const QString string3);

    void UpdateTemp(const QString string1);

private slots:
    void on_ConnectArduinoPB_clicked();

    void on_TempModePB_clicked();

    void on_GyroscopeCB_currentTextChanged(const QString &arg1);

    void on_AccelerometerCB_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QString portname;
    quint16 vendorId;
    quint16 productId;
    QByteArray serialData;
    QString serialBuffer;

    bool DataReceived = false;
    bool arduino_available;

    void arduino_init();

};
#endif // MAINWINDOW_H
