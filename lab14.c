#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

char INPUT_FILE[400];
char DIR_NAME[400];
//int MAX_ITER = INT_MAX;
int MAX_ITER = 1000;
int DUMP_FREQ = 1;

int WIDTH;
int HEIGHT;

int INPUT_FLAG = 0;
int OUTPUT_FLAG = 0;


void printError(char *errorMessage) {
	printf("Error: %s!", errorMessage);
	exit(1);
}

int checkCommand(char *command) {
	if (strcmp(command, "--input") == 0) return 1;
	if (strcmp(command, "--output") == 0) return 2;
	if (strcmp(command, "--max_iter") == 0) return 3;
	if (strcmp(command, "--dump_freq") == 0) return 4;
	return 0;
}

void parseCommand(int argc, char **argv) {
	if (argc < 5 || argc % 2 == 0) printError("Incorrect number of arguments");
	
	int commandType;
	for (int i = 1; i < argc - 1; i += 2) {
		commandType = checkCommand(argv[i]);
		if (commandType == 0) printError("Incorrect command");
		switch (commandType) {
			case 1:
				INPUT_FLAG = 1;
				strcpy(INPUT_FILE, argv[i + 1]);
				break;
			case 2:
				OUTPUT_FLAG = 1;
				strcpy(DIR_NAME, argv[i + 1]);
				break;	
			case 3:
				MAX_ITER = atoi(argv[i + 1]);
				break;
			case 4:
				DUMP_FREQ = atoi(argv[i + 1]);
				break;	
		}
	}
	
	if (INPUT_FLAG == 0 || OUTPUT_FLAG == 0) printError("No input or output were provided");
}


int mod(int number, int modValue) {
	int result = number % modValue;
	if (result >= 0) return result;
	return modValue + result;
}

char changeCell(int x, int y, char **image) {
	char cell = image[y][x];
	char moore[8];
	moore[0] = image[mod(y - 1, HEIGHT)][mod(x, WIDTH)];
	moore[1] = image[mod(y - 1, HEIGHT)][mod(x + 1, WIDTH)];
	moore[2] = image[mod(y, HEIGHT)][mod(x + 1, WIDTH)];
	moore[3] = image[mod(y + 1, HEIGHT)][mod(x + 1, WIDTH)];
	moore[4] = image[mod(y + 1, HEIGHT)][mod(x, WIDTH)];
	moore[5] = image[mod(y + 1, HEIGHT)][mod(x - 1, WIDTH)];
	moore[6] = image[mod(y, HEIGHT)][mod(x - 1, WIDTH)];
	moore[7] = image[mod(y - 1, HEIGHT)][mod(x - 1, WIDTH)];

	char nbrCount = 0;
	for (int i = 0; i < 8; i++) nbrCount += moore[i];
	
	if (cell) {
		
		if (nbrCount == 2 || nbrCount == 3) return 1;
		return 0;
		
	} else {
		if (nbrCount == 3) return 1;
		return 0;
	}
}


void changeGeneration(char **image, char **bufferImage) {
	
	// change generation
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			bufferImage[row][col] = changeCell(col, row, image);
		}
	}
	
	// write back to image
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			image[row][col] = bufferImage[row][col];
		}
	}	
}

void saveGeneration(unsigned char *header, char **image, int iter) {
	char generationName[400];
	strcpy(generationName, "./");
	strcat(generationName, DIR_NAME);
	strcat(generationName, "/");
	char iterNumber[50];
	itoa(iter, iterNumber, 10);
	strncat(generationName, INPUT_FILE, strlen(INPUT_FILE) - 4);
	strcat(generationName, "_");
	strcat(generationName, iterNumber);
	strcat(generationName, ".bmp");
	
	FILE *generationFile = fopen(generationName, "wb");
	
	fwrite(header, 1, 54, generationFile);
	
	char currentDot;
	unsigned char currentRGB[3];
	for (int row = HEIGHT - 1; row >= 0; row--) {
		for (int col = 0; col < WIDTH; col++) {
			currentDot = image[row][col];
			
			if (currentDot) {
				currentRGB[0] = 0x00;
				currentRGB[1] = 0x00;
				currentRGB[2] = 0x00;
			} else {
				currentRGB[0] = 0xFF;
				currentRGB[1] = 0xFF;
				currentRGB[2] = 0xFF;
			}
			
			fwrite(currentRGB, 1, 3, generationFile);
		}
	}
	
	fclose(generationFile);
}


int main(int argc, char **argv) {
	parseCommand(argc, argv);
	
	FILE *file = NULL;
	file = fopen(INPUT_FILE, "rb");
	if (file == NULL) printError("File not found");

	
	// >>> INITIAL GENERATION
	// reading the header
	unsigned char header[54];
	fseek(file, 0, SEEK_SET);
	fread(header, 1, 54, file);
	WIDTH = header[18] + 16 * header[19] + 16 * 16 * header[20] + 16 * 16 * 16 * header[21];
	HEIGHT = header[22] + 16 * header[23] + 16 * 16 * header[24] + 16 * 16 * 16 * header[25];
	
	// malloc memory
	char **image = (char **) malloc(sizeof(char *) * HEIGHT);
	for (int i = 0; i < HEIGHT; i++) image[i] = (char *) malloc(sizeof(char) * WIDTH);
	char **bufferImage = (char **) malloc(sizeof(char *) * HEIGHT);
	for (int i = 0; i < HEIGHT; i++) bufferImage[i] = (char *) malloc(sizeof(char) * WIDTH);	
	
	
	// fill image
	unsigned char currentRGB[3];
	for (int row = HEIGHT - 1; row >= 0; row--) {
		for (int col = 0; col < WIDTH; col++) {
			fread(currentRGB, 1, 3, file);
			if (currentRGB[0] == 0xFF && currentRGB[1] == 0xFF && currentRGB[2] == 0xFF) {
				image[row][col] = 0;
			} else {
				image[row][col] = 1;
			}
		}
	}
	
	mkdir(DIR_NAME);	
	
	for (int iter = 1; iter <= MAX_ITER; iter++) {
		changeGeneration(image, bufferImage);
		if (iter % DUMP_FREQ == 0) {
			saveGeneration(header, image, iter);
		}
	}
	
	free(image);
	free(bufferImage);
	fclose(file);
	
	printf("DONE!\n\n\n");	
	
	return 0;
}
