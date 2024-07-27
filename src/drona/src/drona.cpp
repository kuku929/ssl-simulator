#include "drona.h"
#include <QString>
#include <QNetworkDatagram>
#define LOG qDebug() << "[drona] : "

using namespace sslsim;
Drona::Drona(QObject* parent) : QObject(parent),
    sender(new Dhanush())
{
    sender->moveToThread(&sender_thread);
    connect(this, &Drona::send, sender, &Dhanush::send_velocity);
    sender_thread.setObjectName("sender");
    sender_thread.start();
    // allocate the sender to a separate thread

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
    }
}

Drona::~Drona()
{
    delete sender;
    sender_thread.quit();
    sender_thread.wait();
}
