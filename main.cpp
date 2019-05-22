#include <QCoreApplication>
#include <QMap>
#include "serialportserver.h"
#include <QDebug>

void printHelp()
{
    qDebug()<<"Supported Params:\n\n"
              "  --help                      Print help information\n"
              "  --serial=SerialPortName     Set serialport name\n"
              "  --baudrate=BaudRate         Set serialport baudrate (4800/9600/115200)\n"
              "  --port=TcpPort              Set tcp port (1000~65535)\n";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if(argc==2 && (QString(argv[1]).trimmed()=="--help"))
    {
        printHelp();
        return 0;
    }
    QMap<QString, QString> paramMap;
    for(int i=1; i<argc; i++)
    {
        QString param=QString(argv[i]).trimmed();
        if(param.startsWith("--") && param.contains("="))
        {
            paramMap.insert(param.section("=", 0, 0), param.section("=", 1, 1));
        }
    }
    QString serial=paramMap.value("--serial");
    if(serial.isEmpty())
    {
        printHelp();
        return 0;
    }
    int baudrate=paramMap.value("--baudrate").toInt();
    if((baudrate!=4800)&&(baudrate!=9600)&&(baudrate!=115200))
    {
        printHelp();
        return 0;
    }
    int port=paramMap.value("--port").toInt();
    if((port<1000) || (port>65535))
    {
        printHelp();
        return 0;
    }
    qDebug()<<"Serial Port="<<serial<<"\nBaudRate="<<baudrate<<"\nTcpPort="<<port;
    SerialportServer *server=new SerialportServer(serial, baudrate, port);
    if(server->start())
    {
        qDebug()<<"SerialportServer start success!";
    }
    else
    {
        qDebug()<<"SerialportServer start failed!";
    }
    return a.exec();
}
