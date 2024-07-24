#ifndef KSHETRA_H
#define KSHETRA_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include "protobuf/ssl_wrapper.pb.h"
#include "protobuf/ssl_geometry.pb.h"
#include "protobuf/ssl_detection.pb.h"
#include <google/protobuf/repeated_field.h>

class Bot;
class Ball;

//primitive graphics function for later use
inline QGraphicsEllipseItem* addCircle_(QGraphicsScene *scene, const QPoint &center, int radius, QColor color);
inline QGraphicsLineItem* addLine_(QGraphicsScene *scene, const QPoint &point1, const QPoint &point2, int width);
inline QGraphicsEllipseItem* addArc_(QGraphicsScene *scene, const QPoint center, int radius, int width);

class Kshetra : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Kshetra(QWidget *parent=0);
    inline QPoint transformToScene(QPoint &&point);
    void setGround(qint32 length, qint32 width);
    void setFieldLines(const SSL_GeometryFieldSize &field_info);
    void setBall(QPoint &&point);
    ~Kshetra();

public slots:
    void handleState(QByteArray *buffer);

private:
    struct Bot{
        Bot(QGraphicsScene *scene, QPoint &&point, float orientation,int id, bool is_blue=false);
        void updatePosition(const QPoint &&point, float orientation);
        bool is_blue;
        int id;
        QGraphicsEllipseItem *body_graphics;
        QGraphicsEllipseItem *orientation_graphics;
    };

    QGraphicsScene *scene;
    QPainter *painter;
    QGraphicsEllipseItem *scene_ball;
    std::vector<Bot> scene_pandav;
    std::vector<Bot> scene_kaurav;

    SSL_WrapperPacket state;
    SSL_GeometryData field_geometry;
    SSL_DetectionBall ball;
    google::protobuf::RepeatedPtrField<SSL_DetectionRobot> pandav;
    google::protobuf::RepeatedPtrField<SSL_DetectionRobot> kaurav;

    bool has_state_;
    bool background_init_ = false;
    bool lines_init_ = false;
    bool ball_init_ = false;
    bool bots_init_ = false;

    // Ball scene_ball;

};

#endif // KSHETRA_H
