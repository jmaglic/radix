enable_testing()

set(RADIX_TEST_DIR ${CMAKE_SOURCE_DIR}/test/unit)

set(TEST_NAMES
  radix_emplace
  radix_insert
  radix_insert_hint
  radix_overwrite
  radix_contains
  radix_dereference
  radix_assign
  radix_clear
  radix_erase
  radix_iterator
  radix_iterator_type)

foreach(TN IN ITEMS ${TEST_NAMES})

  add_executable(${TN} ${RADIX_TEST_DIR}/${TN}.cpp)
  set_target_properties(${TN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testbin)
  add_test(NAME ${TN} COMMAND ${TN})

endforeach()

