#ifndef _CASSERT_H_
#define _CASSERT_H_

#include <stdint.h>

#include "cmp.h"
#include "displ.h"

/* These are the internal test functions.
 * They should not be called directly and should instead by used by utilizing
 * their respective wrapper macros defined below
 */
int test_true(const char *macro, const char *f, int l, const char *fun, \
		const char *tk, int exp);
int test_int_eq(const char *macro, const char *f, int l, const char *fun, \
		const char *a_tk, const char *b_tk, int a, int b);
int test_float_eq(const char *macro, const char *f, int l, const char *fun, \
		  const char *a_tk, const char *b_tk, \
		  float a, float b);
int test_arr_eq(const char *macro, const char *f, int l, const char *fun, \
		const char *a_tk, const char *b_tk, const char *nel_tk, \
		const void *a, const void *b, size_t nel, size_t width, \
		int (*compare)(const void *, const void *),
		void (*display)(FILE *, const void *));
int test_str_eq(const char *macro, const char *f, int l, const char *fun, \
		const char *s1_tk, const char *s2_tk, \
		const char *s1, const char *s2);

/* Tests boolean to be true, prints error message if `exp` is false
 * params:
 * 	- exp: boolean expression
 * returns:
 * 	0 if `exp` is true, 1 otherwise
 */
#define TEST_TRUE(exp) \
	test_true("TEST_TRUE", __FILE__, __LINE__, __func__, #exp, exp)

/* Tests integers to be equal, prints error message if `a` is not equal to `b`
 * params:
 * 	- a: integer
 * 	- b: integer
 * returns:
 * 	0 if `a` == `b`, 1 otherwise
 */
#define TEST_INT_EQ(a, b) \
	test_int_eq("TEST_INT_EQ", __FILE__, __LINE__, __func__, \
		#a, #b, a, b)

/* Tests floats to be equal, prints error message if `a` is not equal to `b`
 * params:
 * 	- a: float
 * 	- b: float
 * returns:
 * 	0 if `a` == `b`, 1 otherwise
 */
#define TEST_FLOAT_EQ(a, b) \
	test_float_eq("TEST_FLOAT_EQ", __FILE__, __LINE__, __func__, \
		#a, #b, a, b)

/* Tests byte arrays to be equal, prints error message if `a` is not equal to
 * `b`
 * params:
 * 	- a: uint8_t byte array
 * 	- b: uint8_t byte array
 * 	- nel: size_t length of both `a` and `b`
 * returns:
 * 	0 if `a` == `b`, 1 otherwise
 * notes:
 * 	assumes that both `a` and `b` are of size `nel`;
 * 	in the case where one is shorter, it will cause a read out of bounds,
 * 	in the case where one is longer, it will fail to notice it, possibly
 * 	resulting in false positives
 */
#define TEST_BYTE_ARR_EQ(a, b, nel) \
	test_arr_eq("TEST_BYTE_ARR_EQ", __FILE__, __LINE__, __func__, \
			#a, #b, #nel, a, b, nel, \
			sizeof(uint8_t), bytecmp, byte_displ)

/* Tests integer arrays to be equal, prints error message if `a` is not equal
 * to `b`
 * params:
 * 	- a: integer array
 * 	- b: integer array
 * 	- nel: size_t length of both `a` and `b`
 * returns:
 * 	0 if `a` == `b`, 1 otherwise
 * notes:
 * 	assumes that both `a` and `b` are of size `nel`;
 * 	in the case where one is shorter, it will cause a read out of bounds,
 * 	in the case where one is longer, it will fail to notice it, possibly
 * 	resulting in false positives
 */
#define TEST_INT_ARR_EQ(a, b, nel) \
	test_arr_eq("TEST_INT_ARR_EQ", __FILE__, __LINE__, __func__, \
			#a, #b, #nel, a, b, nel, \
			sizeof(int), intcmp, int_displ)

/* Tests float arrays to be equal, prints error message if `a` is not equal
 * to `b`
 * params:
 * 	- a: float array
 * 	- b: float array
 * 	- nel: size_t length of both `a` and `b`
 * returns:
 * 	0 if `a` == `b`, 1 otherwise
 * notes:
 * 	assumes that both `a` and `b` are of size `nel`;
 * 	in the case where one is shorter, it will cause a read out of bounds,
 * 	in the case where one is longer, it will fail to notice it, possibly
 * 	resulting in false positives
 */
#define TEST_FLOAT_ARR_EQ(a, b, nel) \
	test_arr_eq("TEST_FLOAT_ARR_EQ", __FILE__, __LINE__, __func__, \
			#a, #b, #nel, a, b, nel, \
			sizeof(float), floatcmp, float_displ)

/* Tests C-strings to be equal, prints error message if `s1` is not equal to \
 * `s2`
 * params:
 *	- s1: C-string
 *	- s2: C-string
 * returns:
 * 	0 if `s1` == `s2`, 1 otherwise
 */
#define TEST_STR_EQ(s1, s2) \
	test_str_eq("TEST_STR_EQ", __FILE__, __LINE__, __func__, \
		#s1, #s2, s1, s2)

#endif
