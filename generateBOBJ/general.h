#ifndef _GENERAL_H
#define _GENERAL_H

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>


void countvectors(const std::string& name, float* v, float * n, int *f, int cnt, int edgnum);
void readObjFile(const std::string& name, float* v, float * n, int *f, int cnt, int edgnum);
void outputBOBJ(const std::string& name, float *vectors, float * vetnormal, int * facesindices, int cnt, int edgnum);
void outputOBJ(const std::string& name, float *vectors, float * vetnormal, int * facesindices, int cnt, int edgnum);
struct Triangle {
	Triangle() : flags(0) { c[0] = c[1] = c[2] = 0; }
	Triangle(int n0, int n1, int n2) : flags(0) { c[0] = n0; c[1] = n1; c[2] = n2; }

	int c[3];
	int flags;
};

#endif   
