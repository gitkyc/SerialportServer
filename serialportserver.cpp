#include "serialportserver.h"

SerialportServer::SerialportServer(const QString &serialPortName, int baudRate, int tcpPort, QObject *parent) : QObject(parent)
{
    this->m_serialPortName=serialPortName;
    this->m_baudRate=baudRate;
    this->m_tcpPort=tcpPort;
    m_serialPort=NULL;
    m_tcpServer=NULL;
    m_currentTcpSocket=NULL;
}

bool SerialportServer::start()
{
    m_serialPort=new QSerialPort(this);
    m_serialPort->setPortName(m_serialPortName);
    m_serialPort->setBaudRate(m_baudRate);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    bool serialPortOk=m_serialPort->open(QSerialPort::ReadWrite);
    if(serialPortOk)
    {
        connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(doSerialPortReadyRead()));
    }
    m_tcpServer=new QTcpServer(this);
    bool tcpServerOk=m_tcpServer->listen(QHostAddress::Any, m_tcpPort);
    if(tcpServerOk)
    {
        connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(doTcpServerNewConnection()));
    }
    return (serialPortOk && tcpServerOk);
}

void SerialportServer::doSerialPortReadyRead()
{
    QByteArray data=m_serialPort->readAll();
    if(m_currentTcpSocket==NULL)
    {
        return;
    }
    m_currentTcpSocket->write(data);
}

void SerialportServer::doTcpSocketReadyRead()
{
    QByteArray data=m_currentTcpSocket->readAll();
    m_serialPort->write(data);
}

void SerialportServer::doTcpServerNewConnection()
{
    if(m_currentTcpSocket!=NULL)
    {
        m_currentTcpSocket->close();
        m_currentTcpSocket->deleteLater();
        m_currentTcpSocket=NULL;
    }
    m_currentTcpSocket=m_tcpServer->nextPendingConnection();
    qDebug()<<"New Connection From '"<<m_currentTcpSocket->peerAddress()<<"'";
    connect(m_currentTcpSocket, SIGNAL(readyRead()), this, SLOT(doTcpSocketReadyRead()));
}
