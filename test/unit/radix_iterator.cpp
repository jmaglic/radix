#include <vector>
#include <string>
#include <cassert>
#include "radix.hpp"

int main() {
  typedef xsm::radix<bool> radix;
  radix rdx;
  std::vector<std::string> keys = {"a", "aa", "ab", "abb", "abc", "abd", "abde", "abdf"};

  // Insert
  size_t i = 0;
  for (i = 0; i < keys.size(); ++i){
    rdx.emplace(keys[i], true);
  }

  // begin and end
  i = 0;
  for (auto it = rdx.begin(); it != rdx.end(); ++it){
    assert(it->first == keys[i]);
    assert((*it).first == keys[i]);
    ++i;
  }

  // rbegin and rend
  i = keys.size()-1;
  for (auto it = rdx.rbegin(); it != rdx.rend(); ++it){
    assert(it->first == keys[i]);
    assert((*it).first == keys[i]);
    --i;
  }

  const xsm::radix<bool> c_rdx = rdx;

  // begin and end (const radix)
  i = 0;
  for (auto it = c_rdx.begin(); it != c_rdx.end(); ++it){
    assert(it->first == keys[i]);
    assert((*it).first == keys[i]);
    ++i;
  }

  // rbegin and rend (const radix)
  i = keys.size()-1;
  for (auto it = c_rdx.rbegin(); it != c_rdx.rend(); ++it){
    assert(it->first == keys[i]);
    assert((*it).first == keys[i]);
    --i;
  }

  // cbegin and cend
  i = 0;
  for (auto it = rdx.cbegin(); it != rdx.cend(); ++it){
    assert(it->first == keys[i]);
    assert((*it).first == keys[i]);
    ++i;
  }

  // crbegin and crend
  i = keys.size()-1;
  for (auto it = rdx.crbegin(); it != rdx.crend(); ++it){
    assert(it->first == keys[i]);
    assert((*it).first == keys[i]);
    --i;
  }
  return 0;
}

