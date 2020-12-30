#include <stdio.h>
#include <stdlib.h>

#include "cTest.h"

#define TEST_BATCH_SIZE	1024

struct testcase {
	const char *name;
	int (*fun)(void);
};

struct testcase *tests;		/* Collection of tests */
int tests_size = 0;		/* Size of `tests` */
int curr_test_index = 0;	/* Next available position in `tests` */

int register_test(const char *name, int (*fun)(void))
{
	if (!fun)
		return 1;
	struct testcase test = {name, fun};
	if (curr_test_index >= tests_size) {
		tests_size += TEST_BATCH_SIZE;
		tests = realloc(tests, sizeof(struct testcase)*tests_size);
	}
	tests[curr_test_index++] = test;
	return 0;
}

void clean_tests(void)
{
	free(tests);
	tests = NULL;
	tests_size = 0;
	curr_test_index = 0;
}

int run_tests(const char *filename)
{
	int failures;
	int i;

	fprintf(PRINTSTRM, "--- %s ---\n", filename);

	failures = 0;
	for (i = 0; i < curr_test_index; i++) {
		fprintf(PRINTSTRM, "%s\n", tests[i].name);
		if ((*(tests[i].fun))() != 0)
			failures++;
	}
	
	fprintf(PRINTSTRM, "----------------------------------------------------------\n");
	fprintf(PRINTSTRM, "Ran %d tests\n", curr_test_index);
	if (failures == 0)
		fprintf(PRINTSTRM, "\nOK\n");
	else
		fprintf(PRINTSTRM, "\nFAILED (failures=%d)\n", failures);

	clean_tests();

	return failures;
}
