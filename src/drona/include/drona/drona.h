#ifndef DRONA_H
#define DRONA_H

// #define SIMULATOR_MODE

#include "dhanush.h"
#include "yodha/yodha.h"
#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QThread>
#include <QNetworkDatagram>

enum Team{
    BLUE = 0,
    YELLOW = 1
};

class BotPacket;

class Drona : public QObject
{
	Q_OBJECT
public:
    explicit Drona(QObject *parent = 0);
    void moveToPosition(int id, float x, float y, int team, BotPacket *packet);
    void setPlayers(std::shared_ptr<std::vector<BlueBot>> pandav, std::shared_ptr<std::vector<YellowBot>> kaurav);
    void setBall(std::shared_ptr<Ball> ball);
    ~Drona();
    
private:
    struct Bot {
        int x, y;
        Bot() = default;
        Bot(int x, int y, bool is_blue=false): x(x), y(y), is_blue(is_blue){};
        bool is_blue;
    };
    QThread sender_thread;
    Dhanush *sender;
    BotPacket *m_packet;
#if defined SIMULATOR_MODE
    BotPacket *m_blue_packet;
    BotPacket *m_yellow_packet;
#endif
    std::shared_ptr<std::vector<BlueBot>> pandav;
    std::shared_ptr<std::vector<YellowBot>> kaurav;
    std::shared_ptr<Ball> ball;
    bool has_state_;

signals:
    void send(BotPacket* packet);
public slots:
    void handleState(QByteArray *buffer);
};

#endif // DRONA_H
