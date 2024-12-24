# CMake generated Testfile for 
# Source directory: /home/skarg/privateprofilestring/test
# Build directory: /home/skarg/privateprofilestring/test/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(build_profile "/usr/bin/cmake" "--build" "/home/skarg/privateprofilestring/test/build" "--config" "$<CONFIG>" "--target" "test_profile")
set_tests_properties(build_profile PROPERTIES  FIXTURES_SETUP "test_fixture" _BACKTRACE_TRIPLES "/home/skarg/privateprofilestring/test/CMakeLists.txt;58;add_test;/home/skarg/privateprofilestring/test/CMakeLists.txt;0;")
add_test(test_profile "src/profile/test_profile")
set_tests_properties(test_profile PROPERTIES  FIXTURES_REQUIRED "test_fixture" _BACKTRACE_TRIPLES "/home/skarg/privateprofilestring/test/CMakeLists.txt;64;add_test;/home/skarg/privateprofilestring/test/CMakeLists.txt;0;")
subdirs("src/profile")
