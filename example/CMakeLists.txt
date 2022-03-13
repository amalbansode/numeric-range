#list(APPEND example_sources
#        ${numeric_range_sources})
add_executable(range_map ${numeric_range_sources} ${CMAKE_CURRENT_LIST_DIR}/range_map.cpp)