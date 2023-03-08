#include <iostream>
#include <map>
#include <vector>

#include "radix.hpp"

int main() {
  
  struct StartsWithK {};
  
  struct CompK{
    using is_transparent = void;
    bool operator()(std::string lhs, std::string rhs) const {
      return lhs.compare(rhs) < 0;
    }
    bool operator()(std::string lhs, StartsWithK rhs) const {
      return tolower(lhs[0]) < 'k';
    }
    bool operator()(StartsWithK lhs, std::string rhs) const {
      return 'k' < tolower(rhs[0]);
    }
  };

  struct CompInv{
    using is_transparent = void;
    bool operator()(std::string lhs, std::string rhs) const {
      return lhs.compare(rhs) > 0; // Inverted order
    }
  };

  // Test xsm::radix::contains with custom comparator to find element that starts with a letter
  {
    typedef xsm::radix<bool,CompK> radix;
  
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
    typedef xsm::radix<bool,CompInv> radix;

    radix rdx({{"a", true}, {"b", true}, {"c", true}});

    std::string keys[] = {"c", "b", "a"};
    size_t i = 0;
    for (auto e : rdx){
      assert(e.first == keys[i++]);
    }
  }

  // Test find
  {
    typedef xsm::radix<bool,CompK> radix;

    radix rdx({{"denmark", true}, {"korea", true}, {"kazakhstan", true}, {"togo", true}});
    
    assert(rdx.find("kazakhstan") == rdx.find(StartsWithK()));

    rdx.erase("kazakhstan");

    assert(rdx.find("korea") == rdx.find(StartsWithK()));

    rdx.erase("korea");

    assert(rdx.find("korea") == rdx.end());
    assert(rdx.find(StartsWithK()) == rdx.end());

    // Const overload
    const radix c_rdx({{"denmark", true}, {"korea", true}, {"kazakhstan", true}, {"togo", true}});

    assert(c_rdx.find("kazakhstan") == c_rdx.find(StartsWithK()));
  }

  // Test lookup functions with templated key and const radix
  {
    typedef const xsm::radix<bool,CompK> radix;
    
    // Range of matches exists in tree
    {
      // "kazakhstan" and "korea" should match with StartsWithK
      radix rdx({{"denmark", true}, {"korea", true}, {"kazakhstan", true}, {"togo", true}});

      assert(rdx.lower_bound(StartsWithK()) == rdx.find("kazakhstan"));
      assert(rdx.upper_bound(StartsWithK()) == rdx.find("togo"));
      assert(rdx.find(StartsWithK()) == rdx.find("kazakhstan"));

    }
  }

  {
    typedef const xsm::radix<bool> c_radix;
    c_radix c_rdx({{"hello", true}, {"hi", true}, {"hey", true}});

    std::cout << c_rdx.find(std::string("hello"))->first << std::endl;

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

