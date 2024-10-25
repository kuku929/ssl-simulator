#include "drona.h"
#include <QString>
#include <cmath>
#include <QtMath>
#include <QNetworkDatagram>
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

void Drona::setPlayers(std::shared_ptr<std::vector<BlueBot>> pandav, std::shared_ptr<std::vector<YellowBot>> kaurav)
{
    this->pandav = pandav;
    this->kaurav = kaurav;
}

void Drona::setBall(std::shared_ptr<Ball> ball)
{
    this->ball = ball;
}

void Drona::moveToPosition(int id, float x, float y)
{
    // calculating the x and y velocities
    QPointF relative_pos = pandav->at(id).mapFromScene(x, y);
    QPointF err = relative_pos;
    float orientation_err = qAtan2(relative_pos.y(), relative_pos.x());
    orientation_err = relative_pos.y() > 0 ? fabs(orientation_err) : -fabs(orientation_err);
    float dist_err = pow(err.x()*err.x() + err.y()*err.y(), 0.5);
    float kp = 0.01;
    float vel_for = dist_err*kp;
    float vel_th = 2*orientation_err;
    // LOG << id << " : " << err.x() << ' '<< err.y();
    packet[id].vel_angular = vel_th;
    packet[id].vel_x = vel_for;
    packet[id].vel_y = 0.0f;
    packet[id].id = id;
    packet[id].kick_speed = 5.0f;

}

void Drona::handleState(QByteArray *buffer)
{
    // every time new position is received, recalculate velocity and send
    // updated velocity, SEX.

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
    int bot_index = pandav->size() - 1;
    moveToPosition(1, ball->getPosition().x(), ball->getPosition().y());
    moveToPosition(0, ball->getPosition().x(), ball->getPosition().y());
    emit send(packet);
}

Drona::~Drona()
{
    delete sender;
    sender_thread.quit();
    sender_thread.wait();
}
