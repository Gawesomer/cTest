#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../framework.c"

#define	BUFFSIZE	1024

/*
// Included from C-file
extern struct testcase *tests;
extern int tests_size;
extern int curr_test_index;
*/

int test_fails(void)
{
	return 1;
}

int test_passes(void)
{
	return 0;
}


/*** clean_tests ***/

void test_clean_tests_no_tests(void)
{
	printf("%s\n", __func__);

	clean_tests();

	assert(tests_size == 0);
	assert(curr_test_index == 0);
	assert(tests == NULL);
}

void test_clean_tests_w_tests_allocated_tests_get_freed(void)
{
	printf("%s\n", __func__);

	tests = calloc(32, sizeof(struct testcase));
	tests_size = 32;
	curr_test_index = 16;

	clean_tests();

	assert(tests_size == 0);
	assert(curr_test_index == 0);
	assert(tests == NULL);
}

/*** REGISTER_TEST ***/

void test_REGISTER_TEST_null_adds_no_test(void)
{
	printf("%s\n", __func__);

	REGISTER_TEST(NULL);

	assert(tests_size == 0);
	assert(curr_test_index == 0);
}

void test_REGISTER_TEST_first_test_allocates_and_adds_to_tests(void)
{
	printf("%s\n", __func__);

	int tests_size_snpsht, curr_test_index_snpsht;
	int (*tests_fun_snpsht)(void);
	const char *tests_name_snpsht;

	REGISTER_TEST(test_passes);

	tests_size_snpsht = tests_size;
	curr_test_index_snpsht = curr_test_index;
	tests_fun_snpsht = tests[0].fun;
	tests_name_snpsht = tests[0].name;

	clean_tests();

	assert(tests_size_snpsht == TEST_BATCH_SIZE);
	assert(curr_test_index_snpsht == 1);
	assert(tests_fun_snpsht == test_passes);
	assert(strcmp(tests_name_snpsht, "test_passes") == 0);
}

void test_REGISTER_TEST_fill_tests_reallocates_space(void)
{
	printf("%s\n", __func__);

	int tests_size_snpsht, curr_test_index_snpsht;
	int i;

	for (i = 0; i < TEST_BATCH_SIZE; i++) {
		REGISTER_TEST(test_passes);
	}

	assert(tests_size == TEST_BATCH_SIZE);
	assert(curr_test_index == TEST_BATCH_SIZE);

	REGISTER_TEST(test_passes);

	tests_size_snpsht = tests_size;
	curr_test_index_snpsht = curr_test_index;

	clean_tests();

	assert(tests_size_snpsht == 2*TEST_BATCH_SIZE);
	assert(curr_test_index_snpsht == TEST_BATCH_SIZE+1);
}

/*** RUN_TESTS ***/

void test_RUN_TESTS_no_tests_succeeds(void)
{
	printf("%s\n", __func__);

	int res;
	FILE *devnull, *tmp;

	devnull = fopen("/dev/null", "w");
	tmp = PRINTSTRM;
	PRINTSTRM = devnull;

	res = RUN_TESTS();

	PRINTSTRM = tmp;
	fclose(devnull);

	assert(res == 0);
}

void test_RUN_TESTS_cleans_and_frees_memory_after_use(void)
{
	printf("%s\n", __func__);

	FILE *devnull, *tmp;

	REGISTER_TEST(test_passes);
	REGISTER_TEST(test_fails);

	devnull = fopen("/dev/null", "w");
	tmp = PRINTSTRM;
	PRINTSTRM = devnull;

	RUN_TESTS();

	PRINTSTRM = tmp;
	fclose(devnull);

	assert(tests_size == 0);
	assert(curr_test_index == 0);
	assert(tests == NULL);
}

void test_RUN_TESTS_single_failing_tests_causes_failure(void)
{
	printf("%s\n", __func__);

	int res;
	FILE *devnull, *tmp;

	REGISTER_TEST(test_passes);
	REGISTER_TEST(test_fails);

	devnull = fopen("/dev/null", "w");
	tmp = PRINTSTRM;
	PRINTSTRM = devnull;

	res = RUN_TESTS();

	PRINTSTRM = tmp;
	fclose(devnull);

	assert(res == 1);
}

void test_RUN_TESTS_passing_tests_displays_success(void)
{
	printf("%s\n", __func__);

	int res, assertion;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	REGISTER_TEST(test_passes);
	REGISTER_TEST(test_passes);

	snprintf(expectedstr, BUFFSIZE, \
		"--- %s ---\n"
		"test_passes\n"
		"test_passes\n"
		"----------------------------------------------------------\n"
		"Ran 2 tests\n"
		"\nOK\n", \
		__FILE__);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = PRINTSTRM;
	PRINTSTRM = buffer;

	res = RUN_TESTS();

	PRINTSTRM = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
	assert(res == 0);
}

void test_RUN_TESTS_failing_tests_displays_failures(void)
{
	printf("%s\n", __func__);

	int res, assertion;
	char *actualstr;
	char expectedstr[BUFFSIZE];
	FILE *buffer, *tmp;

	REGISTER_TEST(test_fails);
	REGISTER_TEST(test_fails);

	snprintf(expectedstr, BUFFSIZE, \
		"--- %s ---\n"
		"test_fails\n"
		"test_fails\n"
		"----------------------------------------------------------\n"
		"Ran 2 tests\n"
		"\nFAILED (failures=2)\n", \
		__FILE__);
	actualstr = calloc(BUFFSIZE, sizeof(char));
	buffer = fmemopen(actualstr, BUFFSIZE, "w");

	tmp = PRINTSTRM;
	PRINTSTRM = buffer;

	res = RUN_TESTS();

	PRINTSTRM = tmp;
	fclose(buffer);

	assertion = (strcmp(actualstr, expectedstr) == 0);

	free(actualstr);

	assert(assertion);
	assert(res == 2);
}

int main(void)
{
	printf("--- %s ---\n", __FILE__);
	test_clean_tests_no_tests();
	test_clean_tests_w_tests_allocated_tests_get_freed();

	test_REGISTER_TEST_null_adds_no_test();
	test_REGISTER_TEST_first_test_allocates_and_adds_to_tests();
	test_REGISTER_TEST_fill_tests_reallocates_space();

	test_RUN_TESTS_no_tests_succeeds();
	test_RUN_TESTS_cleans_and_frees_memory_after_use();
	test_RUN_TESTS_single_failing_tests_causes_failure();
	test_RUN_TESTS_passing_tests_displays_success();
	test_RUN_TESTS_failing_tests_displays_failures();

	printf("----------------------------------------------------------\n");
	printf("\nOK\n");

	return EXIT_SUCCESS;
}
