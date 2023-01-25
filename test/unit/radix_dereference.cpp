#include <string>
#include "radix.hpp"

// Use of -> and * operators
int main() {
  xsm::radix<std::string> rdx;
  auto it = rdx.insert(std::make_pair("name", "gordon")).first;

  bool success = true;
  // operator->
  success &= it->first == "name";
  success &= it->second == "gordon";
  // operator*
  success &= (*it).first == "name";
  success &= (*it).second == "gordon";
  // mutability
  it->second = "william";
  success &= rdx.at("name") == "william";
  // mutability
  (*it).second = "elija";
  success &= rdx.at("name") == "elija";

  return success? 0 : -1;
}

