#include "radix.hpp"

// Use of contains()
int main() {

  // LOOKUP TESTS

  // count c
  // template count c
  // contains c
  // template contains c
  
  xsm::radix<bool> rdx;
  rdx.emplace("hello",true);
  rdx.emplace("hellscape",true);
  
  assert(rdx.contains("hello"));
  assert(!rdx.contains("hell"));
  
  rdx.emplace("hell",true);

  assert(rdx.contains("hell"));

  return 0;
}

