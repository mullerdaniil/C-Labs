#include <stdio.h>

int main(int argv, char **argc) {
	
	float array[4];
	float *arrayPointer = array;
	
	array[0] = -3.4;
	array[1] = 4.5;
	array[2] = -5.6;
	array[3] = 6.7;
	
	for (int index = 0; index < 4; index++) {
		printf("array[%d] = %f\n", index, *(arrayPointer + index));
	}
	
	printf("\n");
	
	float *dynamicArray = (float *) malloc(4 * sizeof(int));
	
	dynamicArray[0] = -3.4;
	dynamicArray[1] = 4.5;
	dynamicArray[2] = -5.6;
	dynamicArray[3] = 6.7;
	
	for (int index = 0; index < 4; index++) {
		printf("dynamicArray[%d] = %f\n", index, dynamicArray[index]);
	}
	
	free(dynamicArray);
	
	return 0;
}
