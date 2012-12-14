#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "props.hh"

void tdna_free (struct tdna * parameter) {
	if (parameter->dna)
		free(parameter->dna);
}
void tdna_reset (struct tdna * parameter, int off) {
	parameter->tail = off % parameter->total;
}
float tdna_next (struct tdna * parameter) {
	float curr = parameter->dna[parameter->tail];
	parameter->tail = (parameter->tail + 1) % parameter->total;
	return curr;
}

int tdna_next_i (struct tdna * parameter) {
	float val = tdna_next(parameter);
	return ((int) (val + 0.5));
}
int tdna_load (struct tdna * parameter, FILE * file) {
	if (fscanf (file, "%d", &parameter->total) != 1)
		return 0;
	parameter->dna = (float *)malloc (sizeof(float) * parameter->total);
	
	if (parameter->dna == NULL)
		return 0;
	parameter->tail = 0;
	
	for (int i = 0; i < parameter->total; i ++) {
		if (fscanf(file,"%f",&parameter->dna[i])!=1) {
			tdna_free(parameter);
		}
	}
	return 1;
}
