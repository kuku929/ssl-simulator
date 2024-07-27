#ifndef DHANUSH_H
#define DHANUSH_H

#include "protobuf/ssl_simulation_robot_control.pb.h"
#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QNetworkDatagram>
#include <google/protobuf/repeated_field.h>

class Dhanush: public QObject
{
    Q_OBJECT
public:
    Dhanush();
    ~Dhanush();

public slots:
    void send_velocity();
    void handleState(QByteArray *buffer);

private:
    sslsim::RobotCommand *command;
    QUdpSocket* socket;
    QNetworkDatagram datagram;
    QByteArray buffer;
    void moveToPosition(Bot& robot, float x, float y);
};

#endif // DHANUSH_H
