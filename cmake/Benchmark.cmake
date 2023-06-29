
set(RADIX_BENCHMARK_DIR ${CMAKE_SOURCE_DIR}/test/benchmark)

set(BENCHMARK_NAMES
  benchmark_complete
  benchmark_new_entry
  benchmark_extract_insert
  benchmark_lookup)

foreach(BM IN ITEMS ${BENCHMARK_NAMES})

  add_executable(${BM} ${RADIX_BENCHMARK_DIR}/${BM}.cpp)
  set_target_properties(${BM} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarkbin)

endforeach()
