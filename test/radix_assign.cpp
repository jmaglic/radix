#include <vector>
#include <utility>
#include <algorithm>
#include "radix.hpp"

// Copy/Move constructors/assignment operators
int main() {
  // Init rdx
  xsm::radix<int> rdx;

  std::vector<std::string> keys = {"aaaa", "aabb", "aa", "abc", "abd"};
  std::vector<int> values = {1,1,1,1,1};

  for (size_t i = 0; i < keys.size(); ++i){
    rdx[keys[i]] = values[i];
  }
  
  std::sort(keys.begin(), keys.end());
  keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

  // Copy constructor
  xsm::radix<int> c_rdx = rdx;

  bool success = true;
  auto c_it = c_rdx.begin();
  for (auto it = rdx.begin(); it != rdx.end(); ++it){
    success &= it->first == c_it->first;
    success &= it->second == c_it->second;
    success &= it != c_it;
    ++c_it;
  }

  // Move assignment
  auto comp_it = rdx.begin();

  c_rdx = std::move(rdx);

  success &= comp_it == c_rdx.begin();

  // rdx should be empty
  success &= rdx.begin() == rdx.end();

  size_t i = 0;
  for (c_it = c_rdx.begin(); c_it != c_rdx.end(); ++c_it){
    success &= c_it->first == keys[i];
    ++i;
  }
 
  // Copy assignment
  comp_it = c_rdx.begin();

  rdx = c_rdx;
  
  success &= comp_it != rdx.begin();

  c_it = c_rdx.begin();
  for (auto it = rdx.begin(); it != rdx.end(); ++it){
    success &= it->first == c_it->first;
    success &= it->second == c_it->second;
    success &= it != c_it;
    ++c_it;
  }

  // Move constructor
  xsm::radix<int> n_rdx = std::move(rdx);
  
  success &= rdx.begin() == rdx.end();

  i = 0;
  for (auto n_it = n_rdx.begin(); n_it != n_rdx.end(); ++n_it){
    success &= n_it->first == keys[i];
    ++i;
  }
  return success? 0 : -1;
}

