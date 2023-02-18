#include <iostream>
#include <map>
#include <vector>

#include "radix.hpp"

int main(){

  {
    // NODE HANDLE
    
    typedef xsm::radix<int> radix;

    radix::node_type nh;

    if (nh.empty()){
      std::cout << "Empty" << std::endl;
    }

    if (!nh){
      std::cout << "Empty" << std::endl;
    }

    radix rdx;

    rdx.emplace("hello", 2);
    rdx.emplace("hi", 3);
    rdx.emplace("hel", 4);
    rdx.emplace("h", 4);
    
    for (auto e : rdx) {
      std::cout << e.first << std::endl;
    }

    auto ex_nh = rdx.extract("hel");

    std::cout << "Extracted node with key: " << ex_nh.key() << std::endl;

    for (auto e : rdx) {
      std::cout << e.first << std::endl;
    }

    ex_nh = rdx.extract("h");
    std::cout << "Extracted node with key: " << ex_nh.key() << std::endl;

    ex_nh = rdx.extract("");

    for (auto e : rdx) {
      std::cout << e.first << std::endl;
    }
  }

  // TODO: Test rdx.extract("");
  // May cause deletion of the entire tree?

  // RADIX EXTRACT

  if (false) {
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

  rdx.insert(std::move(nh_a));

  std::cout << "Contents" << std::endl;
  for (auto e : rdx){
    std::cout << e.first << std::endl;
  }

  // Call node move constructor
  auto nh_movecon(std::move(nh_b));
  std::cout << "Moved node handle: " << nh_movecon.key() << std::endl;

  rdx.insert(std::move(nh_movecon));

  auto nh_moveassign = radix::node_type();
  nh_moveassign = std::move(nh_c);
  std::cout << "Move assigned handle: " << nh_moveassign.key() << std::endl;
  std::cout << "Move assigned handle: " << nh_moveassign.mapped() << std::endl;

  rdx.emplace("c", 13);
  // Fail insert
  rdx.insert(std::move(nh_moveassign));

  std::cout << rdx["c"] << std::endl;

  std::cout << (nh_moveassign.empty()? "Empty" : "WHAT") << std::endl;

  }

  // Node handle is not empty

  if (true) {
    //typedef std::map<std::string, int> radix;
    typedef xsm::radix<int> radix;

    radix rdx({{"hello", 1}, {"hell", 2}, {"hellscape", 3}});

    std::cout << "Contents" << std::endl;
    for (auto e : rdx){
      std::cout << e.first << std::endl;
    }

    auto nh = rdx.extract(rdx.find("hell"));

    std::cout << "Node handle" << std::endl;
    std::cout << nh.key() << ": " << nh.mapped() << std::endl;

    auto moved_nh = radix::node_type();
    moved_nh = std::move(nh);

    std::cout << "Old Node handle" << std::endl;
    std::cout << (nh.empty()? "Empty" : "Huh?") << std::endl;
    //std::cout << nh.key() << ": " << nh.mapped() << std::endl;

    std::cout << "New Node handle" << std::endl;
    std::cout << moved_nh.key() << ": " << moved_nh.mapped() << std::endl;

    rdx.emplace("hell", 31);

    rdx.insert(std::move(moved_nh));

    std::cout << "Old Node handle" << std::endl;
    //std::cout << nh.key() << ": " << nh.mapped() << std::endl;
    std::cout << (nh.empty()? "Empty" : "Huh?") << std::endl;

    std::cout << "New Node handle" << std::endl;
    std::cout << (moved_nh.empty()? "Empty" : "Huh?") << std::endl;
    //std::cout << moved_nh.key() << ": " << moved_nh.mapped() << std::endl;
  }

 
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

