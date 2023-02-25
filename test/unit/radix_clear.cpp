#include "radix.hpp"

// Use of clear() and empty()
int main() {
  xsm::radix<bool> rdx;
  assert(rdx.empty());
  
  rdx.emplace("something",true);
  assert(!rdx.empty());

  rdx.clear();
  assert(rdx.empty());
  return 0;
}

