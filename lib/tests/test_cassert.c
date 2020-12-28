#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#include "../cassert.c"

#define BUFFSIZE	1024

// Redefined here for test case consistency
#undef FLT_FIXED_EPS
#undef FLT_ULP_EPS
#define	FLT_FIXED_EPS	0.1f
#define FLT_ULP_EPS	3

/*** TEST_TRUE ***/

void test_TEST_TRUE_true(void)
{
	printf("%s\n", __func__);

	assert(TEST_TRUE(1) == 0);
	assert(TEST_TRUE(INT_MAX) == 0);
	assert(TEST_TRUE(INT_MIN) == 0);
}

void test_TEST_TRUE_true_w_expr(void)
{
	printf("%s\n", __func__);

	assert(TEST_TRUE(3*4 == 12) == 0);
	assert(TEST_TRUE(0 != 1) == 0);
}

void test_TEST_TRUE_false(void)
{
	printf("%s\n", __func__);

	int assertion;
	FILE *devnull, *tmp;

	// Execute with STDOUT redirected to /dev/null
	devnull = fopen("/dev/null", "w");
	tmp = stdout;
	stdout = devnull;

	assertion = (TEST_TRUE(0) != 0);

	stdout = tmp;
	fclose(devnull);

	assert(assertion);
}

void test_TEST_TRUE_false_w_expr(void)
{
	printf("%s\n", __func__);

	int assertion;
	FILE *devnull, *tmp;

	devnull = fopen("/dev/null", "w");
	tmp = stdout;
	stdout = devnull;

	assertion = (TEST_TRUE(3*4 == 13) != 0);

	stdout = tmp;
	fclose(devnull);

	assert(assertion);
}

void test_TEST_TRUE_stacktrace(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	/* args:
	 * 	- function name
	 * 	- file name
	 * 	- execution line number (i.e. when macro is called)
	 * 	- expressions passed to macro
	 */
	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_TRUE(%s)\n"
		"\t\t\t`%s` not true\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 8, "0", "0");
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	// Execute with STDOUT redirected to `actualstr`
	tmp = stdout;
	stdout = buffer;

	TEST_TRUE(0);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_TEST_TRUE_stacktrace_w_expr(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_TRUE(%s)\n"
		"\t\t\t`%s` not true\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, "0 == 1", "0 == 1");
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_TRUE(0 == 1);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}


/*** TEST_INT_EQ ***/

void test_TEST_INT_EQ_eq(void)
{
	printf("%s\n", __func__);

	assert(TEST_INT_EQ(0, 0) == 0);
	assert(TEST_INT_EQ(7, 7) == 0);
	assert(TEST_INT_EQ(-1, -1) == 0);
	assert(TEST_INT_EQ(INT_MAX, INT_MAX) == 0);
	assert(TEST_INT_EQ(INT_MIN, INT_MIN) == 0);
}

void test_TEST_INT_EQ_eq_w_expr(void)
{
	printf("%s\n", __func__);

	assert(TEST_INT_EQ(-8+10, 2) == 0);
	assert(TEST_INT_EQ(2, -8+10) == 0);
	assert(TEST_INT_EQ(-8+10, -8+10) == 0);
	assert(TEST_INT_EQ(TEST_INT_EQ(7, 7), 0) == 0);
	assert(TEST_INT_EQ(0, TEST_INT_EQ(7, 7)) == 0);
	assert(TEST_INT_EQ(TEST_INT_EQ(4, 4), TEST_INT_EQ(7, 7)) == 0);
}

void test_TEST_INT_EQ_not_eq(void)
{
	printf("%s\n", __func__);

	int i;
	int assertions[3];
	FILE *devnull, *tmp;

	devnull = fopen("/dev/null", "w");
	tmp = stdout;
	stdout = devnull;

	assertions[0] = (TEST_INT_EQ(0, 1) != 0);
	assertions[1] = (TEST_INT_EQ(-1, 1) != 0);
	assertions[2] = (TEST_INT_EQ(INT_MAX, INT_MIN) != 0);

	stdout = tmp;
	fclose(devnull);

	for (i = 0; i < 3; i++)
		assert(assertions[i]);
}

void test_TEST_INT_EQ_not_eq_w_expr(void)
{
	printf("%s\n", __func__);

	int i;
	int assertions[6];
	FILE *devnull, *tmp;

	devnull = fopen("/dev/null", "w");
	tmp = stdout;
	stdout = devnull;

	assertions[0] = (TEST_INT_EQ(-8+10, 1) != 0);
	assertions[1] = (TEST_INT_EQ(1, -8+10) != 0);
	assertions[2] = (TEST_INT_EQ(-8+10, 8-10) != 0);
	assertions[3] = (TEST_INT_EQ(TEST_INT_EQ(7, 7), 1) != 0);
	assertions[4] = (TEST_INT_EQ(1, TEST_INT_EQ(7, 7)) != 0);
	assertions[5] = (TEST_INT_EQ(TEST_INT_EQ(1, 2), TEST_INT_EQ(7, 7)) != 0);

	stdout = tmp;
	fclose(devnull);

	for (i = 0; i < 6; i++)
		assert(assertions[i]);
}

void test_TEST_INT_EQ_stacktrace(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_INT_EQ(%s, %s)\n"
		"\t\t\t%d != %d\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, "0", "1", 0, 1);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_INT_EQ(0, 1);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_TEST_INT_EQ_stacktrace_w_expr(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_INT_EQ(%s, %s)\n"
		"\t\t\t%d != %d\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, "-8+10", "8-10", 2, -2);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_INT_EQ(-8+10, 8-10);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

/*** TEST_FLOAT_EQ ***/

void test_TEST_FLOAT_EQ_eq(void)
{
	printf("%s\n", __func__);

	assert(TEST_FLOAT_EQ(1.0f, 1.0f) == 0);
}

void test_TEST_FLOAT_EQ_eq_w_expr(void)
{
	printf("%s\n", __func__);

	assert(TEST_FLOAT_EQ(0.1f+1.0f, 1.0f+0.1f) == 0);
}

void test_TEST_FLOAT_EQ_not_eq(void)
{
	printf("%s\n", __func__);

	int assertion;
	FILE *devnull, *tmp;

	devnull = fopen("/dev/null", "w");
	tmp = stdout;
	stdout = devnull;

	assertion = (TEST_FLOAT_EQ(1.0f, 1.2f) != 0);

	stdout = tmp;
	fclose(devnull);

	assert(assertion);
}

void test_TEST_FLOAT_EQ_not_eq_w_expr(void)
{
	printf("%s\n", __func__);

	int assertion;
	FILE *devnull, *tmp;

	devnull = fopen("/dev/null", "w");
	tmp = stdout;
	stdout = devnull;

	assertion = (TEST_FLOAT_EQ(0.0f+1.0f, 3.0f-1.8f) != 0);

	stdout = tmp;
	fclose(devnull);

	assert(assertion);
}

void test_TEST_FLOAT_EQ_stacktrace(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_FLOAT_EQ(%s, %s)\n"
		"\t\t\t%f != %f\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 8, \
		"1.0f", "1.2f", 1.0f, 1.2f);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_FLOAT_EQ(1.0f, 1.2f);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_TEST_FLOAT_EQ_stacktrace_w_expr(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_FLOAT_EQ(%s, %s)\n"
		"\t\t\t%f != %f\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 8, \
		"0.0f+1.0f", "3.0f-1.8f", 1.0f, 1.2f);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_FLOAT_EQ(0.0f+1.0f, 3.0f-1.8f);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

/*** TEST_BYTE_ARR_EQ ***/

void test_TEST_BYTE_ARR_EQ_eq(void)
{
	printf("%s\n", __func__);

	uint8_t one_el[] = {0x12};
	uint8_t mult_els[] = {0x12, 0x34, 0x45};

	assert(TEST_BYTE_ARR_EQ(NULL, NULL, 0) == 0);
	assert(TEST_BYTE_ARR_EQ(one_el, one_el, 1) == 0);
	assert(TEST_BYTE_ARR_EQ(mult_els, mult_els, 3) == 0);
}

void test_TEST_BYTE_ARR_EQ_stacktrace(void)
{
	printf("%s\n", __func__);

	int assertion;
	uint8_t a[] = {0x12, 0x34, 0x56};
	uint8_t b[] = {0x12, 0xD4, 0x56};
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_BYTE_ARR_EQ(%s, %s, %s)\n"
		"\t\t\t[%x, %x, %x]\n"
		"\t\t!=\t[%x, %x, %x]\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 8, \
		"a", "b", "3", a[0], a[1], a[2], b[0], b[1], b[2]);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_BYTE_ARR_EQ(a, b, 3);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_TEST_BYTE_ARR_EQ_stacktrace_w_null(void)
{
	printf("%s\n", __func__);

	int i;
	int assertions[2];
	uint8_t a[] = {0x12, 0x34, 0x56};
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	actualstr = calloc(BUFFSIZE, sizeof(char));

	// NULL as the first argument
	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_BYTE_ARR_EQ(%s, %s, %s)\n"
		"\t\t\tNULL\n"
		"\t\t!=\t[%x, %x, %x]\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, \
		"NULL", "a", "3", a[0], a[1], a[2]);
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_BYTE_ARR_EQ(NULL, a, 3);

	stdout = tmp;
	fclose(buffer);

	assertions[0] = (strcmp(actualstr, expectedstr) == 0);

	// NULL as the second argument
	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_BYTE_ARR_EQ(%s, %s, %s)\n"
		"\t\t\t[%x, %x, %x]\n"
		"\t\t!=\tNULL\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, \
		"a", "NULL", "3", a[0], a[1], a[2]);
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_BYTE_ARR_EQ(a, NULL, 3);

	stdout = tmp;
	fclose(buffer);

	assertions[1] = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	for (i = 0; i < 2; i++)
		assert(assertions[i]);
}

/*** TEST_INT_ARR_EQ ***/

void test_TEST_INT_ARR_EQ_eq(void)
{
	printf("%s\n", __func__);

	int one_el[] = {12};
	int mult_els[] = {12, 34, 45};

	assert(TEST_INT_ARR_EQ(NULL, NULL, 0) == 0);
	assert(TEST_INT_ARR_EQ(one_el, one_el, 1) == 0);
	assert(TEST_INT_ARR_EQ(mult_els, mult_els, 3) == 0);
}

void test_TEST_INT_ARR_EQ_stacktrace(void)
{
	printf("%s\n", __func__);

	int assertion;
	int a[] = {12, 34, 56};
	int b[] = {12, 255, 56};
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_INT_ARR_EQ(%s, %s, %s)\n"
		"\t\t\t[%d, %d, %d]\n"
		"\t\t!=\t[%d, %d, %d]\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 8, \
		"a", "b", "3", a[0], a[1], a[2], b[0], b[1], b[2]);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_INT_ARR_EQ(a, b, 3);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_TEST_INT_ARR_EQ_stacktrace_w_null(void)
{
	printf("%s\n", __func__);

	int i;
	int assertions[2];
	int a[] = {12, 34, 56};
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	actualstr = calloc(BUFFSIZE, sizeof(char));

	// NULL as the first argument
	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_INT_ARR_EQ(%s, %s, %s)\n"
		"\t\t\tNULL\n"
		"\t\t!=\t[%d, %d, %d]\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, \
		"NULL", "a", "3", a[0], a[1], a[2]);
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_INT_ARR_EQ(NULL, a, 3);

	stdout = tmp;
	fclose(buffer);

	assertions[0] = (strcmp(actualstr, expectedstr) == 0);

	// NULL as the second argument
	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_INT_ARR_EQ(%s, %s, %s)\n"
		"\t\t\t[%d, %d, %d]\n"
		"\t\t!=\tNULL\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, \
		"a", "NULL", "3", a[0], a[1], a[2]);
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_INT_ARR_EQ(a, NULL, 3);

	stdout = tmp;
	fclose(buffer);

	assertions[1] = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	for (i = 0; i < 2; i++)
		assert(assertions[i]);
}

/*** TEST_FLOAT_ARR_EQ ***/

void test_TEST_FLOAT_ARR_EQ_eq(void)
{
	printf("%s\n", __func__);

	float one_el[] = {12.0f};
	float mult_els[] = {12.0f, 34.1f, 45.2f};

	assert(TEST_FLOAT_ARR_EQ(NULL, NULL, 0) == 0);
	assert(TEST_FLOAT_ARR_EQ(one_el, one_el, 1) == 0);
	assert(TEST_FLOAT_ARR_EQ(mult_els, mult_els, 3) == 0);
}

void test_TEST_FLOAT_ARR_EQ_stacktrace(void)
{
	printf("%s\n", __func__);

	int assertion;
	float a[] = {12.1f, 34.2f, 56.3f};
	float b[] = {12.1f, 255.2f, 56.3f};
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_FLOAT_ARR_EQ(%s, %s, %s)\n"
		"\t\t\t[%f, %f, %f]\n"
		"\t\t!=\t[%f, %f, %f]\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 8, \
		"a", "b", "3", a[0], a[1], a[2], b[0], b[1], b[2]);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_FLOAT_ARR_EQ(a, b, 3);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_TEST_FLOAT_ARR_EQ_stacktrace_w_null(void)
{
	printf("%s\n", __func__);

	int i;
	int assertions[2];
	float a[] = {12.0f, 34.1f, 56.2f};
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	actualstr = calloc(BUFFSIZE, sizeof(char));

	// NULL as the first argument
	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_FLOAT_ARR_EQ(%s, %s, %s)\n"
		"\t\t\tNULL\n"
		"\t\t!=\t[%f, %f, %f]\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, \
		"NULL", "a", "3", a[0], a[1], a[2]);
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_FLOAT_ARR_EQ(NULL, a, 3);

	stdout = tmp;
	fclose(buffer);

	assertions[0] = (strcmp(actualstr, expectedstr) == 0);

	// NULL as the second argument
	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_FLOAT_ARR_EQ(%s, %s, %s)\n"
		"\t\t\t[%f, %f, %f]\n"
		"\t\t!=\tNULL\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, \
		"a", "NULL", "3", a[0], a[1], a[2]);
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_FLOAT_ARR_EQ(a, NULL, 3);

	stdout = tmp;
	fclose(buffer);

	assertions[1] = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	for (i = 0; i < 2; i++)
		assert(assertions[i]);
}

/*** TEST_STR_EQ ***/

void test_TEST_STR_EQ_eq(void)
{
	printf("%s\n", __func__);

	char s[] = "hello world";

	assert(TEST_STR_EQ(NULL, NULL) == 0);
	assert(TEST_STR_EQ(s, s) == 0);
}

void test_TEST_STR_EQ_stacktrace(void)
{
	printf("%s\n", __func__);

	int assertion;
	char s1[] = "hello";
	char s2[] = "byebye";
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_STR_EQ(%s, %s)\n"
		"\t\t\t\"%s\"\n"
		"\t\t!=\t\"%s\"\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 8, \
		"s1", "s2", s1, s2);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_STR_EQ(s1, s2);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_TEST_STR_EQ_stacktrace_w_null(void)
{
	printf("%s\n", __func__);

	int i;
	int assertions[2];
	char s[] = "hello world";
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	actualstr = calloc(BUFFSIZE, sizeof(char));

	// NULL as the first argument
	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_STR_EQ(%s, %s)\n"
		"\t\t\tNULL\n"
		"\t\t!=\t\"%s\"\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, \
		"NULL", "s", s);
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_STR_EQ(NULL, s);

	stdout = tmp;
	fclose(buffer);

	assertions[0] = (strcmp(actualstr, expectedstr) == 0);

	// NULL as the second argument
	snprintf(expectedstr, BUFFSIZE, \
		"==========================================================\n"
		"FAIL: %s\n"
		"----------------------------------------------------------\n"
		"\tFile \"%s\", line %d:\n"
		"\t\tTEST_STR_EQ(%s, %s)\n"
		"\t\t\t\"%s\"\n"
		"\t\t!=\tNULL\n"
		"----------------------------------------------------------\n", \
		__func__, __FILE__, __LINE__ + 7, \
		"s", "NULL", s);
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	TEST_STR_EQ(s, NULL);

	stdout = tmp;
	fclose(buffer);

	assertions[1] = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	for (i = 0; i < 2; i++)
		assert(assertions[i]);
}

int main(void)
{
	printf("--- %s ---\n", __FILE__);

	test_TEST_TRUE_true();
	test_TEST_TRUE_true_w_expr();
	test_TEST_TRUE_false();
	test_TEST_TRUE_false_w_expr();
	test_TEST_TRUE_stacktrace();
	test_TEST_TRUE_stacktrace_w_expr();

	test_TEST_INT_EQ_eq();
	test_TEST_INT_EQ_eq_w_expr();
	test_TEST_INT_EQ_not_eq();
	test_TEST_INT_EQ_not_eq_w_expr();
	test_TEST_INT_EQ_stacktrace();
	test_TEST_INT_EQ_stacktrace_w_expr();

	test_TEST_FLOAT_EQ_eq();
	test_TEST_FLOAT_EQ_eq_w_expr();
	test_TEST_FLOAT_EQ_not_eq();
	test_TEST_FLOAT_EQ_not_eq_w_expr();
	test_TEST_FLOAT_EQ_stacktrace();
	test_TEST_FLOAT_EQ_stacktrace_w_expr();

	test_TEST_BYTE_ARR_EQ_eq();
	test_TEST_BYTE_ARR_EQ_stacktrace();
	test_TEST_BYTE_ARR_EQ_stacktrace_w_null();

	test_TEST_INT_ARR_EQ_eq();
	test_TEST_INT_ARR_EQ_stacktrace();
	test_TEST_INT_ARR_EQ_stacktrace_w_null();

	test_TEST_FLOAT_ARR_EQ_eq();
	test_TEST_FLOAT_ARR_EQ_stacktrace();
	test_TEST_FLOAT_ARR_EQ_stacktrace_w_null();

	test_TEST_STR_EQ_eq();
	test_TEST_STR_EQ_stacktrace();
	test_TEST_STR_EQ_stacktrace_w_null();

	printf("----------------------------------------------------------\n");
	printf("\nOK\n");

	return EXIT_SUCCESS;
}
