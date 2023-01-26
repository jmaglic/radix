#include <string>
#include <cassert>
#include <algorithm>
#include "radix.hpp"

int main(){

  // Construct from initialiser list
  {
    std::string key_list[] = {"what", "a", "cool", "way", "to", "initialise", "my", "map"};

    xsm::radix<int> rdx({ 
      { key_list[0], 12 }, 
      { key_list[1], 4 }, 
      { key_list[2], 9 }, 
      { key_list[3], -42 }, 
      { key_list[4], 129 }, 
      { key_list[5], 0 }, 
      { key_list[6], 2 }, 
      { key_list[7], 4 } 
    });

    std::sort(std::begin(key_list), std::end(key_list));

    size_t i = 0;
    for (auto e : rdx){
      assert(e.first == key_list[i]);
      ++i;
    }
    assert(rdx.size() == std::size(key_list));
  }
}
