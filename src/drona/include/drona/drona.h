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
    QThread sender_thread;
    Dhanush *sender;
    SSL_DetectionBall ball;
    SSL_WrapperPacket state;
    google::protobuf::RepeatedPtrField<SSL_DetectionRobot> pandav;
    google::protobuf::RepeatedPtrField<SSL_DetectionRobot> kaurav;

signals:
    void send();
public slots:
    void handleState(QByteArray *buffer);
};

#endif // DRONA_H
