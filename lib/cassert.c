#include <stdio.h>
#include <string.h>

#include "cassert.h"

int test_true(const char *macro, const char *f, int l, const char *fun, \
		const char *tk, int exp)
{
	if (exp)
		return 0;

	printf("==========================================================\n");
	printf("FAIL: %s\n", fun);
	printf("----------------------------------------------------------\n");
	printf("\tFile \"%s\", line %d:\n", f, l);
	printf("\t\t%s(%s)\n", macro, tk);
	printf("\t\t\t`%s` not true\n", tk);
	printf("----------------------------------------------------------\n");

	return 1;
}

int test_int_eq(const char *macro, const char *f, int l, const char *fun, \
		const char *a_tk, const char *b_tk, int a, int b)
{
	if (intcmp(&a, &b) == 0)
		return 0;

	printf("==========================================================\n");
	printf("FAIL: %s\n", fun);
	printf("----------------------------------------------------------\n");
	printf("\tFile \"%s\", line %d:\n", f, l);
	printf ("\t\t%s(%s, %s)\n", macro, a_tk, b_tk);
	printf("\t\t\t%d != %d\n", a, b);
	printf("----------------------------------------------------------\n");

	return 1;
}

int test_float_eq(const char *macro, const char *f, int l, const char *fun, \
		  const char *a_tk, const char *b_tk, \
		  float a, float b)
{
	if (floatcmp(&a, &b) == 0)
		return 0;

	printf("==========================================================\n");
	printf("FAIL: %s\n", fun);
	printf("----------------------------------------------------------\n");
	printf("\tFile \"%s\", line %d:\n", f, l);
	printf ("\t\t%s(%s, %s)\n", macro, a_tk, b_tk);
	printf("\t\t\t%f != %f\n", a, b);
	printf("----------------------------------------------------------\n");

	return 1;
}

int test_arr_eq(const char *macro, const char *f, int l, const char *fun, \
		const char *a_tk, const char *b_tk, const char *nel_tk, \
		const void *a, const void *b, size_t nel, size_t width, \
		int (*compare)(const void *, const void *),
		void (*display)(FILE *, const void *))
{
	if (arrcmp(a, b, nel, width, compare) == 0)
		return 0;

	printf("==========================================================\n");
	printf("FAIL: %s\n", fun);
	printf("----------------------------------------------------------\n");
	printf("\tFile \"%s\", line %d:\n", f, l);
	printf("\t\t%s(%s, %s, %s)\n", macro, a_tk, b_tk, nel_tk);
	printf("\t\t\t");
	arr_displ(stdout, a, nel, width, display);
	printf("\n");
	printf("\t\t!=\t");
	arr_displ(stdout, b, nel, width, display);
	printf("\n");
	printf("----------------------------------------------------------\n");

	return 1;
}

int test_str_eq(const char *macro, const char *f, int l, const char *fun, \
		 const char *s1_tk, const char *s2_tk, \
		 const char *s1, const char *s2)
{
	if (!s1 && !s2)
		return 0;
	if ((s1 && s2) && strcmp(s1, s2) == 0)
		return 0;

	printf("==========================================================\n");
	printf("FAIL: %s\n", fun);
	printf("----------------------------------------------------------\n");
	printf("\tFile \"%s\", line %d:\n", f, l);
	printf("\t\t%s(%s, %s)\n", macro, s1_tk, s2_tk);
	if (s1)
		printf("\t\t\t\"%s\"\n", s1);
	else
		printf("\t\t\tNULL\n");
	if (s2)
		printf("\t\t!=\t\"%s\"\n", s2);
	else
		printf("\t\t!=\tNULL\n");
	printf("----------------------------------------------------------\n");

	return 1;
}
