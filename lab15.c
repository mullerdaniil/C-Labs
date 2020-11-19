#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct Node {
	struct Node *left, *right;
	unsigned char byte;
	int frequency;
} Node;


void treeTraversal(Node *node, unsigned char codeMap[], char *codes, int *lastIndex, char currentString[]) {
	if (node->left == NULL && node->right == NULL) {
		
		codeMap[*lastIndex] = node->byte;
		char *stringPointer = codes + *lastIndex * 400;
		char *currentStringPointer = currentString;
		
		int stringLength = strlen(currentString);
		for (int i = 0; i < stringLength; i++) {
			(*stringPointer) = (*currentStringPointer);
			stringPointer++;
			currentStringPointer++;
		}
		
		(*stringPointer) = '\0';
		
		(*lastIndex)++;
		return;
	}
	
	char currentStringLeft[400];
	char currentStringRight[400];
	strcpy(currentStringLeft, currentString);
	strcpy(currentStringRight, currentString);
	strcat(currentStringLeft, "0");
	strcat(currentStringRight, "1");
	
	treeTraversal(node->left, codeMap, codes, lastIndex, currentStringLeft);
	treeTraversal(node->right, codeMap, codes, lastIndex, currentStringRight);
}

void addStringToBuffer(unsigned char buffer[], int *bufferPivot, char string[]) {
	char *stringPivot = string;
	int stringLength = strlen(string);
	
	
	for (int i = 0; i < stringLength; i++) {
		if (string[i] == '0') {
			buffer[*bufferPivot / 8] &= ~(1 << (7 - (*bufferPivot % 8)));
		} else {
			buffer[*bufferPivot / 8] |= (1 << (7 - (*bufferPivot % 8)));
		}
		(*bufferPivot)++;
		stringPivot++;
	}
}


void printError(char *errorMessage) {
	printf("Error: %s!", errorMessage);
	exit(1);
}

char getBit(unsigned char byte, char i) {
	return '0' + ((byte >> (7 - i)) & 1);
}


void create(char *archiveName, int argc, char **argv) {
	FILE *currentFile;

	// check for files
	for (int fileIndex = 0; fileIndex < argc - 4; fileIndex++) {
		currentFile = NULL;
		currentFile = fopen(argv[fileIndex + 4], "rb");
		if (currentFile == NULL) {
			printf("File  %s  not found!\n",argv[fileIndex + 4]);
			printError("FILE NOT FOUND");
		}
		fclose(currentFile);
	}
	
	
	int fileSizes[argc - 4];
	
	unsigned char currentByte;
	int c;
	int nodeFound;
	
	// initialize the array of byte frequencies and fill it with zeros
	short nodesNumber = 0;
	Node * nodes[512];
	int active[512];
	for (int i = 0; i < 512; i++) active[i] = 0;

	// reading every file
	for (int currentFileIndex = 0; currentFileIndex < argc - 4; currentFileIndex++) {
		
		// opening the file
		currentFile = NULL;
		currentFile = fopen(argv[currentFileIndex + 4], "rb");
		
		fileSizes[currentFileIndex] = 0;
		
		// creating the initial set of nodes
		while ((c = getc(currentFile)) != EOF) {
			
			fileSizes[currentFileIndex]++;
			currentByte = c;
			nodeFound = 0;
		
			for (int i = 0; i < nodesNumber; i++) {
				if (nodes[i]->byte == currentByte) {
					nodes[i]->frequency++;
					nodeFound = 1;
					break;
				}
			}
		
			if (!nodeFound) {
				nodes[nodesNumber] = malloc(sizeof(Node));
				active[nodesNumber] = 1;
				nodes[nodesNumber]->byte = currentByte;
				nodes[nodesNumber]->frequency = 1;
				nodes[nodesNumber]->left = NULL;
				nodes[nodesNumber]->right = NULL;
				nodesNumber++;
			}
		}
		
		fclose(currentFile);
	}
	

	// BUILDING HUFFMAN CODE

	int lastIndex = nodesNumber;
	Node *firstMin, *secondMin;
	int firstMinValue, secondMinValue;
	int firstMinIndex, secondMinIndex;
	
	for (int iter = 0; iter < nodesNumber - 1; iter++) {
		firstMinValue = INT_MAX;
		secondMinValue = INT_MAX;
		
		// find the 1st min
		for (int node = 0; node < lastIndex; node++) {
			if (active[node] && nodes[node] != NULL && nodes[node]->frequency < firstMinValue) {
				firstMinValue = nodes[node]->frequency;
				firstMin = nodes[node];
				firstMinIndex = node;
			}
		}
		
		// find the 2nd min
		for (int node = 0; node < lastIndex; node++) {
			if (active[node] && nodes[node] != NULL && nodes[node]->frequency < secondMinValue && firstMinIndex != node) {
				secondMinValue = nodes[node]->frequency;
				secondMin = nodes[node];
				secondMinIndex = node;
			}
		}
		
		// create a new node and add it to the list of nodes
		active[firstMinIndex] = 0;
		active[secondMinIndex] = 0;
		nodes[lastIndex] = malloc(sizeof(Node));
		nodes[lastIndex]->left = firstMin;
		nodes[lastIndex]->right = secondMin;
		nodes[lastIndex]->frequency = nodes[firstMinIndex]->frequency + nodes[secondMinIndex]->frequency;
		active[lastIndex] = 1;
		lastIndex++;	
		
	}
	
	// find the root of the heap
	Node *root;
	for (int i = 0; i < lastIndex; i++) {
		if (active[i]) {
			root = nodes[i];
			break;
		}
	}
	
	int initialSize = 0;
	for (int i = 0; i < argc - 4; i++) initialSize += fileSizes[i];
	
	// tree traversal
	unsigned char codeMap[nodesNumber];
	int codeMapLastIndex = 0;
	char codeMapCodes[nodesNumber][400];
	char **somePointer = codeMapCodes;
	
	treeTraversal(root, codeMap, codeMapCodes, &codeMapLastIndex, "");
	char encodingTable[256][400] = {0};
	for (int i = 0; i < codeMapLastIndex; i++) strcpy(encodingTable[codeMap[i]], codeMapCodes[i]);
	
	// CREATING ARCHIVE
	
	FILE *archiveFile = fopen(archiveName, "wb");
	
	// archive name size
	short archiveFileNameSize = strlen(archiveName) + 1;
	fwrite(&archiveFileNameSize, 1, 2, archiveFile);
	
	// archive name
	fwrite(archiveName, 1, archiveFileNameSize, archiveFile);
	
	// number of files
	short numberOfFiles = argc - 4;
	fwrite(&numberOfFiles, sizeof(short), 1, archiveFile);
	
	// file name size, file name, file size
	short currentFileNameSize;
	for (int i = 0; i < numberOfFiles; i++) {
		currentFileNameSize = strlen(argv[i + 4]) + 1;
		fwrite(&currentFileNameSize, sizeof(short), 1, archiveFile);
		fwrite(argv[i + 4], 1, currentFileNameSize, archiveFile);
		fwrite(&fileSizes[i], 1, sizeof(int), archiveFile);
	}
	
	// codeMap size
	fwrite(&nodesNumber, 1, 2, archiveFile);
	
	// codeMap: ascii code (1 byte) - size of code (2 byte) - code (? bytes)
	short currentCodeSize;
	for (int i = 0; i < codeMapLastIndex; i++) {
		fwrite(&codeMap[i], 1, 1, archiveFile);
		currentCodeSize = strlen(codeMapCodes[i]) + 1;
		fwrite(&currentCodeSize, 1, 2, archiveFile);
		fwrite(codeMapCodes[i], 1, currentCodeSize, archiveFile);
	}
	
	// content
	unsigned char byteBuffer[20];
	int byteBufferPivot = 0;

	
	for (int currentFileIndex = 0; currentFileIndex < argc - 4; currentFileIndex++) {
		FILE *currentFile = fopen(argv[currentFileIndex + 4], "rb");
		while ((c = getc(currentFile)) != EOF) {
			currentByte = c;

			addStringToBuffer(byteBuffer, &byteBufferPivot, encodingTable[currentByte]);
		
			if (byteBufferPivot > 7) {
				for (int i = 0; i < byteBufferPivot / 8; i++) {
					putc(byteBuffer[i], archiveFile);
				}
				byteBuffer[0] = byteBuffer[byteBufferPivot / 8];
				byteBufferPivot %= 8;
			}	
		}
		
		fclose(currentFile);
		
	}
	
	putc(byteBuffer[0], archiveFile);
	char usefulBitsNumber = byteBufferPivot;
	putc(usefulBitsNumber, archiveFile);
	
	
	printf("Initial size:\t%d bytes \n", initialSize);
	printf("Final size:\t%d bytes \n", ftell(archiveFile));
	printf("Compression:\t%.2f%% \n", ftell(archiveFile) * 100 / (double) initialSize);
	
	/*
		>>> ARCHIVE STRUCTURE
		
			size of archive name (2 bytes)
			archive name (? bytes)
			
			number of files (2 bytes)
			
			File #1 name size (2 bytes)
			File #1 name (? bytes)
			File #1 size (4 bytes)
			
			File #2 name size (2 bytes)
			File #2 name (? bytes)
			File #2 size (4 bytes)
			
			...
			
			File #N name size (2 bytes)
			File #N name (? bytes)
			File #N size (4 bytes)

		
			codeMap size (2 bytes)
			
			codeMap (? bytes):
				ascii code (1 byte) - size of code (2 bytes) - code (? bytes)
				ascii code (1 byte) - size of code (2 bytes) - code (? bytes)
				...
				ascii code (1 byte) - size of code (2 bytes) - code (? bytes)
				
			archive content (? bytes)
			
	*/
	
	
	for (int i = 0; i < lastIndex; i++) free(nodes[i]);
	fclose(archiveFile);
	return 0;
}

int extract(char *fileName) {
	FILE *archiveFile = NULL;
	archiveFile = fopen(fileName, "rb");
	if (archiveFile == NULL) printError("ARCHIVE NOT FOUND");
	
	// archive size
	fseek(archiveFile, 0, SEEK_END);
	int archiveSize = ftell(archiveFile);
	fseek(archiveFile, 0, SEEK_SET);
	
	// archive name
	short archiveNameSize;
	fread(&archiveNameSize, sizeof(short), 1, archiveFile);
	fseek(archiveFile, archiveNameSize, SEEK_CUR);

	short numberOfFiles;
	fread(&numberOfFiles, sizeof(short), 1, archiveFile);
	
	// initializing file nameSize, name and size array
	short filesNameSize[numberOfFiles];
	char filesName[numberOfFiles][2000];
	int filesSize[numberOfFiles];
	
	// reading file nameSize, name and size array
	short currentFileNameSize;
	int currentFileSize;
	for (int currentFileIndex = 0; currentFileIndex < numberOfFiles; currentFileIndex++) {
		fread(&filesNameSize[currentFileIndex], sizeof(short), 1, archiveFile);
		fread(&filesName[currentFileIndex], 1, filesNameSize[currentFileIndex], archiveFile);
		fread(&filesSize[currentFileIndex], sizeof(int), 1, archiveFile);
	}
	
	
	for (int i = 0; i < numberOfFiles; i++) printf("%s   (%d bytes)  \n", filesName[i], filesSize[i]);
	
	
	// CODEMAP
	short codeMapSize;
	fread(&codeMapSize, sizeof(short), 1, archiveFile);
	char encodingTable[256][400];
	char active[256];
	for (int i = 0; i < 256; i++) active[i] = 0;
	
	unsigned char currentCodeByte;
	short currentCodeSize;
	
	for (int i = 0; i < codeMapSize; i++) {
		fread(&currentCodeByte, 1, 1, archiveFile);
		active[currentCodeByte] = 1;
		fread(&currentCodeSize, sizeof(short), 1, archiveFile);
		fread(encodingTable[currentCodeByte], 1, currentCodeSize, archiveFile);	
	}
	
	// DECODING
	unsigned char currentByte;
	char buffer[400];
	buffer[0] = '\0';
	int bufferPointer = 0;
	int byteCount = 0;
	int currentSymbol;
	
	FILE *currentFile;
	
	for (int fileIndex = 0; fileIndex < numberOfFiles; fileIndex++) {
		printf("...extracting  %s  \n", filesName[fileIndex]);
		currentFile = fopen(filesName[fileIndex], "wb");
		byteCount = 0;
		
		while (byteCount < filesSize[fileIndex]) {
			currentByte = getc(archiveFile);
			for (int bit = 0; bit < 8; bit++) {
				buffer[bufferPointer++] = getBit(currentByte, bit);
				buffer[bufferPointer] = '\0';
				
				if (fileIndex == numberOfFiles - 1 && byteCount == filesSize[fileIndex]) break;
				
				for (int code = 0; code < 256; code++) {

					if (active[code] && strcmp(buffer, encodingTable[code]) == 0) {
						
						putc(code, currentFile);
						bufferPointer = 0;
						buffer[bufferPointer] = '\0';
						byteCount++;
						break;
					}
				}	
			}		
		}
		fclose(currentFile);
	}

	fclose(archiveFile);
	
	printf("DONE!\n");
}

void list(char *fileName) {
	FILE *archiveFile = NULL;
	archiveFile = fopen(fileName, "rb");
	if (archiveFile == NULL) printError("ARCHIVE NOT FOUND");
	
	// archive size
	fseek(archiveFile, 0, SEEK_END);
	int archiveSize = ftell(archiveFile);
	fseek(archiveFile, 0, SEEK_SET);
	
	// archive name
	short archiveNameSize;
	fread(&archiveNameSize, sizeof(short), 1, archiveFile);
	char archiveName[archiveNameSize];
	fread(archiveName, 1, archiveNameSize, archiveFile);
	printf(". . . %s . . . \n\n", archiveName);

	short numberOfFiles;
	fread(&numberOfFiles, sizeof(short), 1, archiveFile);
	
	// initializing file nameSize, name and size array
	short filesNameSize[numberOfFiles];
	char filesName[numberOfFiles][2000];
	int filesSize[numberOfFiles];
	
	// reading file nameSize, name and size array
	short currentFileNameSize;
	int currentFileSize;
	for (int currentFileIndex = 0; currentFileIndex < numberOfFiles; currentFileIndex++) {
		fread(&filesNameSize[currentFileIndex], sizeof(short), 1, archiveFile);
		fread(&filesName[currentFileIndex], 1, filesNameSize[currentFileIndex], archiveFile);
		fread(&filesSize[currentFileIndex], sizeof(int), 1, archiveFile);
		
		printf("> %s (%d bytes)\n\n", filesName[currentFileIndex], filesSize[currentFileIndex]);
	}
	
}


void parseCommand(int argc, char **argv) {
	if (argc < 3) printError("Incorrect number of arguments");
	if (strcmp(argv[1], "--file") != 0) printError("Incorrect command");
	
	if (strcmp(argv[3], "--create") == 0) {
		create(argv[2], argc, argv);
		return;
	}
	if (strcmp(argv[3], "--extract") == 0) {
		extract(argv[2]);
		return;
	}
	if (strcmp(argv[3], "--list") == 0) {
		list(argv[2]);
		return;
	}
	
	printError("Incorrect command");
}


int main(int argc, char **argv) {
	
	parseCommand(argc, argv);
	
	
	return 0;
}
