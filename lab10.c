#include <stdio.h>

// greatest common divisor
long long gcd(long long a, long long b) {
	long long c = a % b;
	while (c > 0) {
		a = b;
		b = c;
		c = a % b;
	}
	return b;
}

// least common multiple
long long lcm(long long a, long long b) {
	return a * b / gcd(a, b);
}

// numberToArray
int * numberToArray(long long n, int *arraySizePointer) {
	int digits[50];
	int digitCount = 0;
	
	while (n > 0) {
		digits[digitCount] = n % 10;
		n /= 10;
		digitCount++;
	}
	
	*arraySizePointer = digitCount;
	int *resultArray = (int *) malloc(digitCount * sizeof(int));
	for (int index = 0; index < digitCount; index++) {
		resultArray[index] = digits[index];
	}
	
	return resultArray;
}


int main(int argv, char **argc) {
	
	// testing task #1
	long long a, b;
	printf("Enter two numbers: \n");
	scanf("%lld %lld", &a, &b);
	printf("\n");
	printf("The greatest common divisor is %lld\n", gcd(a, b));
	printf("The least common multiple is %lld\n\n\n", lcm(a, b));
	
	
	
	// testing task #3
	long long n;
	printf("Enter n: \n");
	scanf("%lld", &n);
	printf("\n");
	
	int arraySize;
	int *array = numberToArray(n, &arraySize);
	printf("The array containing digits of n: ");
	for (int i = 0; i < arraySize; i++) {
		printf("%d ", array[i]);
	}
	free(array);
	
	
	return 0;
}
