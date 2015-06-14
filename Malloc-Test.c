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
	char *a;
	char *b;


	printf("Hallo, C!\n");

	testint = (int *)malloc(sizeof(int));
	*testint = 315;
	printf("Wert: %i\n", *testint);

	gc_autoreleasepool( //Create a new gc_autoreleasepool
		gc_autoreleasepool(
			for (i=0; i<=10; i++) {
				malloc(sizeof(double));
			}
			free(testint);
		)
		doub = (double *)malloc(sizeof(double));
		free(doub);
		doub = (double *)malloc(sizeof(double));
		free(doub);
		
		s = "MONA LISA";
		copied = strdup(s);
		printf("Der Name: %s\n", copied);
	)
	
	printf("Enter.");
	getchar();

	gc_autoreleasepool(
		bad_function();
		for (i=0; i<=10; i++) {
			for (j=0; j<=3; j++) {
				malloc(sizeof(double));
			}
		}
		printf("Enter1.");
		getchar();
		bad_function();
	)

	gc_autoreleasepool(
		printf("this is my empty autoreleasepool :)\n");
	)
	
	printf("Enter3.");
	getchar();
	return EXIT_SUCCESS;
}

void bad_function(void) {
    float *a = (float *)malloc(sizeof(float) * 45);
	return;
}

