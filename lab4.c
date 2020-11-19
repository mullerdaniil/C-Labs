#include <stdio.h>

int main(int argv, char **argc) {
	
	int firstNumber, secondNumber;
	
	char *results[2] = {"NO", "YES"};
	
	printf("Enter the first integer number:\n");
	scanf("%d", &firstNumber);
	
	int result = (firstNumber >= 21) && (firstNumber <= 45);
	printf("The result of the check is %s\n\n", results[result]);
	
	printf("Enter the second integer number:\n");
	scanf("%d", &secondNumber);
	
	printf("The 16th bit of the second number is %d\n\n", (secondNumber >> 15) & 1);
	
	return 0;
}
