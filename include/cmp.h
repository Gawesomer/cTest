#ifndef _CMP_H_
#define _CMP_H_

/* Epsilons used for determining single precision float equality
 * `float_cmp` first uses FLT_FIXED_EPS to determine equality, if that fails,
 * the relative measure of distance in Units of Least Precision is used instead
 */
#define FLT_FIXED_EPS	0.1f
#define FLT_ULP_EPS	3

int intcmp(const void *pa, const void *pb);

int bytecmp(const void *pa, const void *pb);

int float_eq(float a, float b);

int floatcmp(const void *pa, const void *pb);

int arrcmp(const void *a_arr, const void *b_arr, size_t nel, size_t width, \
		int (*compare)(const void *, const void *));

#endif
