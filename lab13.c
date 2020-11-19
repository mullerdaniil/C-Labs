#include <stdio.h>
#include <string.h>

unsigned int FRAME_LENGTH_LIMITER = 50;

typedef enum {
	SHOW,
	SET,
	GET
} Command;


union {
	char data[10];
	struct {
		char fileID[3];
		char version[2];
		unsigned char flags;
		unsigned char size[4];
	} fields;
} HEADER;

typedef union {
	char data[10];
	struct {
		char frameID[4];
		unsigned char size[4];
		unsigned short flags;
	} fields;
} Frame;


char fileName[400];
char propName[400];
char propValue[400];
FILE *file;


void copyFromFile(FILE *source) {
	int symbol;
	while ((symbol = getc(source)) != EOF) putc(symbol, file);
}

int checkFrameID(char *frameID) {
	for (int i = 0; i < 4; i++) {
		if (!(frameID[i] >= '0' && frameID[i] <= '9' || frameID[i] >= 'A' && frameID[i] <= 'Z')) return 0;
	}
	return 1;
}

unsigned int charsToInt(char *chars) {
	return chars[3]
		+ ((unsigned int) chars[2] << 7)
		+ ((unsigned int) chars[1] << 14)
		+ ((unsigned int) chars[0] << 21);
					
}

void intToChars(unsigned int number, char *chars) {
	chars[0] = number >> 21;
	chars[1] = number >> 14;
	chars[2] = number >> 7;
	chars[3] = number;
}

void printError(char *errorMessage) {
	printf("Error: %s!\n", errorMessage);
	exit(1);
}

int parseCommand(int argv, char **argc) {
	
	if (argv < 3 || argv > 4) printError("Incorrect number of arguments");
	
	char fpstr[] = "--filepath=";
	if (strncmp(argc[1], fpstr, strlen(fpstr)) != 0) printError("Incorrect command");
	strcpy(fileName, argc[1] + strlen(fpstr));
	

	if (strcmp(argc[2], "--show") == 0) return SHOW;
	if (strncmp(argc[2], "--set=", strlen("--set=")) == 0) {
		if (argv != 4 || strncmp(argc[3], "--value=", strlen("--value=")) != 0) printError("Incorrect command");
		strcpy(propName, argc[2] + strlen("--set="));
		strcpy(propValue, argc[3] + strlen("--value="));
		return SET;
	}
	if (strncmp(argc[2], "--get=", strlen("--get=")) == 0) {
		strcpy(propName, argc[2] + strlen("--get="));
		return GET;
	}
	
	printError("Incorrect command");
}

void show() {
	fseek(file, 0, SEEK_SET);
	fread(HEADER.data, 1, 10, file);
	unsigned int tagSize = charsToInt(HEADER.fields.size);
	if ((HEADER.fields.flags >> 6) & 1) fseek(file, SEEK_CUR, 10);
	
	printf("### HEADER INFO\n");
	printf("\tFile ID:  %.*s\n", 3, HEADER.fields.fileID);
	printf("\tVersion:  %d %d\n", HEADER.fields.version[0], HEADER.fields.version[1]);
	printf("\tTag size: %d bytes\n", tagSize + 10);
	printf("\n\n\n");
	
	Frame frame;
	unsigned int frameSize;
	while (ftell(file) < tagSize + 10) {
		fread(frame.data, 1, 10, file);
		frameSize = charsToInt(frame.fields.size);
		if (!checkFrameID(frame.fields.frameID) || frameSize == 0) break;
		
		printf("# %.*s (%d bytes) \n", 4, frame.fields.frameID, frameSize);
		
		for (unsigned int i = 0; i < frameSize; i++) {
			if (i > FRAME_LENGTH_LIMITER) {
				getc(file);
			} else {		
				printf("%c", getc(file));
			}
		}
		
		if (frameSize > FRAME_LENGTH_LIMITER) printf("\n...");
		
		printf("\n\n\n");	
	}
}

void set() {
	printf("...setting...\n");
	fseek(file, 0, SEEK_SET);
	fread(HEADER.data, 1, 10, file);
	unsigned int tagSize = charsToInt(HEADER.fields.size);
	if ((HEADER.fields.flags >> 6) & 1) fseek(file, SEEK_CUR, 10);
	
	int frameNameFound = 0;
		
	Frame frame;
	unsigned int frameSize;
	while (ftell(file) < tagSize + 10) {
		fread(frame.data, 1, 10, file);
		frameSize = charsToInt(frame.fields.size);
		
		if (strcmp(frame.fields.frameID, propName) == 0) {
			frameNameFound = 1;
			break;
		}

		if (!checkFrameID(frame.fields.frameID)) break;
		fseek(file, frameSize, SEEK_CUR);
	}
	
	
	unsigned int propValueLength = strlen(propValue);
	
	
	if (frameNameFound == 0)
		frameSize = propValueLength;
	
	
	
	// text field case (T...)
	if (frame.fields.frameID[0] == 'T') {
		char bufferPropValue[400];
		char *bufferPointer = bufferPropValue;
		char *valuePointer = propValue;
		
		// UTF-16 LE BOM
		*(bufferPointer++) = 0X01;
		*(bufferPointer++) = 0XFF;
		*(bufferPointer++) = 0XFE;
		
		// creating new value
		while (*valuePointer != '\0') {
			*(bufferPointer++) = *valuePointer;
			*(bufferPointer++) = 0X00;
			valuePointer++;
		}
		
		// copying back
		
		propValueLength = 3 + 2 * strlen(propValue);
		bufferPointer = bufferPropValue;
		valuePointer = propValue;
		
		for (int i = 0; i < propValueLength; i++) {
			*(valuePointer++) = *(bufferPointer++);
		}
		
	}
	
	
	if (frameNameFound) {
		if (strlen(propValue) == 0) {
			tagSize -= frameSize - 10;
		} else {
			tagSize += propValueLength - frameSize;
		}
	} else {
		if (propValueLength != 0) {
			tagSize += 10 + propValueLength;
		}
	}
	
	intToChars(tagSize, HEADER.fields.size);
	intToChars(propValueLength, frame.fields.size);
	strcpy(frame.fields.frameID, propName);
	int frameStart = ftell(file);
	if (frameNameFound) frameStart -= 10;
	
	// writing to temporary file
	FILE *temporaryFile = fopen("tempFile.mp3", "wb");
	
	//writing HEADER
	for (int i = 0; i < 10; i++) putc(HEADER.data[i], temporaryFile);
	fseek(file, 10, SEEK_SET);
	fseek(temporaryFile, 10, SEEK_SET);
	
	// writing (HEADER; frameStart)
	while (ftell(file) < frameStart) putc(getc(file), temporaryFile);
	
	if (!frameNameFound) fseek(temporaryFile, -10, SEEK_CUR);
	
	// writing frame header
	if(propValueLength >= 1) for (int i = 0; i < 10; i++) putc(frame.data[i], temporaryFile);
	
	
	
	// writing frame value
	for (unsigned int i = 0; i < propValueLength; i++) {
		putc(propValue[i], temporaryFile);
	}
	
	// writing rest
	fseek(file, frameSize + 10, SEEK_CUR);
	int symbol;	
	while ((symbol = getc(file)) != EOF) putc(symbol, temporaryFile);

	// copy from temporaryFile
	fclose(file);
	fclose(temporaryFile);
	
	file = fopen(fileName, "wb");
	temporaryFile = fopen("tempFile.mp3", "rb");
	fseek(file, 0, SEEK_SET);
	fseek(temporaryFile, 0, SEEK_SET);
	
	while ((symbol = getc(temporaryFile)) != EOF) putc(symbol, file);
	fclose(temporaryFile);
	remove("tempFile.mp3");
}


void get() {
	fseek(file, 0, SEEK_SET);
	fread(HEADER.data, 1, 10, file);
	unsigned int tagSize = charsToInt(HEADER.fields.size);
	if ((HEADER.fields.flags >> 6) & 1) fseek(file, SEEK_CUR, 10);
	
	Frame frame;
	unsigned int frameSize;
	while (ftell(file) < tagSize + 10) {
		fread(frame.data, 1, 10, file);
		frameSize = charsToInt(frame.fields.size);
		if (!checkFrameID(frame.fields.frameID) || frameSize == 0) break;
		
		if (strcmp(frame.fields.frameID, propName) == 0) {
			printf("%s = ", propName);
			for (unsigned int i = 0; i < frameSize; i++) {
				printf("%c", getc(file));
			}
			printf("\n\n");
			return;
		}
		
		for (unsigned int i = 0; i < frameSize; i++) {
			getc(file);
		}	
	}
	
	printf("No such property found!\n\n");
}



void executeCommand(Command command, char *filePath) {
	
	file = fopen(filePath, "rb");
	if (file == NULL) printError("Unable to access the file");
	
	
	switch (command) {
		case SHOW:
			show();
			break;
		
		case SET:
			set();
			break;
		
		case GET:
			get();
			break;
	}
	
	fclose(file);
}



int main(int argv, char **argc) {
	
	Command command = parseCommand(argv, argc);
	executeCommand(command, fileName);

	return 0;
}
