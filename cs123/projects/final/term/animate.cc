#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
/**
 * Generates a series of numbers with the specified std.dev and mean.
 * It is only an approximation, however.
 **/
int main (int c, char ** args) {
	if ( c < 4 ) {
		fprintf (stderr,"%s run_length extreme angle_step\n", args[0]);
		return -1;
	}
	int run_length = strtol(args[1],NULL,10);
	int extreme = strtol(args[2], NULL, 10);
	float angle_step = strtof(args[3], NULL);
	
	srand (time(NULL));
	
	printf ("%d\n", run_length * extreme * 2);
	for (int i = 0; i < extreme; i++) {
		for (int j = 0; j < run_length; j++) {
			printf ("%f\n", i*angle_step);
		}
	}
	for (int i = 0; i < extreme; i++) {
		for (int j = 0; j < run_length; j++) {
			printf ("%f\n", (extreme-i-1)*angle_step);
		}
	}
}
