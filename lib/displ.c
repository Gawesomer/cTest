#include <stdint.h>
#include <stdio.h>

#include "displ.h"

/* Print integer to specified stream
 * params:
 * 	- stream: stream to print to. defaults to STDOUT if NULL
 * 	- pe: integer pointer
 * returns:
 * 	nothing
 */
void int_displ(FILE *stream, const void *pe)
{
	const int *e;

	if (!stream)
		stream = stdout;

	if (!pe) {
		fprintf(stream, "NULL");
		return;
	}

	e = (int *)pe;
	fprintf(stream, "%d", *e);
}

/* Print byte to specified stream
 * params:
 * 	- stream: stream to print to. defaults to STDOUT if NULL
 * 	- pe: uint8_t pointer
 * returns:
 * 	nothing
 */
void byte_displ(FILE *stream, const void *pe)
{
	const uint8_t *e;

	if (!stream)
		stream = stdout;

	if (!pe) {
		fprintf(stream, "NULL");
		return;
	}

	e = (uint8_t *)pe;
	fprintf(stream, "%x", *e);
}

/* Print float to specified stream
 * params:
 * 	- stream: stream to print to. defaults to STDOUT if NULL
 * 	- pe: float pointer
 * returns:
 * 	nothing
 */
void float_displ(FILE *stream, const void *pe)
{
	const float *e;

	if (!stream)
		stream = stdout;

	if (!pe) {
		fprintf(stream, "NULL");
		return;
	}

	e = (float *)pe;
	fprintf(stream, "%f", *e);
}

/* Print array to specified stream
 * params:
 * 	- stream: stream to print to. defaults to STDOUT if NULL
 * 	- arr: array to print
 * 	- nel: number of elements in `arr`
 * 	- width: size of an element of `arr`
 * 	- display: display function to print the type of data in `arr`
 * returns:
 * 	nothing
 * notes:
 * 	assumes that `arr` is at least of size `nel`;
 * 	in the case where it is shorter, it will cause a read out of bounds
 */
void arr_displ(FILE *stream, const void *arr, size_t nel, size_t width,
		void (*display)(FILE *, const void *))
{
	char *p;
	int i;

	if (!stream)
		stream = stdout;

	if (!arr) {
		fprintf(stream, "NULL");
		return;
	}

	p = (char *)arr;
	i = 0;
	fprintf(stream, "[");
	while ((size_t)i < nel) {
		(*display)(stream, p);
		fprintf(stream, ((size_t)i == nel-1) ? "": ", ");
		p += width;
		i++;
	}
	fprintf(stream, "]");
}
