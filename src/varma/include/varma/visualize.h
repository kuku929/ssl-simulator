#ifndef VISUALIZE_H
#define VISUALIZE_H

#include "QGraphicsItem"
#include "QGraphicsScene"

class Visualize
{
    // this class implements
    // basic visualization features
    // for the simulator.
    // manage the drawing and clearing at each frame
    // within this class and only allow user to add
    // Objects and update them.
    // TrackedObjects have a longer lifetime than FrameObjects.
    // they will persist, but one has to update them. How does
    // the user keep a handle to these? a nice idea, attach
    // another graphicsitem to a TrackedObject, it will follow this
    // object around.
    // for now, focussing on getting frameobjects done.
public:
    static const QPen default_color;
    Visualize(QGraphicsScene *scene): scene(scene){}
    void add_line(QPointF &pos1, QPointF &pos2, QPen color=default_color){
        m_lines.push_back(scene->addLine(QLineF(pos1, pos2), color));
    }
    void interpolate(std::vector<std::pair<double, double>> &points);
    void delete_object(QGraphicsItem *item){
        scene->removeItem(item);
    }

private:
    Visualize(); //making this private, so that no one can call it
    QVector<QGraphicsLineItem *> m_lines;
    QGraphicsScene *scene;
};

const QPen Visualize::default_color  = QPen(Qt::red, 1);

#endif // VISUALIZE_H
