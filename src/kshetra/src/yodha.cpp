#include "yodha.h"
#include "shunya/shunya.h"
#include <QtMath>
#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#define LOG qDebug() << "[yodha] : "
#define ROBOT_RADIUS 10
float SUBTEND_ANGLE=30;

inline QPointF transformFromScene(const QPointF &&point)
{
    // converting to m and changing origin to center of field from top left corner
    // QGraphicsScene : (0,0) top left corner, +y downwards vertical, +x forward horizontal
    // Simulator : (0,0) field center, +y downwards vertical, +x forward horizontal
    //NOTE : hard coded field length and width!!!
    return QPointF(point.x()/100 - 4.5, point.y()/100 - 3);
}
YellowBot::YellowBot(QGraphicsScene *scene, QPoint &&point, float orientation, int id):
    id(id)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF bounding_rect = QRectF(-ROBOT_RADIUS, -ROBOT_RADIUS, 2*ROBOT_RADIUS, 2*ROBOT_RADIUS);
    path.arcTo(bounding_rect, SUBTEND_ANGLE, 360-2*SUBTEND_ANGLE);
    path.closeSubpath();
    path.moveTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));
    path.lineTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), -ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));
    body_graphics = new YellowBotGraphics(path, id);
    scene->addItem(body_graphics);

    if(scene->mouseGrabberItem() != nullptr)
        LOG << "mouse accepted";
}
void YellowBot::updatePosition(const QPoint &&point, float orientation)
{
    //sets the robot at location point with radius and color
    body_graphics->setPos(point);

    //assuming that orientation recieved is opposite of what setRotation expects, need to confirm
    body_graphics->setRotation(qRadiansToDegrees(-orientation));
    return;
}

void YellowBot::YellowBotGraphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::yellow);
    painter->drawPath(path());
}

void YellowBot::YellowBotGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void YellowBot::YellowBotGraphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // should I use shunya here? seems fine for now
    Shunya temp;
    // LOG << "item frame : " << event->lastPos();
    // LOG << "qt based transform : " << mapToScene(event->lastPos());
    // LOG << "my transform : " << QPointF(pos().x() - event->lastPos().y() ,event->lastPos().x() + pos().y());
    temp.move_one_bot(id, transformFromScene(mapToScene(event->lastPos())), false);
}

BlueBot::BlueBot(QGraphicsScene *scene, QPoint &&point, float orientation, int id):
    id(id)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF bounding_rect = QRectF(-ROBOT_RADIUS, -ROBOT_RADIUS, 2*ROBOT_RADIUS, 2*ROBOT_RADIUS);
    path.arcTo(bounding_rect, SUBTEND_ANGLE, 360-2*SUBTEND_ANGLE);
    path.closeSubpath();
    path.moveTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));
    path.lineTo(ROBOT_RADIUS*cos(qDegreesToRadians(SUBTEND_ANGLE)), -ROBOT_RADIUS*sin(qDegreesToRadians(SUBTEND_ANGLE)));
    body_graphics = new BlueBotGraphics(path, id);
    scene->addItem(body_graphics);

}
void BlueBot::updatePosition(const QPoint &&point, float orientation)
{
    //sets the robot at location point with radius and color
    body_graphics->setPos(point);

    //assuming that orientation recieved is opposite of what setRotation expects, need to confirm
    body_graphics->setRotation(qRadiansToDegrees(-orientation));
    return;
}

void BlueBot::BlueBotGraphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::blue);
    painter->drawPath(path());
}

void BlueBot::BlueBotGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void BlueBot::BlueBotGraphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Shunya temp;
    // LOG << "item frame : " << event->lastPos();
    // LOG << "qt based transform : " << mapToScene(event->lastPos());
    // LOG << "my transform : " << QPointF(pos().x() - event->lastPos().y() ,event->lastPos().x() + pos().y());
    temp.move_one_bot(id, transformFromScene(mapToScene(event->lastPos())), true);
}
