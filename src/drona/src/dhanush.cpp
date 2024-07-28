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

void Dhanush::moveToPosition(Bot& robot, float x, float y)
{
    float curr_x, curr_y;

    // updating the current position of the bot
    if (is_blue)
    {
        curr_x = get<0>(blue_bots[id]);
        curr_y = get<1>(blue_bots[id]);
    }
    else
    {
        curr_x = get<0>(yellow_bot_info[id]);
        curr_y = get<1>(yellow_bot_info[id]);
    }


    float kp;
    float err_x = x - curr_x;
    float err_y = y - curr_y;
    float dist_err = sqrt(pow(err_x, 2) + pow(err_y, 2));

    // Wheel and Global Velocities not yet supported, only Local Velocity supported
    //TODO: Write interpreter for wheel velocity
    command->set_id(id);
    sslsim::RobotMoveCommand *move_command = command->mutable_move_command();
    sslsim::MoveLocalVelocity *local_velocity = move_command->mutable_local_velocity();
    local_velocity->set_forward(kp * err_y);
    local_velocity->set_angular(0.0f);
    local_velocity->set_left(kp * err_x);
}
