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
  {
    // insert(P&&)
    std::string key = "anystring";
    int val = 5;

    std::pair<std::string, int>&& p = std::make_pair(key, val);

    xsm::radix<int> rdx;

    auto [it,inserted] = rdx.insert(p);
    assert(inserted && it->first == key && it->second == val);
  }
  return success? 0 : -1;
}
