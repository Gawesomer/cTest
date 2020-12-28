INCLUDEDIRS := ./include
UTILDIR     := ./util

runtests:
	./run_tests.py --valgrind

PHONYS := runtests
