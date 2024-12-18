#ifndef VPoint_h
#define VPoint_h
#include <stdio.h>
/*
	A structure that stores 2D point
*/

struct VPoint
{
public:
    
	double x, y;
	int id;
	/*
		Constructor for structure; x, y - coordinates
	*/

    VPoint(double nx, double ny, int i) 
	{
		x = nx; 
		y = ny;
		id = i;
	}
    VPoint(double nx, double ny) 
	{
		x = nx; 
		y = ny;
		id = -1;
	}
};

#endif

//
// data members : 
//  - list of points which are the vertexes of the graph
//  - an adjacency list representing the graph
//  - the ad list should store weigts of each edges. 
