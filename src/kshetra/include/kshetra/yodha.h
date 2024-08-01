#ifndef YODHA_H
#define YODHA_H

#include <QGraphicsScene>
#include <QGraphicsPathItem>

class BlueBot{
public:
    BlueBot(QGraphicsScene *scene, QPoint &&point, float orientation,int id);
    void updatePosition(const QPoint &&point, float orientation);
    int id;
private:
    class BlueBotGraphics : public QGraphicsPathItem{
    public:
        BlueBotGraphics(){};
        BlueBotGraphics(int id): id(id){};
        BlueBotGraphics(QPainterPath &path, int id):QGraphicsPathItem(path), id(id){};
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    protected:
        int id;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    };
    BlueBotGraphics *body_graphics;

};

class YellowBot{
public:
    YellowBot(QGraphicsScene *scene, QPoint &&point, float orientation,int id);
    void updatePosition(const QPoint &&point, float orientation);
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
};

#endif // YODHA_H
