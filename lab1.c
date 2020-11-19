#include <stdio.h>

int main(int argv, char **argc) {
	
	float realNumber;
	char symbol;
	
	printf("Enter the real number and the symbol:\n");
	
	scanf("%f", &realNumber);
	scanf(" %c", &symbol);
	
	printf("The real number is %f\n", realNumber);
	printf("The symbol is %c\n", symbol);
	
	return 0;
}
