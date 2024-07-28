#ifndef DHANUSH_H
#define DHANUSH_H

#include "protobuf/ssl_simulation_robot_control.pb.h"
#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QNetworkDatagram>
#include <google/protobuf/repeated_field.h>

//TODO: Struct for passing velocities together

class BotPacket: public QObject
{
    Q_OBJECT

public:
    BotPacket(){};
    ~BotPacket() = default;
    float vel_x, vel_y, vel_angular;
    int id;
    bool is_blue;
};

class Dhanush: public QObject
{
    Q_OBJECT

public:
    // struct bot_packet{
    //     float vel_x, vel_y, vel_angular;
    //     int id;
    //     bool is_blue;
    // };
    Dhanush();
    ~Dhanush();

private:
    struct Bot{
        int x, y;
        int id;
        Bot(int x, int y,int id, bool is_blue=false): x(x), y(y), is_blue(is_blue), id(id){};
        bool is_blue;
    };
    sslsim::RobotCommand *command;
    QUdpSocket* socket;
    QNetworkDatagram datagram;
    QByteArray buffer;
    void moveToPosition(float x, float y);

public slots:
    // void send_velocity(std::vector<bot_packet> packet);
    void send_velocity(BotPacket*packet);

};

#endif // DHANUSH_H
