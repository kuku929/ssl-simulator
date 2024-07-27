#ifndef DHANUSH_H
#define DHANUSH_H

#include "protobuf/ssl_simulation_robot_control.pb.h"
#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QNetworkDatagram>

class Dhanush: public QObject
{
    Q_OBJECT
public:
    Dhanush();
    ~Dhanush();

public slots:
    void send_velocity();

private:
    sslsim::RobotCommand *command;
    QUdpSocket* socket;
    QNetworkDatagram datagram;
    QByteArray buffer;
    void moveToPosition(int id, float x, float y);
};

#endif // DHANUSH_H
