#include <cassert>
#include "radix.hpp"

int main() {

  assert(std::bidirectional_iterator<xsm::radix<int>::iterator>);
  assert(std::bidirectional_iterator<xsm::radix<bool>::iterator>);
  assert(std::bidirectional_iterator<xsm::radix<std::string>::iterator>);
  assert(std::bidirectional_iterator<xsm::radix<double>::iterator>);
  assert(std::bidirectional_iterator<xsm::radix<float>::iterator>);
  assert(std::bidirectional_iterator<xsm::radix<long>::iterator>);

  return 0;
}

