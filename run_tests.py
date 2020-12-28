#!/usr/bin/env python3

import argparse
import os


"""
Get command line arguments
params:
    none
returns:
    taget_file: filename
    valgrind: boolean to indicate whether Valgrind should be used
"""
def get_args():
    help_text = "Run tests"
    parser = argparse.ArgumentParser(description=help_text)
    parser.add_argument("target_file", nargs='?', default=os.getcwd(), \
            help="top directory from which test executables will be searched")
    parser.add_argument("--valgrind", action="store_true",
            help="run tests with Valgrind")
    args = parser.parse_args()

    return (args.target_file, args.valgrind)


"""
Search recursively from `search_dir` for test files.
Test files are files with names starting in "test_" that are executable.
params:
    - search_dir: top directory from which recursive search will begin
returns:
    set of test file names relative to current working directory
"""
def find_test_files(search_dir):
    test_files = set()
    for (root, dirs, files) in os.walk(search_dir):
        for filename in (name for name in files if name.startswith("test_")):
            file_rel_path = os.path.join(root, filename)
            if os.access(file_rel_path, os.X_OK):
                test_files.add(file_rel_path)
    return test_files


"""
Determine if Valgrind is installed
params:
    none
return:
    True if Valgrind is installed, False otherwise
"""
def valgrind_installed():
    return (os.system("which valgrind > /dev/null") == 0)


"""
Run test files in `test_files`
params:
    - test_files: collection of test file names
                  file name must be relative to the current working directory
                  test files must be executable
    - use_valgrind: boolean too indicate whether tests should be run using
                    Valgrind. Valgrind will only be used if it`s determined to
                    be installed
returns:
    set of test file names that failed (i.e. returned a nonzero exit status)
"""
def run_tests(test_files, use_valgrind):
    failed_tests = set()

    print("### Running tests ////////////////////////////////////////////////")

    if use_valgrind and valgrind_installed():
        valgrind_cmd = "valgrind --error-exitcode=1 --leak-check=full"
    else:
        valgrind_cmd = ""

    for test_file in test_files:
        cmd = "{} {}".format(valgrind_cmd, test_file)
        if os.system(cmd) != 0:
            failed_tests.add(test_file)
    return failed_tests


"""
Display final status
Prints OK if all tests passed, prints the filenames of the test files that
failed otherwise
params:
    - test_files: collection of test files that were run
    - failed_tests: collection of test file names that failed
returns:
    nothing
"""
def display_status(test_files, failed_tests):
    print("//////////////////////////////////////////////////////////////////")
    print("")

    print("Ran the following files:")
    for test_file in test_files:
        print("\t- {}".format(test_file))

    print("")

    if len(failed_tests) == 0:
        print("OK - All tests passed")
        return

    print("FAILED - The following failed:")
    for test_file in failed_tests:
        print("\t- {}".format(test_file))


if __name__ == "__main__":
    search_dir, use_valgrind = get_args()
    test_files = find_test_files(search_dir)
    failed_tests = run_tests(test_files, use_valgrind)
    display_status(test_files, failed_tests)
    exit(len(failed_tests))
