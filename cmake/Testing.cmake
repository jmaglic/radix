enable_testing()

set(RADIX_TEST_DIR ${CMAKE_SOURCE_DIR}/test)

add_executable(radix_emplace ${RADIX_TEST_DIR}/radix_emplace.cpp)
add_test(NAME radix_emplace COMMAND ./radix_emplace)

add_executable(radix_insert ${RADIX_TEST_DIR}/radix_insert.cpp)
add_test(NAME radix_insert COMMAND ./radix_insert)

