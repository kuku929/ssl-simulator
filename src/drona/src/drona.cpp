#include "drona.h"
#include "core/sslprotocols.h"
#include "protobuf/ssl_wrapper.pb.h"
#include "protobuf/sslsim.h"
#include <QString>
#include <QNetworkDatagram>

using namespace sslsim;

Drona::Drona(QObject* parent) : QObject(parent), socket(new QUdpSocket(this)){
    // create a QUDP socket

    this->_addr.setAddress(SSL_VISION_ADDRESS);
    this->_port = quint16(SSL_SIMULATED_VISION_PORT);

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    //the problem is with qudpsocket since the slot is being called fine
    socket->bind(QHostAddress::AnyIPv4, 10020, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(socket->state() != QAbstractSocket::BoundState){
        qDebug() << "socket not bound";
    }

    // new syntax, do not use SIGNAL() and SLOT()
    // auto success = connect(socket, &QUdpSocket::readyRead);
    // if(!success){
    //     qDebug() << socket->errorString();
    // }
    // qDebug() << socket->bytesAvailable();
}

Drona::~Drona(){
    delete socket;
    delete command;
}

void Drona::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"socket error occured and the error is : "<<socketError;
}

void Drona::setPortAndAddress(int port, const QString& address) {
    this->_port_sim = quint16(port);
    this->_addr_sim.setAddress(address);
}

void Drona::run() {
    RobotControl robot_control;
    RobotCommand *command = robot_control.add_robot_commands();
    moveToPosition(command->mutable_move_command(), 0, 1, 1);
    //sendCommand(2.0f, 0);

    //TODO: create the robot command and add RobotMoveCommand with wheel velocities

    //TODO: Add function to move the bot

    QByteArray dgram;
    dgram.resize(robot_control.ByteSize());
    robot_control.SerializeToArray(dgram.data(), dgram.size());
    if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_PORT) > -1) {
        qDebug("[drona] : sent data");
    }
}


void Drona::moveToPosition(sslsim::RobotMoveCommand *move_command, int id, float x, float y) {
    command->set_id(id);
    // sslsim::RobotMoveCommand *move_command = command->mutable_move_command();
    sslsim::MoveWheelVelocity *wheel_velocity = move_command->mutable_wheel_velocity();
    wheel_velocity->set_front_right(1.0f);
    wheel_velocity->set_front_left(-1.0f);
    wheel_velocity->set_back_left(-1.0f);
    wheel_velocity->set_back_right(1.0f);
}

void Drona::sendCommand(float velX, int id) {
    double zero = 0.0;
    RobotControl packet;
    bool yellow = false;
    RobotCommand* command = packet.add_robot_commands();
    command->set_id(id);
    RobotMoveCommand *move_command = command->mutable_move_command();
    MoveLocalVelocity *vel_command = move_command->mutable_local_velocity();

    vel_command->set_forward(velX);
    vel_command->set_left(0.0f);
    vel_command->set_angular(0.0f);

    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    // todo : this will probably not work since socket in bounded, use write()
    if (socket->writeDatagram(dgram, this->_addr_sim, this->_port_sim) > -1) {
        qDebug("send data");
    }
}
