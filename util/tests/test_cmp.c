#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../cmp.c"

// Redefined here for test case consistency
#define FLT_FIXED_EPS	0.1f
#define FLT_ULP_EPS	3

/*** check_null ***/

void test_check_null(void)
{
	printf("%s\n", __func__);

	int e = 0;

	assert(check_null(&e, NULL) == 2);
	assert(check_null(NULL, &e) == -1);
	assert(check_null(NULL, NULL) == 1);
	assert(check_null(&e, &e) == 0);
}

void test_check_null_useage(void)
{
	printf("%s\n", __func__);

	int e = 0;

	assert(check_null(&e, NULL)-1 > 0);
	assert(check_null(NULL, &e)-1 < 0);
	assert(check_null(NULL, NULL)-1 == 0);
	assert(check_null(&e, &e)-1 != 0);
}

/*** intcmp ***/

void test_intcmp_eq(void)
{
	printf("%s\n", __func__);

	int a, b;

	a = b = 7;

	assert(intcmp(&a, &b) == 0);
}

void test_intcmp_lt(void)
{
	printf("%s\n", __func__);

	int a, b;

	a = 5;
	b = 7;

	assert(intcmp(&a, &b) < 0);
}

void test_intcmp_gt(void)
{
	printf("%s\n", __func__);

	int a, b;

	a = 7;
	b = 5;

	assert(intcmp(&a, &b) > 0);
}

void test_intcmp_null(void)
{
	printf("%s\n", __func__);

	int e = 0;

	assert(intcmp(&e, NULL) > 0);
	assert(intcmp(NULL, &e) < 0);
	assert(intcmp(NULL, NULL) == 0);
}

/*** bytecmp ***/

void test_bytecmp_eq(void)
{
	printf("%s\n", __func__);

	uint8_t a, b;

	a = b = 0xCD;

	assert(bytecmp(&a, &b) == 0);
}

void test_bytecmp_lt(void)
{
	printf("%s\n", __func__);

	uint8_t a, b;

	a = 0x0A;
	b = 0xA0;

	assert(bytecmp(&a, &b) < 0);
}

void test_bytecmp_gt(void)
{
	printf("%s\n", __func__);

	uint8_t a, b;

	a = 0xA0;
	b = 0x0A;

	assert(bytecmp(&a, &b) > 0);
}

void test_bytecmp_null(void)
{
	printf("%s\n", __func__);

	uint8_t e;

	assert(bytecmp(&e, NULL) > 0);
	assert(bytecmp(NULL, &e) < 0);
	assert(bytecmp(NULL, NULL) == 0);
}

/*** ftoi_punning ***/

void test_ftoi_punning(void)
{
	printf("%s\n", __func__);

	assert(ftoi_punning(0.0f) == 0);
	assert(ftoi_punning(35.02f) == 1108087931);
	assert(ftoi_punning(0.1f) == 1036831949);
	assert(ftoi_punning(-128.0f) == -1023410176);
	assert(ftoi_punning(6.022141e23f) == 1727990831);
}

/*** itof_punning ***/

void test_itof_punning(void)
{
	printf("%s\n", __func__);

	assert(itof_punning(0) == 0.0f);
	assert(itof_punning(1108087931) == 35.02f);
	assert(itof_punning(1036831949) == 0.1f);
	assert(itof_punning(-1023410176) == -128.0f);
	assert(itof_punning(1727990831) == 602214100383781913362432.0f);
}

/*** ulpdist ***/

void test_ulpdist_eq(void)
{
	printf("%s\n", __func__);

	assert(ulpdist(0.1f, 0.1f) == 0);
	assert(ulpdist(6.022141e23f, 6.022141e23f) == 0);
	assert(ulpdist(-0.0f, 0.0f) == 0);
}

void test_ulpdist_neq(void)
{
	printf("%s\n", __func__);

	assert(ulpdist(1.0f, 1.0f+FLT_EPSILON) == 1);
	assert(ulpdist(35.020000f, 35.020031f) == 8);
	assert(ulpdist(35.020031f, 35.020000f) == 8);
	assert(ulpdist(0.001000000047497451305389f, \
			0.001000011689029633998871f) == 100);
	assert(ulpdist(602214100383781913362432.0f, \
			602214136412578932326400.0f) == 1);
}

void test_ulpdist_opposite_signs(void)
{
	printf("%s\n", __func__);

	assert(ulpdist(-1.0f, 1.0f) == INT_MAX);
	assert(ulpdist(FLT_EPSILON, -1*FLT_EPSILON) == INT_MAX);
}

/*** float_ep ***/

void test_float_eq_exactly_eq(void)
{
	printf("%s\n", __func__);

	assert(float_eq(0.01f, 0.01f) == 0);
	assert(float_eq(1.0f, 1.0f) == 0);
	assert(float_eq(3000000000.0f, 3000000000.0f) == 0);
}

void test_float_eq_eq_w_fixed_eps(void)
{
	printf("%s\n", __func__);

	assert(float_eq(0.10f, 0.15f) == 0);
	assert(float_eq(0.0f, 0.000001f) == 0);
	assert(float_eq(1.0f, 1.0f+FLT_EPSILON) == 0);
	assert(float_eq(-1*FLT_EPSILON, FLT_EPSILON) == 0);
}

void test_float_eq_eq_w_ulp_eps(void)
{
	printf("%s\n", __func__);

	assert(float_eq(34.199993133544921875000000f, \
				34.200000762939453125000000f) == 0);
	assert(float_eq(602214100383781913362432.0f, \
				602214136412578932326400.0f) == 0);
}

void test_float_eq_neq(void)
{
	printf("%s\n", __func__);

	assert(float_eq(-0.1f, 0.1f) != 0);
	assert(float_eq(0.1f, 0.2f+FLT_EPSILON) != 0);
	assert(float_eq(34.0f, 35.0f) != 0);
}

/*** floatcmp ***/

void test_floatcmp_eq(void)
{
	printf("%s\n", __func__);

	float a, b;

	a = b = 7.1f;
	assert(floatcmp(&a, &b) == 0);
}

void test_floatcmp_lt(void)
{
	printf("%s\n", __func__);

	float a, b;

	a = 5.0f;
	b = 7.0f;

	assert(floatcmp(&a, &b) < 0);
}

void test_floatcmp_gt(void)
{
	printf("%s\n", __func__);

	float a, b;

	a = 7.0f;
	b = 5.0f;

	assert(floatcmp(&a, &b) > 0);
}

void test_floatcmp_null(void)
{
	printf("%s\n", __func__);

	float e = 0.0f;

	assert(floatcmp(&e, NULL) > 0);
	assert(floatcmp(NULL, &e) < 0);
	assert(floatcmp(NULL, NULL) == 0);
}

/*** arrcmp ***/

void test_arrcmp_eq(void)
{
	printf("%s\n", __func__);

	int a_arr[] = {0, 1, 2};
	int b_arr[] = {0, 1, 2};

	assert(arrcmp(a_arr, b_arr, 3, sizeof(int), intcmp) == 0);
}

void test_arrcmp_neq(void)
{
	printf("%s\n", __func__);

	int a_arr[] = {0, 2, 1};
	int b_arr[] = {0, 1, 2};

	assert(arrcmp(a_arr, b_arr, 3, sizeof(int), intcmp) != 0);
}

void test_arrcmp_null(void)
{
	printf("%s\n", __func__);

	int arr[] = {0, 1, 2};

	assert(arrcmp(arr, NULL, 3, sizeof(int), intcmp) != 0);
	assert(arrcmp(NULL, arr, 3, sizeof(int), intcmp) != 0);
	assert(arrcmp(NULL, NULL, 0, sizeof(int), intcmp) == 0);
}

int main(void)
{
	printf("--- %s ---\n", __FILE__);

	test_check_null();
	test_check_null_useage();

	test_intcmp_eq();
	test_intcmp_lt();
	test_intcmp_gt();
	test_intcmp_null();

	test_bytecmp_eq();
	test_bytecmp_lt();
	test_bytecmp_gt();
	test_bytecmp_null();

	test_ftoi_punning();
	test_itof_punning();

	test_ulpdist_eq();
	test_ulpdist_neq();
	test_ulpdist_opposite_signs();

	test_float_eq_exactly_eq();
	test_float_eq_eq_w_fixed_eps();
	test_float_eq_eq_w_ulp_eps();
	test_float_eq_neq();

	test_floatcmp_eq();
	test_floatcmp_lt();
	test_floatcmp_gt();
	test_floatcmp_null();

	test_arrcmp_eq();
	test_arrcmp_neq();
	test_arrcmp_null();

	printf("----------------------------------------------------------\n");
	printf("\nOK\n");

	return EXIT_SUCCESS;
}
