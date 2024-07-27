#include "dhanush.h"
#include "core/sslprotocols.h"
#define LOG qDebug() << "[dhanush] : "
using namespace sslsim;

Dhanush::Dhanush():
    socket(new QUdpSocket(this))
{

}

Dhanush::~Dhanush(){
    delete socket;
}

void Dhanush::send_velocity()
{
    RobotControl robot_control;
    command = robot_control.add_robot_commands();
    moveToPosition(0, 1.0f, 1.0f);

    //TODO: create the robot command and add RobotMoveCommand with wheel velocities

    //TODO: Add function to move the bot

    QByteArray dgram;
    dgram.resize(robot_control.ByteSize());
    robot_control.SerializeToArray(dgram.data(), dgram.size());
    if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_BLUE_PORT) > -1) {
        // LOG << "sent data";
    }
}

void Dhanush::moveToPosition(int id, float x, float y)
{
    // Wheel and Global Velocities not yet supported, only Local Velocity supported
    //TODO: Write interpreter for wheel velocity
    command->set_id(id);
    sslsim::RobotMoveCommand *move_command = command->mutable_move_command();
    sslsim::MoveLocalVelocity *local_velocity = move_command->mutable_local_velocity();
    local_velocity->set_forward(-10.0f);
    local_velocity->set_angular(0.0f);
    local_velocity->set_left(0.0f);
}
