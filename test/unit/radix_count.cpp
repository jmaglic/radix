#include "custom_comp.hpp"
#include "radix.hpp"

int main() {
  //typedef std::map<std::string,bool,xsm::comp::CompK> map_type;
  typedef xsm::radix<bool,xsm::comp::CompK> radix; 

  radix map({
    {"art", true},
    {"beast", true},
    {"kilo", true},
    {"koala", true},
    {"knight", true},
    {"zebra", true}
  });

  assert(map.count(std::string("art")));
  assert(map.count(std::string("beast")));
  assert(map.count(std::string("kilo")));
  assert(map.count(std::string("koala")));
  assert(map.count(std::string("knight")));
  assert(map.count(std::string("zebra")));
  assert(!map.count(std::string("orange")));

  assert(map.count(xsm::comp::StartsWithK()) == 3);

  map.erase("kilo");

  assert(map.count(xsm::comp::StartsWithK()) == 2);

  map.erase("koala");

  assert(map.count(xsm::comp::StartsWithK()) == 1);

  map.erase("knight");

  assert(map.count(xsm::comp::StartsWithK()) == 0);

}
