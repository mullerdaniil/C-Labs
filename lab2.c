#include <stdio.h>
#include <math.h>

int main(int argv, char **argc) {
	
	char divisionByZeroError[] = "Error: Division by zero. Incorrect argument!\n";
	char negativeNumberSquareRoot[] = "Error: Taking the square root of a negative number!\n";
	
	float x, z1, z2;
	
	printf("Enter the argument:\n");
	scanf("%f", &x);
	
	if (x * x - 2 * x - 3 + (x - 1) * sqrt(x * x - 9) == 0 || x - 3 == 0) {
		printf("%s", divisionByZeroError);
	} else {
		if (x * x - 9 < 0 || (x + 3) / (x - 3) < 0) {
			printf("%s", negativeNumberSquareRoot);
		} else {
			z1 = (x * x + 2 * x - 3 + (x + 1) * sqrt(x * x - 9)) / (x * x - 2 * x - 3 + (x - 1) * sqrt(x * x - 9));
			z2 = sqrt((x + 3) / (x - 3));
			
			printf("z1 = %f\n", z1);
			printf("z2 = %f\n", z2);
		}
	}
	
	return 0;
}
