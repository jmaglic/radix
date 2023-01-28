#include <iostream>
#include <map>
#include <vector>

#include "radix.hpp"

int main(){

  //typedef std::map<std::string, int> radix;
  typedef xsm::radix<int> radix;

  radix rdx;

  auto [it, suc] = rdx.emplace("a", 12);
  rdx.emplace("b", 13);
  rdx.emplace("ba", 14);
  rdx.emplace("bb", 15);
  rdx.emplace("c", 16);
  rdx.emplace("ca", 17);
  rdx.emplace("cb", 18);
  rdx.emplace("cc", 19);

  std::cout << "Contents" << std::endl;
  for (auto e : rdx){
    std::cout << e.first << std::endl;
  }

  auto nh_a = rdx.extract(it);
  auto nh_b = rdx.extract(std::string("b"));
  auto nh_c = rdx.extract("c");

  std::cout << "Contents" << std::endl;
  for (auto e : rdx){
    std::cout << e.first << std::endl;
  }

  /*
  struct IsEven {};
  struct IsOdd {};

  struct Comp{
    using is_transparent = void;
    bool operator()(int lhs, int rhs) const {
      return lhs < rhs;
    }
    bool operator()(int lhs, IsEven rhs) const {
      return lhs%2;
    }
    bool operator()(IsEven lhs, int rhs) const {
      return rhs%2;
    }
    bool operator()(int lhs, IsOdd rhs) const {
      return (lhs+1)%2;
    }
    bool operator()(IsOdd lhs, int rhs) const {
      return (rhs+1)%2;
    }
  };

  std::map<int,bool,Comp> map;
  map.emplace(1,true);
  map.emplace(2,true);
  map.emplace(3,true);
  map.emplace(4,true);

  //std::cout << map.lower_bound(IsEven())->first << std::endl;
  //std::cout << map.upper_bound(IsEven())->first << std::endl;

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

