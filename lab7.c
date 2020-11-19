#include <stdio.h>

#define PI 3.14159265358979323846

struct Circle {
	float centerXCoordinate, centerYCoordinate, radius;
};

float circleSquare(struct Circle circle) {
	return PI * circle.radius * circle.radius;
}

int main(int argv, char **argc) {
	
	enum PrintEdition {
		BOOK,
		MAGAZINE,
		NEWSPAPER,
		CALENDAR,
		ALBUM
	};
	
	printf("NEWSPAPER value is %d\n\n", NEWSPAPER);
	
	
	struct Circle circle;
	circle.centerXCoordinate = 16;
	circle.centerYCoordinate = 23.4;
	circle.radius = 1;
	
	printf("The square of the circle is %f\n\n", circleSquare(circle));
	
	
	union Keyboard {
		struct BitField {
			unsigned char NumLock : 1;
			unsigned char CapsLock : 1;
			unsigned char ScrollLock : 1;
		} bitField;
		unsigned char state;
	};
	
	union Keyboard keyboard;
	
	printf("Enter the hexadecimal number: \n");
	scanf("%x", &keyboard.commonState);
	
	char *states[2] = {"OFF", "ON"};
	printf("States:\n");
	printf("NumLock is %s\n", states[keyboard.bitField.NumLock]);
	printf("CapsLock is %s\n", states[keyboard.bitField.CapsLock]);
	printf("ScrollLock is %s\n", states[keyboard.bitField.ScrollLock]);
	
	return 0;
}
