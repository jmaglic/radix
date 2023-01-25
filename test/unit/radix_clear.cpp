#include "radix.hpp"

// Use of clear() and empty()
int main() {
  xsm::radix<bool> rdx;
  bool success = true;
  success &= rdx.empty();
  
  rdx.insert("something",true);
  success &= !rdx.empty();

  rdx.clear();
  success &= rdx.empty();
  return success? 0 : -1;
}

