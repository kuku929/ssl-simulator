#include "vyasa.h"
#include "core/sslprotocols.h"
// #include "protobuf/ssl_wrapper.pb.h"
// #include "protobuf/sslsim.h"
#include <QString>
#include <QUdpSocket>
#include <QNetworkDatagram>

Vyasa::Vyasa(QObject* parent) : QObject(parent), socket(new QUdpSocket(this))
{
    // create a QUDP socket
    this->_addr.setAddress(SSL_VISION_ADDRESS_LOCALHOST);
    this->_port = quint16(SSL_SIMULATED_VISION_PORT);

    // if socket fails to connect
    connect(socket, &QAbstractSocket::errorOccurred,this, &Vyasa::onSocketError);

    //the problem is with qudpsocket since the slot is being called fine
    socket->bind(_addr, _port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(socket->state() != QAbstractSocket::BoundState){
        qDebug() << "socket not bound";
    }

    // new syntax, do not use SIGNAL() and SLOT()
    auto success = connect(socket, &QUdpSocket::readyRead, this, &Vyasa::handleDatagrams);
    if(!success){
        qDebug() << socket->errorString();
    }
}

Vyasa::~Vyasa()
{
    delete socket;
}

void Vyasa::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"[vyasa] : socket error occured and the error is : "<<socketError;
}

void Vyasa::setPortAndAddress(int port, const QString& address)
{
    this->_port = quint16(port);
    this->_addr.setAddress(address);
}

// void Vyasa::sendCommand(float velX, int id) {
//     double zero = 0.0;
//     RobotControl packet;
//     bool yellow = false;
//     RobotCommand* command = packet.add_robot_commands();
//     command->set_id(id);

//     RobotMoveCommand *move_command = command->mutable_move_command();
//     MoveLocalVelocity *vel_command = move_command->mutable_local_velocity();

//     vel_command->set_forward(velX);
//     vel_command->set_left(0.0f);
//     vel_command->set_angular(0.0f);

//     QByteArray dgram;
//     dgram.resize(packet.ByteSize());
//     packet.SerializeToArray(dgram.data(), dgram.size());
//     // todo : this will probably not work since socket in bounded, use write()
//     if (socket->writeDatagram(dgram, this->_addr_sim, this->_port_sim) > -1) {
//         qDebug("send data");
//     }
// }

void Vyasa::handleDatagrams()
{
// when data comes in
    while(socket->hasPendingDatagrams()){
        datagram = socket->receiveDatagram();
        buffer = datagram.data();
        emit recievedState(&buffer);
    }
}
