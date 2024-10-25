#ifndef VISHNU_H
#define VISHNU_H
#include <QtNetwork>
#include <QWidget>
#include <QObject>

class Vishnu : public QObject{
    Q_OBJECT
public:
    Vishnu(QObject *parent, QHostAddress address, qint32 port);
    Vishnu(QObject *parent);
    ~Vishnu()
    {
        delete socket;
    }
private:
    QHostAddress m_addr;
    qint32 m_port;
    QUdpSocket *socket;
private slots:
    void handleDatagrams();
};

#endif // VISHNU_H
