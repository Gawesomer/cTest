CURRPROGS := lib/test.a
CURRTESTS := lib/tests/test_cassert lib/tests/test_cmp \
	lib/tests/test_displ lib/tests/test_framework

PROGRAMS += $(CURRPROGS)
TESTS	 += $(CURRTESTS)

PHONYS += lib lib/tests

lib: $(CURRPROGS) $(CURRTESTS)

lib/tests: $(CURRTESTS)

lib/test.a: lib/test.a( \
	lib/framework.o lib/cassert.o lib/cmp.o lib/displ.o \
	)

lib/tests/test_cassert: lib/tests/test_cassert.o lib/cmp.o lib/displ.o

lib/tests/test_cmp: lib/tests/test_cmp.o

lib/tests/test_displ: lib/tests/test_displ.o

lib/tests/test_framework: lib/tests/test_framework.o
