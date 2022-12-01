enable_testing()

set(RADIX_TEST_DIR ${CMAKE_SOURCE_DIR}/test)

set(TEST_NAMES
  radix_emplace
  radix_insert
  radix_overwrite
  radix_contains
  radix_dereference
  radix_assign
  radix_clear
  radix_erase
  radix_iterator)

foreach(TN IN ITEMS ${TEST_NAMES})

  add_executable(${TN} ${RADIX_TEST_DIR}/${TN}.cpp)
  add_test(NAME ${TN} COMMAND ${TN})

endforeach()

