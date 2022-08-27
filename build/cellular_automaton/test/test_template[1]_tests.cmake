add_test( sample_template.test /home/shingo/work/2205/cellular_automaton/build/bin/test_template [==[--gtest_filter=sample_template.test]==] --gtest_also_run_disabled_tests)
set_tests_properties( sample_template.test PROPERTIES WORKING_DIRECTORY /home/shingo/work/2205/cellular_automaton/build/cellular_automaton/test)
set( test_template_TESTS sample_template.test)
