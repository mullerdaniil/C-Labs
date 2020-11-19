#include <stdio.h>

int main(int argv, char **argc) {
	
	int firstNumber, secondNumber;
	
	printf("Enter the octal integer number:\n");
	scanf("%o", &firstNumber);
	
	printf("The number is %d\n\n", firstNumber);
	
	printf("The number in hex is %x\n\n", firstNumber);
	
	printf("The number in hex with 4-digit right shift is %x\n\n", firstNumber >> 4);
	
	printf("Enter the hex integer number:\n");
	scanf("%x", &secondNumber);
	printf("The bitwise AND of two numbers is %x\n\n", firstNumber & secondNumber);
	
	return 0;
}
