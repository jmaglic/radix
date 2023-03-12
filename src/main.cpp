#include <iostream>
#include <map>
#include <vector>

#include "radix.hpp"
#include "custom_comp.hpp"

int main() {

  /* Custom comparator test
   *
  using StartsWithK = xsm::comp::StartsWithK;
  
  // Test xsm::radix::contains with custom comparator to find element that starts with a letter
  {
    typedef xsm::radix<bool,xsm::comp::CompK> radix;
  
    radix rdx({{"albert", true}, {"kristina", false}, {"sarah", true}});
  
    assert(rdx.count("albert"));
    assert(rdx.count("kristina"));
    assert(rdx.count("sarah"));
    assert(rdx.contains(StartsWithK()));
    assert(rdx.count(StartsWithK()));
  
    rdx.erase(rdx.find("kristina"));
  
    assert(rdx.count("albert"));
    assert(!rdx.count("kristina"));
    assert(rdx.count("sarah"));
    assert(!rdx.contains(StartsWithK()));
    assert(!rdx.count(StartsWithK()));
  }

  // Test inverting order of tree
  {
    typedef xsm::radix<bool,xsm::comp::CompInv> radix;

    radix rdx({{"a", true}, {"b", true}, {"c", true}});

    std::string keys[] = {"c", "b", "a"};
    size_t i = 0;
    for (auto e : rdx){
      assert(e.first == keys[i++]);
    }
  }
  */

  // LOOKUP TESTS

  using StartsWithK = xsm::comp::StartsWithK;

  // count c
  // template count c
  // contains c
  // template contains c
  // /equal_range

  // const find
  // /const lower_bound
  // /const upper_bound
  
  // find
  // /lower_bound
  // /upper_bound

  // template const find
  // template const lower_bound
  // template const upper_bound
  
  // template find
  // template lower_bound
  // /template upper_bound

  // Testing lookup for const radix
  {   
    typedef const xsm::radix<int> radix;
    //typedef const std::map<std::string,int> radix;

    std::initializer_list<std::pair<const std::string,int>> init_list 
      = {{"waste", 123}, {"water", 22}, {"watt", 93}, {"worm", 2}, {"wormhole", 3}};
    radix c_rdx(init_list);

    // Test basic const lookups
    for (auto e : init_list){
      auto it = c_rdx.find(e.first);
      assert(it->first == e.first);
      assert(it->second == e.second);

      assert(it == c_rdx.lower_bound(e.first));
      assert(++it == c_rdx.upper_bound(e.first));
    }

    // Test key that is not contained in map
    const std::string unknown_key = "heaven";
    //assert(c_rdx.find(unknown_key) == c_rdx.cend());
    std::cout << (c_rdx.find(unknown_key) == c_rdx.cend()? 1 : 0) << std::endl;
    std::cout << c_rdx.lower_bound(unknown_key)->first << std::endl;
    std::cout << c_rdx.upper_bound(unknown_key)->first << std::endl;


  }
  //
  // 2. non-const radix
  //
  // 3. const radix with custom comp
  //
  // 4. non-const radix with custom comp

  /* ISSUE WITH STD::MAP
  radix rdx2nd;
  rdx2nd.insert({{"cool", 1}, {"story", 29}});
  auto it2nd = rdx2nd.find("cool");

  printf("Size -- rdx: %lu -- rdx2nd: %lu\n", rdx.size(), rdx2nd.size());
  auto what = rdx.extract(it2nd);
  printf("Size -- rdx: %lu -- rdx2nd: %lu\n", rdx.size(), rdx2nd.size());


  std::cout << "Contents rdx" << std::endl;
  for (auto e : rdx){
    std::cout << e.first << std::endl;
  }

  std::cout << "Contents rdx2nd" << std::endl;
  for (auto e : rdx2nd){
    std::cout << e.first << std::endl;
  }
*/






  /*
  auto it = rdx.begin();
  std::cout << it->first << std::endl;
  std::cout << (it++)->first << std::endl;
  std::cout << it->first << std::endl;
  std::cout << (++it)->first << std::endl;
  std::cout << it->first << std::endl;
*/
  
  /*
  for (auto elem : rdx){
    std::cout << elem.first << std::endl;
  }

  auto it = rdx.begin();
  std::cout << it->first << std::endl;
  std::advance(it,2);
  std::cout << it->first << std::endl;
*/

}

