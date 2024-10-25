#include "vishnu.h"
#include "shunya/shunya.h"
#include "core/sslprotocols.h"
#include "protobuf/ssl_gc_referee_message.pb.h"
#define LOG qDebug() << "[vishnu] : "

Vishnu::Vishnu(QObject *parent, QHostAddress address, qint32 port):
    QObject(parent),
    m_addr(address),
    m_port(port),
    socket(new QUdpSocket(this))
{
    socket->bind(m_addr, m_port);
    connect(socket, &QUdpSocket::readyRead, this, &Vishnu::handleDatagrams);
}

Vishnu::Vishnu(QObject *parent):
    QObject(parent),
    socket(new QUdpSocket(this))
{
    LOG << "staring";
    m_addr.setAddress(SSL_GAME_CONTROLLER_ADDRESS);
    m_port = SSL_GAME_CONTROLLER_PORT;
    socket->bind(QHostAddress::AnyIPv4, m_port);
    // needed to listen to multicast packages apparently, spent too long on this ;)
    socket->joinMulticastGroup(m_addr);
    connect(socket, &QUdpSocket::readyRead, this, &Vishnu::handleDatagrams);
}
void Vishnu::handleDatagrams()
{
    auto datagram = socket->receiveDatagram();
    auto buffer = datagram.data();
    Referee ref_msg;
    ref_msg.ParseFromArray(buffer.data(), buffer.size());
    if(ref_msg.stage() == Referee::NORMAL_FIRST_HALF){
        LOG << "STARTING GAME!";
        Shunya shunya;
        shunya.setup();
    }
}

