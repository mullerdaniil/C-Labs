#include <stdio.h>

int main(int argv, char **argc) {
	
	int array[] = {34, 45, 56, 67, 78, 89};
	int firstMatrix[2][2] = {{2, 1}, {1, 3}};
	int secondMatrix[2][2] = {{1, 2}, {3, 1}};
	int resultMatrix[2][2] = {{0, 0}, {0, 0}};
	
	for (int row = 0; row < 2; row++) {
		for (int column = 0; column < 2; column++) {
			for (int n = 0; n < 2; n++) {
				resultMatrix[row][column] += firstMatrix[row][n] * secondMatrix[n][column];
			}
		}
	}
	
	for (int index = 0; index < 6; index++) {
		printf("array[%d] = %d\n", index, array[index]);
	}
	
	printf("\n");
	
	for (int row = 0; row < 2; row++) {
		for (int column = 0; column < 2; column++) {
			printf("Matrix%d%d = %d\t", row + 1, column + 1, resultMatrix[row][column]);
		}
		printf("\n");
	}
	
	return 0;
}
