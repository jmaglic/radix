#include <iostream>
#include <map>
#include <vector>

#include "radix.hpp"
#include "custom_comp.hpp"

int main() {

  //typedef std::map<std::string,bool,xsm::comp::CompK> map_type;
  typedef xsm::radix<bool,xsm::comp::CompK> map_type; 

  map_type map;
  map.emplace("art", true);
  map.emplace("beast", true);
  map.emplace("kilo", true);
  map.emplace("koala", true);
  map.emplace("knight", true);
  map.emplace("zebra", true);

  std::cout << map.count(xsm::comp::StartsWithK()) << std::endl;

  auto it = map.lower_bound(xsm::comp::StartsWithK());

  int i = 0;
  while (it != map.upper_bound(xsm::comp::StartsWithK())){
    ++i;
    ++it;
  }
  std::cout << i << std::endl;


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

