CURRTESTS := util/tests/test_cmp

TESTS	 += $(CURRTESTS)

PHONYS += util util/tests

util/tests: $(CURRTESTS)

util/tests/test_cmp: util/tests/test_cmp.o
