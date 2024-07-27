#include "dhanush.h"
#include "core/sslprotocols.h"
#include <math.h>
#define LOG qDebug() << "[dhanush] : "
using namespace sslsim;

Dhanush::Dhanush():
    socket(new QUdpSocket(this))
{

}

Dhanush::~Dhanush(){
    delete socket;
}

void Dhanush::send_velocity(std::vector<bot_packet> packets)
{
    RobotControl robot_control;
    command = robot_control.add_robot_commands();
    command->set_id(packets[0].id);
    //moveToPosition(0, 1.0f, 1.0f);

    //TODO: create the robot command and add RobotMoveCommand with wheel velocities

    //TODO: Add function to move the bot

    RobotMoveCommand *move_command = command->mutable_move_command();
    MoveLocalVelocity *local_vel = move_command->mutable_local_velocity();
    local_vel->set_forward(packets[0].vel_y);
    local_vel->set_left(packets[0].vel_x);
    local_vel->set_angular(0.0f);

    QByteArray dgram;
    dgram.resize(robot_control.ByteSize());
    robot_control.SerializeToArray(dgram.data(), dgram.size());
    if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_BLUE_PORT) > -1) {
        LOG << "sent data: " << packets[0].vel_y;
    }
}
