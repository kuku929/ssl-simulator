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

void Drona::handleState(QByteArray *buffer)
{
    //slot when message received by vyasa, draws the robots
    if(state.ParseFromArray(buffer->data(), buffer->size())){
        if(state.has_detection()){
            //drawing ball
            if(state.detection().balls_size() != 0){
                ball = state.detection().balls(0);
            }else{
                LOG << "ball not there! paying respects";
            }
            if(state.detection().robots_blue_size() != 0){
                pandav = state.detection().robots_blue();
                for(auto itr=pandav.begin(); itr != pandav.end(); ++itr){
                }
                }else{
                    LOG << "blue bots not there! paying respects";
                }

                //yellow bots
                if(state.detection().robots_yellow_size() != 0){
                    kaurav = state.detection().robots_yellow();
                    for(auto itr=kaurav.begin(); itr != kaurav.end(); ++itr){
                    }
                }else{
                    LOG << "yellow bots not there! paying respects";
                }
            }else{
                //blue bots
                if(state.detection().robots_blue_size() != 0){
                    pandav = state.detection().robots_blue();
                    for(auto itr=pandav.begin(); itr != pandav.end(); ++itr){
                        scene_pandav.push_back(Bot(scene,transformToScene(QPoint(itr->x(), itr->y())), itr->orientation(), itr->robot_id(), true));
                    }
                }else{
                    LOG << "blue bots not there! paying respects";
                }

                //yellow bots
                if(state.detection().robots_yellow_size() != 0){
                    kaurav = state.detection().robots_yellow();
                    for(auto itr=kaurav.begin(); itr != kaurav.end(); ++itr){
                        scene_kaurav.push_back(Bot(scene,transformToScene(QPoint(itr->x(), itr->y())), itr->orientation(), itr->robot_id()));
                    }
                }else{
                    LOG << "yellow bots not there! paying respects";
                }
                bots_init_ = true;
                setScene(scene);
            }

        }
    }
    emit send();
}

Drona::~Drona()
{
    delete sender;
    sender_thread.quit();
    sender_thread.wait();
}
