
#include "radix.hpp"
#include <cassert>

int main(){
  //typedef std::map<std::string,bool,xsm::comp::CompK> map_type;
  typedef xsm::radix<bool> map_type; 

  {
    // Complete for various search keys
    map_type map;
    map.emplace("art", true);
    map.emplace("beast", true);
    map.emplace("kilo", true);
    map.emplace("koala", true);
    map.emplace("knight", true);
    map.emplace("zebra", true);

    map_type::iterator b_it, e_it;
    std::tie(b_it, e_it) = map.complete("k");

    assert(b_it->first == "kilo");
    assert((++b_it)->first == "knight");
    assert((++b_it)->first == "koala");
    assert(++b_it == e_it);

    std::tie(b_it, e_it) = map.complete("ar");
    assert(b_it->first == "art");
    assert(++b_it == e_it);

    std::tie(b_it, e_it) = map.complete("l");
    assert(++b_it == e_it);

    // Complete in empty map
    map_type empty_map;

    std::tie(b_it, e_it) = empty_map.complete("k");
    assert(b_it == e_it);
  }

  // Same tests but for const map
  {
    // Complete for various search keys
    const map_type map({
        {"art", true},
        {"beast", true},
        {"kilo", true},
        {"koala", true},
        {"knight", true},
        {"zebra", true}
      });


    map_type::const_iterator b_it, e_it;
    std::tie(b_it, e_it) = map.complete("k");

    assert(b_it->first == "kilo");
    assert((++b_it)->first == "knight");
    assert((++b_it)->first == "koala");
    assert(++b_it == e_it);

    std::tie(b_it, e_it) = map.complete("ar");
    assert(b_it->first == "art");
    assert(++b_it == e_it);

    std::tie(b_it, e_it) = map.complete("l");
    assert(++b_it == e_it);

    // Complete in empty map
    const map_type empty_map;

    std::tie(b_it, e_it) = empty_map.complete("k");
    assert(b_it == e_it);
  }
}
