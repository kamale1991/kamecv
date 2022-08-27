add_test( binary_image.test /home/shingo/work/2205/cellular_automaton/build/bin/test_opencv [==[--gtest_filter=binary_image.test]==] --gtest_also_run_disabled_tests)
set_tests_properties( binary_image.test PROPERTIES WORKING_DIRECTORY /home/shingo/work/2205/cellular_automaton/build/cellular_automaton/test)
set( test_opencv_TESTS binary_image.test)
