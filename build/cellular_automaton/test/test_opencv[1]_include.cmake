if(EXISTS "/home/shingo/work/2205/cellular_automaton/build/cellular_automaton/test/test_opencv[1]_tests.cmake")
  include("/home/shingo/work/2205/cellular_automaton/build/cellular_automaton/test/test_opencv[1]_tests.cmake")
else()
  add_test(test_opencv_NOT_BUILT test_opencv_NOT_BUILT)
endif()
