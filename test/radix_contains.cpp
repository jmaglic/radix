#include "radix.hpp"

// Use of contains()
int main() {
  bool success = true;
  xsm::radix<bool> rdx;
  rdx.insert("hello",true);
  rdx.insert("hellscape",true);
  
  success &= rdx.contains("hello");
  success &= !rdx.contains("hell");
  
  rdx.insert("hell",true);

  success &= rdx.contains("hell");

  return success? 0 : -1;
}

