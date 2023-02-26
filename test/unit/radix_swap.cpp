#include "radix.hpp"

int main() {

  xsm::radix<int> rdx({{"aa", 1}, {"ab", 2}, {"a", 3}});

  xsm::radix<int> rdx2;

  rdx.swap(rdx2);

  assert(rdx2.contains("aa"));
  assert(rdx2.contains("ab"));
  assert(rdx2.contains("a"));
  assert(rdx2.at("aa") == 1);
  assert(rdx2.at("ab") == 2);
  assert(rdx2.at("a") == 3);
  assert(rdx2.size() == 3);
  assert(rdx.empty());

  return 0;
}

