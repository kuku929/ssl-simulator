#ifndef DRONA_H
#define DRONA_H

#include "dhanush.h"
#include "protobuf/ssl_detection.pb.h"
#include "protobuf/ssl_wrapper.pb.h"
#include "protobuf/ssl_detection.pb.h"
#include <google/protobuf/repeated_field.h>
#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QThread>
#include <QNetworkDatagram>

class Drona : public QObject
{
	Q_OBJECT
public:
    explicit Drona(QObject *parent = 0);
    void moveToPosition(int id, float x, float y);
    ~Drona();
    
private:
    struct Bot{
        int x, y;
        int id;
        Bot(int x, int y,int id, bool is_blue=false): x(x), y(y), is_blue(is_blue), id(id){};
        bool is_blue;
    };
    QThread sender_thread;
    Dhanush *sender;
    SSL_DetectionBall ball;
    SSL_WrapperPacket state;
    google::protobuf::RepeatedPtrField<SSL_DetectionRobot> pandav;
    google::protobuf::RepeatedPtrField<SSL_DetectionRobot> kaurav;
    bool has_state_;
    std::map<int, Bot> blue_bots;
    std::map<int, Bot> yellow_bots;

signals:
    void send();
public slots:
    void handleState(QByteArray *buffer);
};

#endif // DRONA_H
