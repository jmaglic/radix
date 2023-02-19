#include <iostream>
#include <map>
#include <vector>

#include "commonwords.hpp"
#include "radix.hpp"

int main(){
    
  typedef xsm::radix<int> radix;
  //typedef std::map<std::string,int> radix;

  {
    // Node handle manipulation

    // Default constructor

    radix::node_type nh;
    assert(nh.empty());
    assert(!nh);

    // Get node handle from radix::extract

    radix rdx({{"hello", 2}, {"hi", 3}, {"hel", 4}, {"h", 12}});
   
    auto nh_ex = rdx.extract("hel");

    assert(nh_ex.key() == "hel");
    assert(nh_ex.mapped() == 4);

    // Move assignment

    nh = std::move(nh_ex);

    assert(nh_ex.empty());
    assert(nh.key() == "hel");
    assert(nh.mapped() == 4);

    // Move construction
    
    nh_ex = rdx.extract("hi");

    assert(nh_ex.key() == "hi");
    assert(nh_ex.mapped() == 3);

    radix::node_type nh_constructed(std::move(nh_ex));

    assert(nh_ex.empty());
    assert(nh_constructed.key() == "hi");
    assert(nh_constructed.mapped() == 3);

  }

  {
    // Extract and insert 

    radix rdx({{"a", 12}, {"b", 13}, {"ba", 14}, {"bb", 15}, 
        {"c", 16}, {"ca", 17}, {"cb", 18}, {"cc", 19}});

    assert(rdx.size() == 8);

    // Extract overloads
    auto nh_a = rdx.extract(rdx.find("a"));
    auto nh_b = rdx.extract(std::string("b"));
    auto nh_c = rdx.extract("c");

    assert(rdx.size() == 5);

    {
      // Insert node handle
      
      auto insert_return = rdx.insert(std::move(nh_a));

      assert(rdx.size() == 6);
      assert(rdx.contains("a"));
      assert(insert_return.position == rdx.find("a"));
      assert(insert_return.node.empty());
      assert(insert_return.inserted);
    }

    {
      // Move node, then insert
      
      auto nh_movecon(std::move(nh_b));

      assert(nh_b.empty());
      assert(nh_movecon.key() == "b");
      assert(nh_movecon.mapped() == 13);

      auto insert_return = rdx.insert(std::move(nh_movecon));

      assert(rdx.size() == 7);
      assert(rdx.contains("b"));
      assert(insert_return.position == rdx.find("b"));
      assert(insert_return.node.empty());
      assert(insert_return.inserted);
      assert(nh_movecon.empty());

    }

    {
      // Failed insert
      
      rdx.emplace("c", 13);

      auto insert_return = rdx.insert(std::move(nh_c));

      assert(rdx.size() == 8);
      assert(rdx.contains("c"));
      assert(rdx["c"] == 13);
      assert(insert_return.position == rdx.find("c"));
      assert(!insert_return.node.empty());
      assert(insert_return.node.key() == "c");
      assert(!insert_return.inserted);
      assert(nh_c.empty());

    }
  }

  {
    // Test situations where node handle becomes empty

    radix rdx({{"hello", 1}, {"hell", 2}, {"hellscape", 3}});

    auto nh = rdx.extract(rdx.find("hell"));

    assert(nh.key() == "hell");
    assert(nh.mapped() == 2);

    // Move node
    auto moved_nh = std::move(nh);

    assert(nh.empty());
    assert(moved_nh.key() == "hell");
    assert(moved_nh.mapped() == 2);

    // Block key in rdx
    rdx.emplace("hell", 31);

    auto ret = rdx.insert(std::move(moved_nh));

    assert(!ret.inserted);
    assert(ret.position == rdx.find("hell"));
    assert(rdx.at("hell") == 31);
    assert(nh.empty());
    assert(moved_nh.empty());
  }

  {
    // Insert node handle with hint
    
    auto [common_words, imported] = commonwords::readWords();
    if (!imported){
      commonwords::failedImport();
    }

    radix rdx;

    for (const std::string& word : common_words){
      rdx.emplace(word, 1);
    }

    radix rdx_2nd;
    auto it = rdx_2nd.end();

    for (const std::string& word : common_words){
      auto nh = rdx.extract(word);
      it = rdx_2nd.insert(it, std::move(nh));

      assert(it->first == word);
    }
    assert(rdx_2nd.size() == 10000);
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

