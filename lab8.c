#include <stdio.h>

int main(int argv, char **argc) {
	
	int n;
	char firstString[200];
	char secondString[200];
	char resultString[200];
	char symbol;
	
	
//	// task #2
//	printf("Enter two strings and n: \n");
//	scanf("%s %s %d", firstString, secondString, &n);
//	printf("\n\n");
//	
//	strcpy(resultString, firstString);
//	strncat(resultString, secondString, n);
//	printf("The concatenation of the 1st string and n symbols of the 2nd string:\n%s\n\n\n\n", resultString);
//	
//	
//	// task #3
//	printf("Enter two strings: \n");
//	scanf("%s %s", firstString, secondString);
//	printf("\n\n");
//	
//	printf("The comparison result:\n");
//	int comparisonResult = strcmp(firstString, secondString);
//	if (comparisonResult > 0) {
//		printf("The 1st string is bigger than the 2nd string");
//	} else if (comparisonResult < 0) {
//		printf("The 1st string is lower than the 2nd string");
//	} else {
//		printf("The 1st string equals to the 2nd string");
//	}
//	printf("\n\n\n\n");
//	
//	
//	// task #6
//	printf("Enter two strings and n: \n");
//	scanf("%s %s %d", firstString, secondString, &n);
//	printf("\n\n");
//	
//	strncpy(firstString, secondString, n);
//	printf("The 1st string with copied symbols from the 2nd string:\n%s\n\n\n\n", firstString);
//	
//	
//	// task #9
//	printf("Enter the string and the symbol: \n");
//	scanf("%s %c", firstString, &symbol);
//	printf("\n\n");
//	
//	char *findSymbolResult = NULL;
//	findSymbolResult = strrchr(firstString, symbol);
//	if (findSymbolResult == NULL) {
//		printf("No such symbol was found");
//	} else {
//		printf("The last occurence of the symbol is %d", findSymbolResult - firstString);
//	}
//	printf("\n\n\n\n");
	
	
	// task #12
	printf("Enter two strings: \n");
	scanf("%s %s", firstString, secondString);
	printf("\n\n");
	

	
	
	
	printf("The size of the 1st string prefix not containing the 2nd string symbols is %d\n", strcspn(firstString, secondString));
	
	
	return 0;
}
