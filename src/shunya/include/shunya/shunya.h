#ifndef SHUNYA_H
#define SHUNYA_H

#include "protobuf/ssl_simulation_control.pb.h"
#include <QObject>
#include <QUdpSocket>
#include <QPointF>

class Shunya : public QObject
{
    Q_OBJECT
public:
    explicit Shunya(QObject *parent = nullptr);
    void setBotPosition(google::protobuf::RepeatedPtrField<sslsim::TeleportRobot> *bot, int id, float x, float y, float orientation=0, bool is_blue=true);
    ~Shunya();

public slots:
    void setup();
    void move_one_bot(int id, QPointF point, bool is_blue);
    void onSocketError(QAbstractSocket::SocketError socketError);
    void handleDatagrams();

signals:

private:
    sslsim::SimulatorCommand *command;
    QUdpSocket *socket;
    QHostAddress _addr;
    quint16 _port;
};

#endif // SHUNYA_H
