#include "radix.hpp"

int main() {
  xsm::radix<bool> rdx;
  bool success = true;
  // Emplace entry
  {
    auto [it,emplaced] = rdx.emplace("hello",true);
    success &= it->first == "hello";
    success &= it->second;
    success &= emplaced;
  }
  // Attempt emplacing an element with an existing key
  {
    auto [it,emplaced] = rdx.emplace("hello",false);
    success &= it->first == "hello";
    success &= it->second;
    success &= !emplaced;
  }
  // New key is prefix of existing key
  {
    auto [it,emplaced] = rdx.emplace("hell",true);
    success &= it->first == "hell";
    success &= it->second;
    success &= emplaced;
  }
  // Keys diverge
  {
    auto [it,emplaced] = rdx.emplace("hero",true);
    success &= it->first == "hero";
    success &= it->second;
    success &= emplaced;
  }
  // Non-leaf node gets converted
  {
    success &= !rdx.contains("he");
    auto [it,emplaced] = rdx.emplace("he",true);
    success &= it->first == "he";
    success &= it->second;
    success &= emplaced;
  }
  // Existing key is a prefix of new key
  {
    auto [it,emplaced] = rdx.emplace("hellow",true);
    success &= it->first == "hellow";
    success &= it->second;
    success &= emplaced;
  }
  // Use pair's converting move constructor
  {
    auto [it,emplaced] = rdx.emplace(std::make_pair("here", true));
    success &= it->first == "here";
    success &= it->second;
    success &= emplaced;
  }
  return success? 0 : -1;
}

