#include <string>
#include "radix.hpp"

// Changing value using at()
int main(){
  xsm::radix<std::string> rdx;
  std::string name1 = "jasmin";
  std::string name2 = "alexandra";
  rdx.emplace("name", name1);

  assert(rdx.at("name") == name1);

  rdx.at("name") = name2;

  assert(rdx.at("name") == name2);
  return 0;
}

