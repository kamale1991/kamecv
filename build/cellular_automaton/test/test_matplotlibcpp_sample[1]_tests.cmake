add_test( matplotlib.sample /home/shingo/work/2205/cellular_automaton/build/bin/test_matplotlibcpp_sample [==[--gtest_filter=matplotlib.sample]==] --gtest_also_run_disabled_tests)
set_tests_properties( matplotlib.sample PROPERTIES WORKING_DIRECTORY /home/shingo/work/2205/cellular_automaton/build/cellular_automaton/test)
set( test_matplotlibcpp_sample_TESTS matplotlib.sample)
