#include "drona.h"
#include "planner.h"
#include <QString>
#include <cmath>
#include <QtMath>
#include <QNetworkDatagram>
#define LOG qDebug() << "[drona] : "
#define BLUE_BOTS 6
#define YELLOW_BOTS 6

using namespace sslsim;

Drona::Drona(QObject* parent) : QObject(parent),
    sender(new Dhanush())
{
    sender->moveToThread(&sender_thread);
    connect(this, &Drona::send, sender, &Dhanush::send_velocity);
    // allocate the sender to a separate thread
    sender_thread.setObjectName("sender");
    sender_thread.start();
    m_packet = new BotPacket[BLUE_BOTS];
#if defined(SIMULATOR_MODE)
    m_blue_packet = new BotPacket[BLUE_BOTS];
    m_yellow_packet = new BotPacket[YELLOW_BOTS];
#endif
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

void Drona::moveToPosition(int id, float x, float y, int team, BotPacket *packet)
{
    // calculating the x and y velocities
    QPointF relative_pos;
    if(team == Team::BLUE){
        relative_pos = pandav->at(id).mapFromScene(x, y);
        packet[id].is_blue = true;
    }else{
        relative_pos = kaurav->at(id).mapFromScene(x, y);
        packet[id].is_blue = false;
    }
    QPointF err = relative_pos;
    float orientation_err = qAtan2(relative_pos.y(), relative_pos.x());
    orientation_err = relative_pos.y() > 0 ? fabs(orientation_err) : -fabs(orientation_err);
    float dist_err = pow(err.x()*err.x() + err.y()*err.y(), 0.5);
    float kp = 0.01;
    float vel_for = dist_err*kp;
    float vel_th = 2*orientation_err;
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

    static int counter = 0;
    std::vector<std::pair<double, double>> bot_pos;
    for(int i=0; i < kaurav->size(); ++i){
        bot_pos.push_back(make_pair(kaurav->at(i).getx(), kaurav->at(i).gety()));
    }

    std::pair<double, double> endpt;
    endpt.first = 0.0f;
    endpt.second = 100.0f;
    LOG << counter << endl;
    if(counter == 100){
        vertices = plan_path(bot_pos, endpt, 0);
        counter =0;
    }
    emit draw_graph(&vertices);
    // LOG << vertices.size();
    // }
#if defined(SIMULATOR_MODE)
    // reseting packet, will make this better
    for(int i=0; i < BLUE_BOTS; ++i){
        m_blue_packet[i].id = i;
        m_blue_packet[i].is_blue = true;
        m_blue_packet[i].vel_x = 0.0f;
        m_blue_packet[i].vel_y = 0.0f;

    }

    for(int i=0; i < YELLOW_BOTS; ++i){
        m_yellow_packet[i].id = i;
        m_yellow_packet[i].is_blue = false;
        m_yellow_packet[i].vel_x = 0.0f;
        m_yellow_packet[i].vel_y = 0.0f;

    }
    moveToPosition(10, ball->getPosition().x(), ball->getPosition().y(), Team::YELLOW, m_yellow_packet);
    // moveToPosition(0, ball->getPosition().x(), ball->getPosition().y(), Team::YELLOW, m_yellow_packet);
    // moveToPosition(0, ball->getPosition().x(), ball->getPosition().y(), Team::BLUE, m_blue_packet);
    // moveToPosition(1, ball->getPosition().x(), ball->getPosition().y(), Team::BLUE, m_blue_packet);
    emit send(m_blue_packet);
    emit send(m_yellow_packet);
#else

    // reseting packet, will make this better
    for(int i=0; i < YELLOW_BOTS; ++i){
        m_packet[i].id = i;
        m_packet[i].is_blue = false;
        m_packet[i].vel_x = 0.0f;
        m_packet[i].vel_y = 0.0f;
        m_packet[i].vel_angular = 0.0f;

    }
    if(vertices.size() > 0)moveToPosition(kaurav->front().id, vertices.back().x(), vertices.back().y(), Team::YELLOW, m_packet);
    emit send(m_packet);
#endif //SIMULATOR MODE
    counter++;
}

Drona::~Drona()
{
    delete sender;
    sender_thread.quit();
    sender_thread.wait();
}
