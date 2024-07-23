#ifndef KSHETRA_H
#define KSHETRA_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include "protobuf/ssl_wrapper.pb.h"
#include "protobuf/ssl_geometry.pb.h"
#include "protobuf/ssl_detection.pb.h"
#include <google/protobuf/repeated_field.h>

class Kshetra : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Kshetra(QWidget *parent=0);
    inline QPoint transformToScene(QPoint &&point);
    void setGround(qint32 length, qint32 width);
    void setBall(QPoint &&point);
    void setBot(QPoint &&point, float orientation, bool is_blue=false);
    ~Kshetra();

public slots:
    void handleState(QByteArray *buffer);

private:
    QGraphicsScene *scene;
    QPainter *painter;
    SSL_WrapperPacket state;
    SSL_GeometryData field_geometry;
    SSL_DetectionBall ball;
    google::protobuf::RepeatedPtrField<SSL_DetectionRobot> pandav;
    google::protobuf::RepeatedPtrField<SSL_DetectionRobot> kaurav;
    bool has_state_;
    bool scene_init_;


    inline void addCircle_(QPoint &center, int radius, QColor color);
};

#endif // KSHETRA_H
