add_test( generate_gaussian_random_array.test /home/shingo/work/2205/cellular_automaton/build/bin/test_noise [==[--gtest_filter=generate_gaussian_random_array.test]==] --gtest_also_run_disabled_tests)
set_tests_properties( generate_gaussian_random_array.test PROPERTIES WORKING_DIRECTORY /home/shingo/work/2205/cellular_automaton/build/cellular_automaton/test)
set( test_noise_TESTS generate_gaussian_random_array.test)
