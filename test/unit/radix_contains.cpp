#include "radix.hpp"

// Use of contains()
int main() {

  xsm::radix<bool> rdx;
  rdx.emplace("hello",true);
  rdx.emplace("hellscape",true);
  
  assert(rdx.contains(std::string("hello")));
  assert(!rdx.contains(std::string("hell")));
  assert(rdx.contains("hello"));
  assert(!rdx.contains("hell"));
  
  rdx.emplace("hell",true);

  assert(rdx.contains("hell"));

  return 0;
}

