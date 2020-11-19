#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <math.h>

struct Point {
	float x, y;
};

struct Triangle {
	struct Point p1, p2, p3;
};

float triangleLength(struct Triangle triangle);

float triangleArea(struct Triangle triangle);

struct Triangle createNewTriangle(float x1, float y1, float x2, float y2, float x3, float y3);

#endif
