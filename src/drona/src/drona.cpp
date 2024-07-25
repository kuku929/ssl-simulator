#include "drona.h"
#include "core/sslprotocols.h"
#include "protobuf/ssl_wrapper.pb.h"
#include "protobuf/sslsim.h"
#include <QString>
#include <QNetworkDatagram>

using namespace sslsim;

Drona::Drona(QObject* parent) : QObject(parent), socket(new QUdpSocket(this)){
    // create a QUDP socket

}

Drona::~Drona(){
    delete socket;
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
    command = robot_control.add_robot_commands();
    moveToPosition(0, 1.0f, 1.0f);

    //TODO: create the robot command and add RobotMoveCommand with wheel velocities

    //TODO: Add function to move the bot

    QByteArray dgram;
    dgram.resize(robot_control.ByteSize());
    robot_control.SerializeToArray(dgram.data(), dgram.size());
    if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_BLUE_PORT) > -1) {
        qDebug("[drona] : sent data");
    }
}


void Drona::moveToPosition(int id, float x, float y) {
    // Wheel and Global Velocities not yet supported, only Local Velocity supported
    //TODO: Write interpreter for wheel velocity
    command->set_id(id);
    sslsim::RobotMoveCommand *move_command = command->mutable_move_command();
    sslsim::MoveLocalVelocity *local_velocity = move_command->mutable_local_velocity();
    local_velocity->set_forward(-10.0f);
    local_velocity->set_angular(0.0f);
    local_velocity->set_left(0.0f);
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
    if (socket->writeDatagram(dgram, this->_addr_sim, this->_port_sim) > -1) {
        qDebug("send data");
    }
}
