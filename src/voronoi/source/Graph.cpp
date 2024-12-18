#include <vector>
#include <cmath>
#include <map>
#include "Voronoi.h"
#include "VPoint.h"
#include "Graph.h"
using namespace std;
using namespace vor;
using namespace gra;

Graph::Graph(Edges * e){
    // we assume each vertex which is the vertex of an edge has an id
    //cout << e->size() << "\n";

    map<int, bool> seen;
    for (Edges::iterator i = e->begin();i!=e->end();++i){
        if(seen[((*i)->start)->id]==false){
            seen[((*i)->start)->id]=true;
            idmap[((*i)->start)->id] = (*i)->start;
        }
        if(seen[((*i)->end)->id]==false){
            seen[((*i)->end)->id]=true;
            idmap[((*i)->end)->id] = (*i)->end;
        }
    }
    for(auto p:seen){
        if(p.second==true){
            vertices.push_back(p.first);
        }
    }
    for (Edges::iterator i = e->begin();i!=e->end();++i){
        int start_id = (*i)->start->id;
        int end_id = (*i)->end->id;
        int start_x = (*i)->start->x;
        int start_y = (*i)->start->y;
        int end_x = (*i)->end->x;
        int end_y = (*i)->end->y;
        double dist = (start_x-end_x)*(start_x-end_x) + (start_y-end_y)*(start_y-end_y);
        dist = pow(dist, 0.5);
        edges.push_back(make_pair(dist, make_pair(start_id, end_id)));
    }
}


void Graph::display(){
    cout << "Vertices : ";
    for(auto i:vertices){
        cout << i << " ";
    }
    cout << "\n";

    cout << "Edges : ";
    for(auto i:edges){
        cout << i.second.first << " " << i.second.second << " - " << i.first << '\n';
    }
}

int Graph::getNearestVertex(double x, double y){
    int minind = 0;
    double minval = dist(make_pair(x, y), getVertexById(vertices[0]));
    for(int i=1;i<vertices.size();i++){
        double d = dist(make_pair(x, y), getVertexById(vertices[i]));
        if(d<minval){
            minind = i;
            minval = d;
        }
    }
    return vertices[minind];
}


pair<int, int> Graph::getPathEndpoints(pair<int, int> startVertices, pair<int, int> endVertices, double startx, double starty, double endx, double endy){
    pair<double, double> A = make_pair(startx, starty);
    pair<double, double> B = make_pair((*idmap[startVertices.first]).x, (*idmap[startVertices.first]).y);
    pair<double, double> C = make_pair((*idmap[startVertices.second]).x, (*idmap[startVertices.second]).y);
    pair<double, double> D = make_pair(endx, endy);
    pair<double, double> E = make_pair((*idmap[endVertices.first]).x, (*idmap[endVertices.first]).y);
    pair<double, double> F = make_pair((*idmap[endVertices.second]).x, (*idmap[endVertices.second]).y);
    double AB = dist(A, B);
    double AC = dist(A, C);
    double BE = dist(B, E);
    double BF = dist(B, F);
    double CE = dist(C, E);
    double CF = dist(C, F);
    double ED = dist(E, D);
    double FD = dist(F, D);
    double path1 = AB+BF+FD;
    double path2 = AB+BE+ED;
    double path3 = AC+CF+FD;
    double path4 = AC+CE+ED;
    if(path1<=path2 and path1<=path3 and path1<=path4)return make_pair(startVertices.first, endVertices.second);
    if(path2<=path1 and path2<=path3 and path2<=path4)return make_pair(startVertices.first, endVertices.first);
    if(path3<=path1 and path3<=path2 and path3<=path4)return make_pair(startVertices.second, endVertices.second);
    else return make_pair(startVertices.second, endVertices.first);

}

double Graph::dist(pair<double, double>A, pair<double, double> B){
    double x1=A.first, y1 = A.second, x2 = B.first, y2 = B.second;
    double d = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
    d = pow(d, 0.5);
    return d;
}

pair<double, double> Graph::getVertexById(int id){
    return make_pair((*idmap[id]).x, (*idmap[id]).y);
}

vector<int> Graph::shortestPath(int start, int end){
    map<int, double> d;
    for(int v :vertices){
        d[v] = -1.00;
    }
    map<int, vector<pair<int , double>>> adlist;
    for (auto e:edges){
        int A = e.second.first;
        int B = e.second.second;
        double d = e.first;
        adlist[A].push_back(make_pair(B, d));
        adlist[B].push_back(make_pair(A, d));
    }

    priority_queue<pair<pair<double, int>, int>> pq;
    pq.push(make_pair(make_pair(0.0, start), -1));
    map<int, int> backtrack;
    while(!pq.empty()){
        cout << "size: " << pq.size() << endl;
        pair<pair<double, int>, int> t = pq.top();
        pq.pop();
        if(d[t.first.second]!=-1.0){
            continue;
        }
        d[t.first.second] = -1*(t.first.first);
        backtrack[t.first.second] = t.second;
        for(auto edge:adlist[t.first.second]){
            int next_vertex = edge.first;
            double weight = edge.second;
            if(d[next_vertex]!=-1.0){
                continue;
            }
            pq.push(make_pair(make_pair(-d[t.first.second]-weight, next_vertex), t.first.second));
        }
    }
    vector<int> path;
    path.push_back(end);
    int MAX_SIZE = 100;
    int i=0;
    while(backtrack[end]!=-1){
        if(i > MAX_SIZE)break;
        i++;
        // cout << "fuck" << endl;
        path.push_back(backtrack[end]);
        end = backtrack[end];
    }
    return path;
} 

vector<int> vertices;
vector<pair<double,pair<int, int>>> edges;
map<int, VPoint*> idmap;
