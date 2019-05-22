#ifndef SERIALPORTSERVER_H
#define SERIALPORTSERVER_H

#include <QObject>
#include <QSerialPort>
#include <QTcpServer>
#include <QTcpSocket>

class SerialportServer : public QObject
{
    Q_OBJECT
public:
    explicit SerialportServer(const QString &serialPortName, int baudRate, int tcpPort, QObject *parent = 0);
    bool start();

signals:

private slots:
    void doSerialPortReadyRead();
    void doTcpSocketReadyRead();
    void doTcpServerNewConnection();

private:
    QString m_serialPortName;
    int m_baudRate;
    int m_tcpPort;
    QSerialPort *m_serialPort;
    QTcpServer *m_tcpServer;
    QTcpSocket *m_currentTcpSocket;
};

#endif // SERIALPORTSERVER_H
