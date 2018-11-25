include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR})

set( TEST_SRCS
    test_keyval.cpp
)

add_executable( test_cbsl ${TEST_SRCS})
target_link_libraries(test_cbsl gtest_main cbsl rt)