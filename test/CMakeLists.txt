list(APPEND test_sources
        ${numeric_range_sources}
        ${CMAKE_CURRENT_LIST_DIR}/catch.hpp)
add_executable(numeric_range_test ${test_sources} ${CMAKE_CURRENT_LIST_DIR}/numeric_range_test.cpp)