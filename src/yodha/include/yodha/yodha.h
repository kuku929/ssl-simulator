#ifndef YODHA_H
#define YODHA_H

#include <QGraphicsScene>
#include <QGraphicsPathItem>
class BlueBotGraphics;
class YellowBotGraphics;

class BlueBot{
public:
    BlueBot(){};
    BlueBot(QGraphicsScene *scene, QPointF &&point, float orientation,int id);
    float getx(){ return x; }
    float gety(){ return y; }
    QPointF mapFromScene(float x, float y){ return body_graphics->mapFromScene(x, y); }
    void updatePosition(const QPointF &&point, float orientation);
    int id;
private:
    class BlueBotGraphics : public QGraphicsPathItem{
    public:
        BlueBotGraphics(){};
        BlueBotGraphics(int id): id(id){};
        ~BlueBotGraphics(){};
        BlueBotGraphics(QPainterPath &path, int id):QGraphicsPathItem(path), id(id){};
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    protected:
        int id;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    };
    float x, y, orientation;
    BlueBotGraphics *body_graphics=nullptr;

};

class YellowBot{
public:
    YellowBot(){};
    YellowBot(QGraphicsScene *scene, QPointF &&point, float orientation,int id);
    void updatePosition(const QPointF &&point, float orientation);
    int id;
private:
    class YellowBotGraphics : public QGraphicsPathItem{
    public:
        YellowBotGraphics(){};
        YellowBotGraphics(int id): id(id){};
        YellowBotGraphics(QPainterPath &path, int id):QGraphicsPathItem(path), id(id){};
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    protected:
        int id;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    };
    YellowBotGraphics *body_graphics;
    float x, y, orientation;
};

class Ball{
public:
    Ball(QColor color, float radius);
    Ball(QPointF pos, QGraphicsScene *scene);
    void updatePosition(QPointF pos);
    QPointF getPosition(){ return position; }
private:
    QColor color;
    float radius;
    QGraphicsEllipseItem *graphics = nullptr;
    QPointF position;
};

#endif // YODHA_H
