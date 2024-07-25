#ifndef DRONA_H
#define DRONA_H

#include <QObject>
#include <QUdpSocket>
#include <QString>
#include "protobuf/ssl_simulation_robot_control.pb.h"
#include <QNetworkDatagram>

class Drona : public QObject
{
	Q_OBJECT
public:
    explicit Drona(QObject *parent = 0);
    void setPortAndAddress(int port, const QString& address);
    void sendCommand(float velX, int id);
    void moveToPosition(int id, float x, float y);
    ~Drona();
    
public slots:
    void onSocketError(QAbstractSocket::SocketError socketError);
    void run();

private:
    sslsim::RobotCommand *command;
    quint16 _port;
    quint16 _port_sim;
    QHostAddress _addr_sim;
	QHostAddress _addr;
    QUdpSocket* socket;
    QNetworkDatagram datagram;
    QByteArray buffer;
    
signals:
	// void configureBots();
};

#endif // DRONA_H
