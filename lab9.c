#include <stdio.h>

int main(int argv, char **argc) {
	
	// task #1
	char string[200];
	
	printf("Enter the string: \n");
	scanf("%s", string);
	printf("\n\n\n");
	
	int index = 0;
	int upperCaseLetterCount = 0, lowerCaseLetterCount = 0, digitCount = 0;
	char currentChar;
	while(string[index] != 0) {
		currentChar = string[index];
		if ('A' <= currentChar && currentChar <= 'Z') {
			upperCaseLetterCount++;
		} else if ('a' <= currentChar && currentChar <= 'z') {
			lowerCaseLetterCount++;
		} else if ('0' <= currentChar && currentChar <= '9') {
			digitCount++;
		}
		
		index++;
	}
	
	printf("The number of\n");
	printf("\tuppercase letters:\t%d\n", upperCaseLetterCount);
	printf("\tlowercase letters:\t%d\n", lowerCaseLetterCount);
	printf("\tdigits:\t\t\t%d\n", digitCount);
	printf("\n\n\n\n");
	
	
	
	// task #3
	int digit;
	char *result[10] = {"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
	
	printf("Enter the digit: \n");
	scanf("%d", &digit);
	printf("\n\n");
	
	if (0 <= digit && digit <= 9) {
		printf("%s\n", result[digit]);
	} else {
		printf("Error: Not a digit!\n");
	}
	
	return 0;
}
