#ifndef PROPS_H
#define PROPS_H

struct tdna {
	float * dna;
	int     total;
	int     tail;
};
void tdna_free (struct tdna * parameter);
void tdna_reset (struct tdna * parameter, int off);
float tdna_next (struct tdna * parameter);
int tdna_next_i (struct tdna * parameter);
int tdna_load (struct tdna * parameter, FILE * file);


#endif
