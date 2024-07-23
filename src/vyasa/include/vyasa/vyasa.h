#ifndef VYASA_H
#define VYASA_H

#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QNetworkDatagram>

class Vyasa: public QObject {
    Q_OBJECT
    public:
        explicit Vyasa(QObject* parent = 0);
        ~Vyasa();
        // todo : add the receiver part to vyasa
        void setPortAndAddress(int port, const QString& address);
        // void sendCommand(float velX, int id);

    signals:
        void recievedState(QByteArray *buffer);

    public slots:
        void handleDatagrams();
        void onSocketError(QAbstractSocket::SocketError socketError);

    private:
        QHostAddress _addr; //address to listen to for ssl-vision data
        quint16 _port; //port to listen to for ssl-vision data
        QUdpSocket* socket;
        QNetworkDatagram datagram;
        QByteArray buffer;
};
#endif // VYASA_H
