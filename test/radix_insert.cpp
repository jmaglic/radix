#include "radix.hpp"

int main() {
  xsm::radix<bool> rdx;
  bool success = true;
  // Insert const lvalue reference
  {
    auto [it,inserted] = rdx.insert(std::make_pair("aaa",true));
    success &= it->first == "aaa";
    success &= inserted;
  }
  // Insert rvalue reference
  {
    auto [it,inserted] = rdx.insert(std::move(std::make_pair("bbb",true)));
    success &= it->first == "bbb";
    success &= inserted;
  }
  return success? 0 : -1;
}
