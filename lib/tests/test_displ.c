#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../displ.c"

#define BUFFSIZE	1024

/*** int_displ ***/

void test_int_displ_to_alternate_stream(void)
{
	printf("%s\n", __func__);

	int assertion;
	int e = 128;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer;

	snprintf(expectedstr, BUFFSIZE, "%d", e);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	int_displ(buffer, &e);

	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_int_displ_null(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[] = "NULL";
	FILE *buffer;

	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	int_displ(buffer, NULL);

	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_int_displ_null_stream_defaults_to_stdout(void)
{
	printf("%s\n", __func__);

	int assertion;
	int e = 128;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, "%d", e);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	int_displ(NULL, &e);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

/*** byte_displ ***/

void test_byte_displ_to_alternate_stream(void)
{
	printf("%s\n", __func__);

	int assertion;
	uint8_t e = 0x7D;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer;

	snprintf(expectedstr, BUFFSIZE, "%x", e);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	byte_displ(buffer, &e);

	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_byte_displ_null(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[] = "NULL";
	FILE *buffer;

	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	byte_displ(buffer, NULL);

	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_byte_displ_null_stream_defaults_to_stdout(void)
{
	printf("%s\n", __func__);

	int assertion;
	uint8_t e = 0x7D;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, "%x", e);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	byte_displ(NULL, &e);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

/*** float_displ ***/

void test_float_displ_to_alternate_stream(void)
{
	printf("%s\n", __func__);

	int assertion;
	float e = 128.05f;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer;

	snprintf(expectedstr, BUFFSIZE, "%f", e);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	float_displ(buffer, &e);

	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_float_displ_null(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[] = "NULL";
	FILE *buffer;

	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	float_displ(buffer, NULL);

	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_float_displ_null_stream_defaults_to_stdout(void)
{
	printf("%s\n", __func__);

	int assertion;
	float e = 128.05f;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, "%f", e);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	float_displ(NULL, &e);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

/*** arr_displ ***/

void test_arr_displ_null(void)
{
	printf("%s\n", __func__);

	int assertion;
	char *actualstr;
	char expectedstr[] = "NULL";
	FILE *buffer, *tmp;

	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	arr_displ(stdout, NULL, 0, sizeof(int), int_displ);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_arr_displ_empty(void)
{
	printf("%s\n", __func__);

	int assertion;
	int arr[] = {0};
	char *actualstr;
	char expectedstr[] = "[]";
	FILE *buffer, *tmp;

	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	arr_displ(stdout, arr, 0, sizeof(int), int_displ);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_arr_displ_multiple_els(void)
{
	printf("%s\n", __func__);

	int assertion;
	int arr[] = {1, 2, 3};
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, "[%d, %d, %d]", \
			arr[0], arr[1], arr[2]);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	arr_displ(stdout, arr, 3, sizeof(int), int_displ);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_arr_displ_null_stream_defaults_to_stdout(void)
{
	printf("%s\n", __func__);

	int assertion;
	int arr[] = {1, 2, 3};
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	snprintf(expectedstr, BUFFSIZE, "[%d, %d, %d]", \
			arr[0], arr[1], arr[2]);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = stdout;
	stdout = buffer;

	arr_displ(NULL, arr, 3, sizeof(int), int_displ);

	stdout = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

void test_arr_displ_to_alternate_stream(void)
{
	printf("%s\n", __func__);

	int assertion;
	int arr[] = {1, 2, 3};
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer;

	snprintf(expectedstr, BUFFSIZE, "[%d, %d, %d]", \
			arr[0], arr[1], arr[2]);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	arr_displ(buffer, arr, 3, sizeof(int), int_displ);

	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
}

int main(void)
{
	printf("--- %s ---\n", __FILE__);

	test_int_displ_to_alternate_stream();
	test_int_displ_null();
	test_int_displ_null_stream_defaults_to_stdout();

	test_byte_displ_to_alternate_stream();
	test_byte_displ_null();
	test_byte_displ_null_stream_defaults_to_stdout();

	test_float_displ_to_alternate_stream();
	test_float_displ_null();
	test_float_displ_null_stream_defaults_to_stdout();

	test_arr_displ_null();
	test_arr_displ_empty();
	test_arr_displ_multiple_els();
	test_arr_displ_null_stream_defaults_to_stdout();
	test_arr_displ_to_alternate_stream();

	printf("----------------------------------------------------------\n");
	printf("\nOK\n");

	return EXIT_SUCCESS;
}
