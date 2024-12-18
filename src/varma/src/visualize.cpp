#include "visualize.h"


void Visualize::interpolate(std::vector<std::pair<double, double>> &points)
{
    // this should be size_type
    // m_lines.resize(points.size() - 1);
    for(int i=0;i < points.size() - 1; ++i){
        auto pos1 = QPointF(points[i].first, points[i].second);
        auto pos2 = QPointF(points[i+1].first, points[i+1].second);
        // add_overwrite<QGraphicsLineItem>(m_lines, scene->addLine(QLineF(pos1, pos2), default_color));
    }
}


template<class T>
void Visualize::add_overwrite(QVector<T *> item_list, T * item){

}
