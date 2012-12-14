#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
/**
 * Generate a random number with a std.dev of 1 and mean of 0.
 * 
 * http://c-faq.com/lib/gaussian.html
 **/
double gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;
	

	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
			} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	return X;
}
/**
 * Generates a series of numbers with the specified std.dev and mean.
 * It is only an approximation, however.
 **/
int main (int c, char ** args) {
	if ( c < 4 ) {
		fprintf (stderr,"%s samples mean std.dev\n", args[0]);
		return -1;
	}
	int count = strtol(args[1],NULL,10);
	float mean = strtof(args[2], NULL);
	float stddev = strtof(args[3], NULL);
	
	srand (time(NULL));
	
	printf ("%d\n",count);
	for (int i = 0; i < count; i++) {
		printf ("%f\n",(gaussrand() * stddev) + mean );
	}
}
