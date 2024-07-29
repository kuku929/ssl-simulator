#include "drona.h"
#include <QString>
#include <QNetworkDatagram>
#include <math.h>
#define LOG qDebug() << "[drona] : "
#define TOTAL_BOTS 22

using namespace sslsim;

Drona::Drona(QObject* parent) : QObject(parent),
    sender(new Dhanush())
    // packet(new BotPacket())
{
    sender->moveToThread(&sender_thread);
    connect(this, &Drona::send, sender, &Dhanush::send_velocity);
    // allocate the sender to a separate thread
    sender_thread.setObjectName("sender");
    sender_thread.start();

    packet = new BotPacket[TOTAL_BOTS];

}

void Drona::moveToPosition(int id, float x, float y)
{
    float curr_x, curr_y;

    // updating the current position of the bot
    if (1==1)
    {
        curr_x = blue_bot_info[id].x;
        curr_y = blue_bot_info[id].y;
    }
    else
    {
        curr_x = yellow_bot_info[id].x;
        curr_y = yellow_bot_info[id].y;
    }

    // calculating the x and y velocities
    float err_x = x - curr_x;
    float err_y = y - curr_y;
    float dist_err = sqrt(pow(err_x, 2) + pow(err_y, 2));
    float kp = 0.01;
    float vel_y = -kp * err_y;
    float vel_x = kp * err_x;
    packet[id].vel_angular = 0.0f;
    packet[id].vel_x = vel_x;
    packet[id].vel_y = vel_y;
    packet[id].id = id;

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
                    Bot info(itr->x(), itr->y(), 1);
                    blue_bot_info[itr->robot_id()] = info;
                }
            }else{
                LOG << "blue bots not there! paying respects";
            }

            // updating yellow bots positions
            if(state.detection().robots_yellow_size() != 0){
                kaurav = state.detection().robots_yellow();
                for(auto itr=kaurav.begin(); itr != kaurav.end(); ++itr){
                    Bot info(itr->x(), itr->y());
                    yellow_bot_info[itr->robot_id()] = info;
                }
            }else{
                LOG << "yellow bots not there! paying respects";
            }

        }
        // reseting packet, will make this better
        for(int i=0; i < TOTAL_BOTS/2; ++i){
            packet[i].id = i;
            packet[i].is_blue = false;
            packet[i].vel_x = 0.0f;
            packet[i].vel_y = 0.0f;

        }

        for(int i=0; i < TOTAL_BOTS/2; ++i){
            packet[i].id = i;
            packet[i].is_blue = true;
            packet[i].vel_x = 0.0f;
            packet[i].vel_y = 0.0f;
        }
        moveToPosition(0, 2000.0f, 2000.0f);
        moveToPosition(1, 0.0f, 0.0f);
        emit send(packet);
    }
}

Drona::~Drona()
{
    delete sender;
    sender_thread.quit();
    sender_thread.wait();
}
