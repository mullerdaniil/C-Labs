#include <stdio.h>
#include <time.h>

int main(int argv, char **argc) {

	//task #1
	if (argv < 2) {
		printf("Error: No file name!\n");
		exit(1);
	}
	char *fileName = argc[1];
	freopen(fileName, "w", stdout);
	
	
	time_t currentTime = time(NULL);
	struct tm timeStruct;
	char currentDate[50];
	
	for (int index = 0; index < 10; index++) {
		timeStruct = *localtime(&currentTime);
		
		strftime(currentDate, 50, "%bm %d, %Y", &timeStruct);
		printf("%s\n", currentDate);
		currentTime += 24 * 3600;
	}
	
	return 0;
}
