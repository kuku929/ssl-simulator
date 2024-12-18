#include "dhanush.h"
#include "core/sslprotocols.h"
#define LOG qDebug() << "[dhanush] : "
// this method is very bad, need to think of a better way to get total bots and the format of the game,
// probably some message from the simulator
#define TOTAL_BOTS 10
using namespace sslsim;

Dhanush::Dhanush():
    socket(new QUdpSocket(this))
{

}


Dhanush::~Dhanush(){
    delete socket;
}


void Dhanush::send_velocity(BotPacket* packet)
{
    // this can lead to race conditions since memory is shared between two threads.
    // cannot provide local copy since QObjects are non-copyable
    // In practice, it is rare since the signal is emmitted at 100 Hz and this function should
    // finish much faster
    // This is so dumb, a thread should be launched for performing heavy tasks. This light ahh
    // function should not need a separate thread!!
    // But, this is not entirely useless. In the future, we can spawn threads for heavy computations
    // like high level strategy which would run at 10 or 25 or 50 Hz
    RobotControl robot_control;
    //preparing packet
    for(int i=0;i < TOTAL_BOTS/2; ++i){
        command = robot_control.add_robot_commands();
        command->set_id(packet[i].id);
        command->set_kick_speed(packet[i].kick_speed);
        RobotMoveCommand *move_command = command->mutable_move_command();
        MoveLocalVelocity *local_vel = move_command->mutable_local_velocity();
        local_vel->set_forward(packet[i].vel_x);
        local_vel->set_left(packet[i].vel_y);
        local_vel->set_angular(packet[i].vel_angular);

    }

    // sending packets
    QByteArray dgram;
    dgram.resize(robot_control.ByteSize());
    robot_control.SerializeToArray(dgram.data(), dgram.size());
    if(packet->is_blue){
        if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_BLUE_PORT) > -1) {
            // for logging purposes
        }
    }else{
        if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_YELLOW_PORT) > -1) {
            // for logging purposes
        }
    }
}
