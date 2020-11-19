#include "triangle.h"

float triangleLength(struct Triangle triangle) {
	float z12 = sqrt((triangle.p1.x - triangle.p2.x) * (triangle.p1.x - triangle.p2.x) + (triangle.p1.y - triangle.p2.y) * (triangle.p1.y - triangle.p2.y));
	float z23 = sqrt((triangle.p2.x - triangle.p3.x) * (triangle.p2.x - triangle.p3.x) + (triangle.p2.y - triangle.p3.y) * (triangle.p2.y - triangle.p3.y));
	float z31 = sqrt((triangle.p3.x - triangle.p1.x) * (triangle.p3.x - triangle.p1.x) + (triangle.p3.y - triangle.p1.y) * (triangle.p3.y - triangle.p1.y));
	return z12 + z23 + z31;
}

float triangleArea(struct Triangle triangle) {
	return 0.5 * abs(triangle.p1.x * triangle.p2.y + triangle.p2.x * triangle.p3.y + triangle.p3.x * triangle.p1.y - triangle.p1.y * triangle.p2.x - triangle.p2.y * triangle.p3.x - triangle.p3.y * triangle.p1.x);
}

struct Triangle createNewTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
	struct Triangle newTriangle;
	newTriangle.p1.x = x1;
	newTriangle.p2.x = x2;
	newTriangle.p3.x = x3;
	newTriangle.p1.y = y1;
	newTriangle.p2.y = y2;
	newTriangle.p3.y = y3;
	return newTriangle;
}
