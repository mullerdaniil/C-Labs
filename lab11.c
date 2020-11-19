#include <stdio.h>
#include "triangle.h"

int main(int argv, char **argc) {
	
	// testing task #6
	float x1, y1, x2, y2, x3, y3;
	printf("Enter the 6 numbers to create a new triangle: x1 y1 x2 y2 x3 y3\n");
	scanf("%f %f %f %f %f %f", &x1, &y1, &x2, &y2, &x3, &y3);
	printf("\n");
	struct Triangle triangle = createNewTriangle(x1, y1, x2, y2, x3, y3);

	printf("The length of the triangle is %f\n\n", triangleLength(triangle));
	printf("The area of the triangle is %f\n\n", triangleArea(triangle));
	
	return 0;
}
