#ifndef graph_h
#define graph_h

#include "Voronoi.h"
#include <vector>
#include <map>

using namespace std;

namespace gra{
    class Graph{
        public:
            Graph(vor::Edges * e);

            void display();

            int getNearestVertex(double x, double y);

            pair<int, int> getPathEndpoints(pair<int, int> startVertices, pair<int, int> endVertices, double startx, double starty, double endx, double endy);

            double dist(pair<double, double>A, pair<double, double> B);

            vector<int> shortestPath(int start, int end);

            pair<double, double> getVertexById(int id);
            vector<int> vertices;
            vector<pair<double,pair<int, int>>> edges;
            map<int, VPoint*> idmap;
    };
}


#endif