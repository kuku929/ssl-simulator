#include "shunya.h"
#include "core/sslprotocols.h"
#include "protobuf/ssl_simulation_error.pb.h"
#include <QNetworkDatagram>

Shunya::Shunya(QObject *parent)
    : QObject{parent},
    socket(new QUdpSocket(this)),
    command(new sslsim::SimulatorCommand())
{

    // this->_addr.setAddress(SSL_VISION_ADDRESS_LOCALHOST);
    // this->_port = quint16(SSL_SIMULATED_VISION_PORT);
    // // if socket fails to connect
    // connect(socket, &QAbstractSocket::errorOccurred,this, &Shunya::onSocketError);
    // socket->bind(_addr, _port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    // if(socket->state() != QAbstractSocket::BoundState){
    //     qDebug() << "socket not bound";
    // }
    // // new syntax, do not use SIGNAL() and SLOT()
    // auto success = connect(socket, &QUdpSocket::readyRead, this, &Shunya::handleDatagrams);
    // if(!success){ qDebug() << socket->errorString();}

}

void Shunya::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"[shunya] : socket error occured and the error is : "<<socketError;
}

void Shunya::handleDatagrams()
{
// when data comes in
    while(socket->hasPendingDatagrams()){
        qDebug() << "[shunya] : simulator error ";
        auto datagram = socket->receiveDatagram();
        auto buffer = datagram.data();

        sslsim::SimulatorResponse error;
        if(error.ParseFromArray(buffer.data(), buffer.size())){
            if(!error.errors().begin()->has_message()){
                break;
            }
            for(auto itr=error.errors().begin(); itr != error.errors().end(); itr++){
                qDebug() << QString(itr->message().data());
            }
        }
    }
}

Shunya::~Shunya(){
    delete socket;
    delete command;
    command = nullptr;
    socket = nullptr;
}
void Shunya::setup()
{
    //creating the message
    command = new sslsim::SimulatorCommand();
    sslsim::SimulatorControl *control = command->mutable_control();
    auto bot = control->mutable_teleport_robot();

    //initial position of the bots can be set here
    setBotPosition(bot, 0, 0, 0);
    setBotPosition(bot, 1, 1,-1);
    setBotPosition(bot, 2, 1.5,0);
    setBotPosition(bot, 3, 1.5,1);
    setBotPosition(bot, 4, 1.5,-1);
    QByteArray dgram;
    dgram.resize(command->ByteSize());
    command->SerializeToArray(dgram.data(), dgram.size());

    //sending message
    if (socket->writeDatagram(dgram, QHostAddress::LocalHost, SSL_SIMULATION_CONTROL_PORT) > -1) {
        qDebug("[shunya] : sent data");
    }
}

void Shunya::setBotPosition(google::protobuf::RepeatedPtrField<sslsim::TeleportRobot> *bot, int id, float x, float y, float orientation, bool is_blue)
{
    auto bot_pos = bot->Add();
    gameController::BotId *bot_id = bot_pos->mutable_id();
    if(is_blue){
        bot_id->set_team(gameController::BLUE);
    }else{
        bot_id->set_team(gameController::YELLOW);
    }
    bot_id->set_id(id);
    //we cannot pass partial positions as of now, need to give all values
    bot_pos->set_x(x);
    bot_pos->set_y(y);
    bot_pos->set_present(true);
    bot_pos->set_by_force(true);
    bot_pos->set_orientation(orientation);

}
