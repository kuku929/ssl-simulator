#include "kshetra.h"
#include <google/protobuf/repeated_field.h>
#include <QNetworkDatagram>
#include <QString>
#define BALL_RADIUS 10
#define ROBOT_RADIUS 20
Kshetra::Kshetra(QWidget *parent):
    QGraphicsView(parent),
    painter(new QPainter()),
    scene(new QGraphicsScene(this))

{}


Kshetra::~Kshetra()
{
    delete painter;
    delete scene;
    painter = nullptr;
    scene = nullptr;
}
void Kshetra::handleState(QByteArray *buffer)
{
    //slot when message received by vyasa, draws the robots
    if(state.ParseFromArray(buffer->data(), buffer->size())){
        has_state_ = true;
        if(state.has_geometry()){
            field_geometry = state.geometry();
            qint32 field_width = field_geometry.field().field_length(); //refers to x axis
            qint32 field_height = field_geometry.field().field_width(); //refers to y axis

            //drawing field
            setGround(field_width, field_height);
        }
        if(state.has_detection()){
            //drawing ball
            if(state.detection().balls_size() != 0){
                ball = state.detection().balls(0);
            }else{
                //if ball not received use the last ball position
                qDebug() << "ball not there! paying respects";
            }
            //if the first packet does not have ball position then scam will happen
            setBall(transformToScene(QPoint(ball.x(), ball.y())));

            //drawing robots
            if(state.detection().robots_blue_size() != 0){
                pandav = state.detection().robots_blue();
                for(auto itr=pandav.begin(); itr != pandav.end(); ++itr){
                    setBot(transformToScene(QPoint(itr->x(), itr->y())), itr->orientation(),true);
                    // qDebug() << "[kshetra] : " << itr->x() << ' ' << itr->y();
                }
            }else{
                qDebug() << "blue bots not there! paying respects";
            }

            if(state.detection().robots_yellow_size() != 0){
                kaurav = state.detection().robots_yellow();
                for(auto itr=kaurav.begin(); itr != kaurav.end(); ++itr){
                    setBot(transformToScene(QPoint(itr->x(), itr->y())), itr->orientation());
                }
            }else{
                qDebug() << "yellow bots not there! paying respects";
            }

        }
        setScene(scene);
    }
}


void Kshetra::setGround(qint32 width, qint32 height)
{
    //sets the ground and field color with dimensions, the length received should be in mm
    if(scene_init_){
        scene->setSceneRect(QRectF(0,0,width/10, height/10));
    }else{
        scene = new QGraphicsScene(QRectF(0,0,width/10, height/10), this);
        scene_init_=true;
    }
    scene->setBackgroundBrush(QColor(0,100,50));
    scene->addRect(QRectF(0,0,width/10, height/10), QPen(), Qt::darkGreen);
}

void Kshetra::setBall(QPoint &&point)
{
    //sets the ball at location point with radius and color
    if(!scene_init_){
        scene = new QGraphicsScene(this);
        scene_init_ = true;
    }

    addCircle_(point, BALL_RADIUS, Qt::black);
}

void Kshetra::setBot(QPoint &&point, float orientation, bool is_blue)
{
    //sets the robot at location point with radius and color
    //todo : implement orientation
    if(!scene_init_){
        scene = new QGraphicsScene(this);
        scene_init_ = true;
    }
    QColor bot_color;
    if(is_blue){
        bot_color = Qt::blue;
    }else{
        bot_color = Qt::yellow;
    }
    addCircle_(point, ROBOT_RADIUS, bot_color);
}

inline void Kshetra::addCircle_(QPoint &center, int radius, QColor color)
{
    //internal function to draw a circle
    QRectF bounding_rect = QRectF(center.x() - radius, center.y() - radius, radius, radius);
    scene->addEllipse(bounding_rect, QPen(), QBrush(color));
}

inline QPoint Kshetra::transformToScene(QPoint &&point)
{
    //converting to cm by dividing 10
    return QPoint(point.x()/10 + scene->width()/2, point.y()/10 + scene->height()/2);
}
