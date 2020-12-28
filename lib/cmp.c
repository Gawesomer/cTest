#include <float.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#include "cmp.h"

/* Check that pointers given are not NULL
 * params:
 * 	- pa: void pointer
 * 	- pb: void pointer
 * returns:
 * 	0 if neither `pa` nor `pb` are NULL,
 * 	1 if both `pa` and `pb` are NULL,
 * 	-1 if only `pa` is NULL,
 * 	2 if only `pb` is NULL
 * notes:
 * 	intended to remove code duplication from comparator functions.
 * 	should be used as follows at the start of a comparator function in
 * 	order to check for NULL-pointers:
 * 		int c;
 * 		if ((c = check_null(pa, pb)) != 0)
 * 			return c-1;
 */
int check_null(const void *pa, const void *pb)
{
	if (!pa && !pb)
		return 1;
	else if (!pa)
		return -1;
	else if (!pb)
		return 2;
	return 0;
}

/* Compare two integers
 * params:
 * 	- pa: integer pointer
 * 	- pb: integer pointer
 * returns:
 * 	integer greater than, equal to, or less than 0, accordingly as the
 * 	integer pointed to by `pa` is greater than, equal to, or less than the
 * 	integer pointed to by `pb`
 * notes:
 * 	a NULL-pointer will be considered as the smallest element
 */
int intcmp(const void *pa, const void *pb)
{
	int c;
	const int *a;
	const int *b;

	if ((c = check_null(pa, pb)) != 0)
		return c-1;

	a = (int *)pa;
	b = (int *)pb;
	if (*a == *b)
		return 0;
	if (*a < *b)
		return -1;
	return 1;
}

/* Compare two bytes
 * params:
 * 	- pa: uint8_t pointer
 * 	- pb: uint8_t pointer
 * returns:
 * 	integer greater than, equal to, or less than 0, accordingly as the
 * 	byte pointed to by `pa` is greater than, equal to, or less than the
 * 	byte pointed to by `pb`
 * notes:
 * 	a NULL-pointer will be considered as the smallest element
 */
int bytecmp(const void *pa, const void *pb)
{
	int c;
	const uint8_t *a;
	const uint8_t *b;

	if ((c = check_null(pa, pb)) != 0)
		return c-1;

	a = (uint8_t *)pa;
	b = (uint8_t *)pb;
	if (*a == *b)
		return 0;
	if (*a < *b)
		return -1;
	return 1;
}

/* Type pun a float to an integer
 * params:
 * 	- f: float
 * returns:
 * 	int32_t filled with binary data from `f`
 */
int32_t ftoi_punning(float f)
{
	int32_t e;
	memcpy(&e, &f, sizeof(float));
	return e;
}

/* Type pun an integer to a float
 * params:
 * 	- e: int32_t
 * returns:
 * 	float filled with binary data from `e`
 */
float itof_punning(int32_t e)
{
	float f;
	memcpy(&f, &e, sizeof(int32_t));
	return f;
}

/* Compute the distance in number of floating point values
 * (Units of Least Precision) between two floats
 * params:
 * 	- a: float
 * 	- b: float
 * returns:
 * 	distance between `a` and `b` in ULPs
 */
int32_t ulpdist(float a, float b)
{
	int32_t ia, ib;
	int32_t dist;

	if (a == b)
		return 0;
	if ((a < 0.0f) != (b < 0.0f))
		return INT_MAX;

	ia = ftoi_punning(a);
	ib = ftoi_punning(b);

	dist = ia - ib;
	if (dist < 0)
		dist *= -1;
	return dist;
}

/* Check if two floats are equal within FLT_FIXED_EPS or FLT_ULP_EPS
 * params:
 * 	- a: float
 * 	- b: float
 * returns:
 * 	0 if the absolute difference between `a` and `b` is less than or
 * 	equal to FLT_FIXED_EPS, or if the distance in ULPs between `a` and `b`
 * 	is less than FLT_ULP_EPS, nonzero otherwise
 * credits:
 * 	implementation for `float_eq` as well as helper functions was taken
 * 	from Matt Kline's paper "Comparing Floating-Point Numbers Is Tricky":
 * 	https://bitbashing.io/comparing-floats.html
 */
int float_eq(float a, float b)
{
	float difference = fabs(a - b);
	if (difference <= FLT_FIXED_EPS)
		return 0;
	if ((a < 0.0f) != (b < 0.0f))
		return 1;
	if (ulpdist(a, b) <= FLT_ULP_EPS)
		return 0;
	return 1;
}

/* Compare two floats
 * params:
 * 	- pa: float pointer
 * 	- pb: float pointer
 * returns:
 * 	integer greater than, equal to, or less than 0, accordingly as the
 * 	float pointed to by `pa` is greater than, equal to, or less than the
 * 	float pointed to by `pb`
 * notes:
 * 	a NULL-pointer will be considered as the smallest element
 */
int floatcmp(const void *pa, const void *pb)
{
	int c;
	const float *a;
	const float *b;

	if ((c = check_null(pa, pb)) != 0)
		return c-1;

	a = (float *)pa;
	b = (float *)pb;
	if (float_eq(*a, *b) == 0)
		return 0;
	if (*a < *b)
		return -1;
	return 1;
}

/* Compare two arrays
 * params:
 * 	- a_arr: array
 * 	- b_arr: array
 * 	- nel: number of elements in `a_arr` and `b_arr`
 * 	- width: size of an element of the given arrays
 * 	- compare: comparison function for the type of elements in the arrays
 * returns:
 * 	0 if all elements of `a_arr` and `b_arr` are equal, non-zero otherwise
 * notes:
 * 	assumes that both `a_arr` and `b_arr` are of size `nel`;
 * 	in the case where one is shorter, it will cause a read out of bounds,
 * 	in the case where one is longer, it will fail to notice it, possibly
 * 	resulting in false positives
 */
int arrcmp(const void *a_arr, const void *b_arr, size_t nel, size_t width, \
		int (*compare)(const void *, const void *))
{
	char *pa, *pb;
	int i, c;

	if ((c = check_null(a_arr, b_arr)) != 0)
		return c-1;

	pa = (char *)a_arr;
	pb = (char *)b_arr;
	i = 0;
	while ((size_t)i < nel) {
		if ((*compare)((void *)pa, (void *)pb) != 0)
			return 1;
		pa += width;
		pb += width;
		i++;
	}

	return 0;
}
