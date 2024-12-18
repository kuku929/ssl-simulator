#include "planner.h"

vor::Voronoi * v;
vor::Vertices * ver; // vertices
vor::Vertices * dir; // velocities of vertices
vor::Edges * edg;	 // hrany diagramu

double w = 900; //width of bounding box
double h = 600; //width of bounding box

int target_id = 5;

pair<double, double> startpt, endpt;

double dist(pair<double, double> &p1, pair<double, double> &p2){
    double x = p1.first-p2.first;
    double y = p1.second-p2.second;
    x*=x;
    y*=y;
    double d = (x+y);
    return d;
}

#define LOG qDebug() << "[planner]: "
using namespace gra;

vector<QPointF> plan_path(vector<pair<double, double>> bot_pos, pair<double, double> endpt, int target_id)
{
    LOG << "started";
    using namespace vor;

    v = new Voronoi();
    // ver stores the positions of all adversary bots
    ver = new Vertices();
    //creates the obstacle bots
    for(int i=0; i < bot_pos.size(); ++i)
    {
        ver->push_back(new VPoint( bot_pos[i].first , bot_pos[i].second, i));
        LOG << i << ' ' << bot_pos[i].first << ' ' << bot_pos[i].second;
    }
    int val = 200;
    // start point front
    ver->push_back(new VPoint( ver->at(target_id)->x + val*0.766, ver->at(target_id)->y + val*0.642, bot_pos.size()));
    ver->push_back(new VPoint( ver->at(target_id)->x + val*0.342, ver->at(target_id)->y - val*0.939, bot_pos.size() + 1));
    ver->push_back(new VPoint( ver->at(target_id)->x - val*0.984, ver->at(target_id)->y + val*0.173, bot_pos.size() + 2));

    // end point front
    ver->push_back(new VPoint( endpt.first + val*0.766, endpt.second + val*0.642, bot_pos.size() + 3));
    ver->push_back(new VPoint( endpt.first + val*0.342, endpt.second - val*0.939, bot_pos.size() + 4));
    ver->push_back(new VPoint( endpt.first - val*0.984, endpt.second + val*0.173, bot_pos.size() + 5));

    // LOG << 1;
    // make w and h large if cup happens
    edg = v->GetEdges(ver, w, h, target_id); //GetEdges takes the vertices and the bounding box size as input
                                  // and performs voronoi on them, returning all the edges.

    // LOG << 2;
    Graph g = Graph(edg);
    auto nearest_index = g.getNearestVertex(ver->at(target_id)->x, ver->at(target_id)->y);
    int start_id = nearest_index;
    int end_id = g.getNearestVertex(endpt.first, endpt.second);
    vector<int> path = g.shortestPath(start_id, end_id);
    // LOG << 3;

    vector<QPointF> pos;
    for(int i=0;i<path.size()-1;i++){
        auto p = g.getVertexById(path[i]);
        pos.push_back(QPointF(p.first, p.second));
    }
    LOG << "ended" << path.size();
    return pos;
}
