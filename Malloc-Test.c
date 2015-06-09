#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C-Autoreleasepool.h"

void bad_function(void);

int main(void) {
	int *testint;
	const char *s;
	char *copied;
	double *doub;
	int i;
	int j;


	printf("Hallo, C!\n");

	testint = (int *)malloc(sizeof(int));
	*testint = 315;
	printf("Wert: %i\n", *testint);

	gc_autoreleasepool( //Create a new gc_autoreleasepool
		for (i=0; i<=100; i++) {
			malloc(sizeof(double));
		}
		doub = (double *)malloc(sizeof(double));
		free(doub);
		doub = (double *)malloc(sizeof(double));
		free(doub);
		
		s = "MONA LISA";
		copied = strdup(s);
		printf("Der Name: %s\n", copied);

		free(testint);
	)
	
	printf("Enter.");
	getchar();

	gc_autoreleasepool(
		bad_function();
		for (i=0; i<=100; i++) {
			for (j=0; j<=10; j++) {
				malloc(sizeof(double));
			}
		}
		printf("Enter1.");
		getchar();
		bad_function();
	)

	gc_autoreleasepool(
		printf("this is an Empty autoreleasepool.\n");
	)
	
	printf("Enter3.");
	getchar();
	return EXIT_SUCCESS;
}

void bad_function(void) {
    float *a = (float *)malloc(sizeof(float) * 45);
	return;
}

