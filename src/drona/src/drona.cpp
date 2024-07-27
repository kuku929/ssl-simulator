#include "drona.h"
#include <QString>
#include <QNetworkDatagram>
#include <math.h>
#define LOG qDebug() << "[drona] : "


using namespace sslsim;
Drona::Drona(QObject* parent) : QObject(parent),
    sender(new Dhanush())
{
    sender->moveToThread(&sender_thread);
    // connect(this, &Drona::send, sender, &Dhanush::send_velocity);
    sender_thread.setObjectName("sender");
    sender_thread.start();
    // allocate the sender to a separate thread

}

void Drona::moveToPosition(float x, float y)
{
    float curr_x, curr_y;

    // updating the current position of the bot
    if (1==1)
    {
        curr_x = std::get<0>(blue_bot_info[0]);
        curr_y = std::get<1>(blue_bot_info[0]);
    }
    else
    {
        curr_x = std::get<0>(yellow_bot_info[0]);
        curr_y = std::get<1>(yellow_bot_info[0]);
    }

    // calculating the x and y velocities
    float kp;
    float err_x = x - curr_x;
    float err_y = y - curr_y;
    float dist_err = sqrt(pow(err_x, 2) + pow(err_y, 2));

    float vel_y = kp * err_y;
    float vel_x = kp * err_x;
    std::vector<bot_packet> packets;
    bot_packet packet;
    packet.vel_angular = 0.0f;
    packet.vel_x = vel_x;
    packet.vel_y = vel_y;
    packet.id = 0;
    packets.push_back(packet);
    emit send(packets);

}

void Drona:: handleState(QByteArray *buffer)
{
    if(state.ParseFromArray(buffer->data(), buffer->size())){
        has_state_ = true;
        if(state.has_detection()){
            // updating blue bots positions
            if(state.detection().robots_blue_size() != 0){
                pandav = state.detection().robots_blue();
                for(auto itr=pandav.begin(); itr != pandav.end(); ++itr){
                    std::tuple<float, float> info(itr->x(), itr->y());
                    blue_bot_info[itr->robot_id()] = info;
                }
            }else{
                LOG << "blue bots not there! paying respects";
            }

            // updating yellow bots positions
            if(state.detection().robots_yellow_size() != 0){
                kaurav = state.detection().robots_yellow();
                for(auto itr=kaurav.begin(); itr != kaurav.end(); ++itr){
                    std::tuple<float, float> info(itr->x(), itr->y());
                    yellow_bot_info[itr->robot_id()] = info;
                }
            }else{
                LOG << "yellow bots not there! paying respects";
            }

        }

        moveToPosition(0.0, 0.0);
    }
}

Drona::~Drona()
{
    delete sender;
    sender_thread.quit();
    sender_thread.wait();
}
