if(EXISTS "/home/shingo/work/cvs/kamecv/build/cellular_automaton/test/test_binary_image[1]_tests.cmake")
  include("/home/shingo/work/cvs/kamecv/build/cellular_automaton/test/test_binary_image[1]_tests.cmake")
else()
  add_test(test_binary_image_NOT_BUILT test_binary_image_NOT_BUILT)
endif()
