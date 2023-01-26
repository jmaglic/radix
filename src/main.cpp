#include <iostream>
#include <map>
#include <vector>

#include "radix.hpp"

int main(){


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

