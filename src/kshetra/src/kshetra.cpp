#include "kshetra.h"
#include <google/protobuf/repeated_field.h>
#include <QNetworkDatagram>
#include <QString>
#include <cmath>
#define BALL_RADIUS 5
#define ROBOT_RADIUS 10
#define LOG qDebug() << "[kshetra] : "
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

inline QPoint vecToPoint(const Vector2f &v){
    return QPoint(v.x(), v.y());
}

inline QRectF boundingSquare(const QPoint &center, int half_side)
{
    return QRectF(center.x() - half_side, center.y() - half_side, 2*half_side, 2*half_side);
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
            setFieldLines(field_geometry.field());
        }
        if(state.has_detection()){
            //drawing ball
            if(state.detection().balls_size() != 0){
                ball = state.detection().balls(0);
                setBall(transformToScene(QPoint(ball.x(), ball.y())));
            }else{
                LOG << "ball not there! paying respects";
            }

            //drawing robots
            if(bots_init_){ // if bots are already placed, just update their position
                //blue bots
                if(state.detection().robots_blue_size() != 0){
                    pandav = state.detection().robots_blue();
                    for(auto itr=pandav.begin(); itr != pandav.end(); ++itr){
                        auto bot = std::find_if(scene_pandav.begin(), scene_pandav.end(), [&](BlueBot b){return b.id == itr->robot_id();});
                        if(bot == scene_pandav.end()){
                            LOG << "adding robot " << itr->robot_id();
                            scene_pandav.push_back(BlueBot(scene,transformToScene(QPoint(itr->x(), itr->y())), itr->orientation(), itr->robot_id()));
                            continue;
                        }
                        bot->updatePosition(transformToScene(QPoint(itr->x(), itr->y())), itr->orientation());
                    }
                }else{
                    LOG << "blue bots not there! paying respects";
                }

                //yellow bots
                if(state.detection().robots_yellow_size() != 0){
                    kaurav = state.detection().robots_yellow();
                    for(auto itr=kaurav.begin(); itr != kaurav.end(); ++itr){
                        auto bot = std::find_if(scene_kaurav.begin(), scene_kaurav.end(), [&](YellowBot b){return b.id == itr->robot_id();});
                        if(bot == scene_kaurav.end()){
                            LOG << "adding robot " << itr->robot_id();
                            scene_kaurav.push_back(YellowBot(scene,transformToScene(QPoint(itr->x(), itr->y())), itr->orientation(), itr->robot_id()));
                            continue;
                        }
                        bot->updatePosition(transformToScene(QPoint(itr->x(), itr->y())), itr->orientation());
                    }
                }else{
                    LOG << "yellow bots not there! paying respects";
                }
            }else{
                //blue bots
                if(state.detection().robots_blue_size() != 0){
                    pandav = state.detection().robots_blue();
                    for(auto itr=pandav.begin(); itr != pandav.end(); ++itr){
                        scene_pandav.push_back(BlueBot(scene,transformToScene(QPoint(itr->x(), itr->y())), itr->orientation(), itr->robot_id()));
                    }
                }else{
                    LOG << "blue bots not there! paying respects";
                }

                //yellow bots
                if(state.detection().robots_yellow_size() != 0){
                    kaurav = state.detection().robots_yellow();
                    for(auto itr=kaurav.begin(); itr != kaurav.end(); ++itr){
                        scene_kaurav.push_back(YellowBot(scene,transformToScene(QPoint(itr->x(), itr->y())), itr->orientation(), itr->robot_id()));
                    }
                }else{
                    LOG << "yellow bots not there! paying respects";
                }
                bots_init_ = true;
                setScene(scene);
            }

        }
    }
}


void Kshetra::setGround(qint32 width, qint32 height)
{
    //sets the ground and field color with dimensions, the length received should be in mm
    if(background_init_) return;
    scene = new QGraphicsScene(QRectF(0,0,width/10, height/10), this);
    scene->setBackgroundBrush(QColor(0,100,50));
    scene->addRect(QRectF(0,0,width/10, height/10), QPen(), Qt::darkGreen);
    background_init_ = true;
}


void Kshetra::setFieldLines(const SSL_GeometryFieldSize &field_info)
{
    //rn i am not storing the field lines since they are static
    if(lines_init_) return;
    auto lines = field_info.field_lines();
    for(auto itr=lines.begin(); itr != lines.end(); ++itr){
        addLine_(scene, transformToScene(vecToPoint(itr->p1())), transformToScene(vecToPoint(itr->p2())), itr->thickness());
    }

    auto arcs = field_info.field_arcs();
    for(auto itr=arcs.begin(); itr != arcs.end(); ++itr){
        addArc_(scene, transformToScene(vecToPoint(itr->center())), itr->radius()/10, itr->thickness());
    }
    lines_init_ = true;
}

void Kshetra::setBall(QPoint &&point)
{
    if(ball_init_){
        scene_ball->setRect(boundingSquare(point, BALL_RADIUS));
        return;
    }
    //sets the ball at location point with radius and color
    scene_ball = addCircle_(scene, point, BALL_RADIUS, Qt::black);
    ball_init_ = true;
}

inline QGraphicsEllipseItem* addCircle_(QGraphicsScene *scene, const QPoint &center, int radius, QColor color)
{
    //internal function to draw a circle
    //the coordinate system of QGraphicsScene is based on top-left corner as origin and an inverted y-axis
    QRectF bounding_rect = QRectF(center.x() - radius, center.y() - radius, 2*radius, 2*radius);
    return scene->addEllipse(bounding_rect, QPen(), QBrush(color));
}

inline QGraphicsLineItem* addLine_(QGraphicsScene *scene, const QPoint &point1, const QPoint &point2, int width)
{
    return scene->addLine(QLineF(point1, point2), QPen(QBrush(Qt::white), width));
}

inline QGraphicsEllipseItem* addArc_(QGraphicsScene *scene, const QPoint center, int radius, int width)
{
    QRectF bounding_rect = QRectF(center.x() - radius, center.y() - radius, 2*radius, 2*radius);
    return scene->addEllipse(bounding_rect, QPen(QBrush(Qt::white), width));
}

inline QPoint Kshetra::transformToScene(QPoint &&point)
{
    //converting to cm by dividing 10
    return QPoint(point.x()/10 + scene->width()/2, point.y()/10 + scene->height()/2);
}
