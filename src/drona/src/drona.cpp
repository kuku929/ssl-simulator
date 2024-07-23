#include "drona.h"
#include "core/sslprotocols.h"
#include "protobuf/ssl_wrapper.pb.h"
#include "protobuf/sslsim.h"
#include <QString>
#include <QNetworkDatagram>

using namespace sslsim;

Drona::Drona(QObject* parent) : QObject(parent), socket(new QUdpSocket(this)){
    // create a QUDP socket

    this->_addr.setAddress(SSL_VISION_ADDRESS);
    this->_port = quint16(SSL_SIMULATED_VISION_PORT);

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    //the problem is with qudpsocket since the slot is being called fine
    socket->bind(QHostAddress::AnyIPv4, 10020, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(socket->state() != QAbstractSocket::BoundState){
        qDebug() << "socket not bound";
    }

    // new syntax, do not use SIGNAL() and SLOT()
    auto success = connect(socket, &QUdpSocket::readyRead, this, &Drona::handleDatagrams);
    if(!success){
        qDebug() << socket->errorString();
    }
    // qDebug() << socket->bytesAvailable();
}

Drona::~Drona(){
    delete socket;
}

void Drona::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"socket error occured and the error is : "<<socketError;
}

void Drona::setPortAndAddress(int port, const QString& address) {
    this->_port_sim = quint16(port);
    this->_addr_sim.setAddress(address);
}

void Drona::sendCommand(float velX, int id) {
    double zero = 0.0;
    RobotControl packet;
    bool yellow = false;
    RobotCommand* command = packet.add_robot_commands();
    command->set_id(id);

    RobotMoveCommand *move_command = command->mutable_move_command();
    MoveLocalVelocity *vel_command = move_command->mutable_local_velocity();

    vel_command->set_forward(velX);
    vel_command->set_left(0.0f);
    vel_command->set_angular(0.0f);

    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    // todo : this will probably not work since socket in bounded, use write()
    if (socket->writeDatagram(dgram, this->_addr_sim, this->_port_sim) > -1) {
        qDebug("send data");
    }
}

void Drona::handleDatagrams() {
// when data comes in

    QHostAddress sender;
    quint16 senderPort;
    while(socket->hasPendingDatagrams()){
        auto datagram = socket->receiveDatagram();
        auto buffer = datagram.data();
        SSL_WrapperPacket data;
        if(data.ParseFromArray(buffer.data(), buffer.size())){
            if(data.has_geometry()){
                emit recieved("YES");
            }
            // emit recieved(QString::fromStdString(buffer.toStdString()));
        }
        // qDebug() << "Message from: " << sender.toString();
        // qDebug() << "Message: " << buffer;
    }


}
