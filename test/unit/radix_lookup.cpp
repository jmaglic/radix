#include "radix.hpp"
#include "custom_comp.hpp"

int main() {
  {   
    typedef xsm::radix<int> radix;
    //typedef const std::map<std::string,int> radix;

    std::initializer_list<std::pair<const std::string,int>> init_list 
      = {{"waste", 123}, {"water", 22}, {"watt", 93}, {"worm", 2}, {"wormhole", 3}};
    radix rdx(init_list);

    const radix c_rdx = rdx;

    // Test basic const lookups
    for (auto e : init_list){
      auto c_it = c_rdx.find(e.first);
      assert(c_it->first == e.first);
      assert(c_it->second == e.second);

      assert(c_it == c_rdx.lower_bound(e.first));
      assert(++c_it == c_rdx.upper_bound(e.first));

      auto it = rdx.find(e.first);
      assert(it->first == e.first);
      assert(it->second == e.second);

      assert(it == rdx.lower_bound(e.first));
      assert(++it == rdx.upper_bound(e.first));
    }

    {
      // Test key that is not contained in map
      const std::string unknown_key = "heaven";
      assert(c_rdx.find(unknown_key) == c_rdx.cend());
      assert(c_rdx.lower_bound(unknown_key) == c_rdx.find(std::string("waste")));
      assert(c_rdx.upper_bound(unknown_key) == c_rdx.find(std::string("waste")));
      
      assert(rdx.find(unknown_key) == rdx.cend());
      assert(rdx.lower_bound(unknown_key) == rdx.find(std::string("waste")));
      assert(rdx.upper_bound(unknown_key) == rdx.find(std::string("waste")));
    }

    {
      // Test unknown key that comes after map contents
      const std::string unknown_key = "zebra";
      assert(c_rdx.find(unknown_key) == c_rdx.cend());
      assert(c_rdx.lower_bound(unknown_key) == c_rdx.end());
      assert(c_rdx.upper_bound(unknown_key) == c_rdx.end());

      assert(rdx.find(unknown_key) == rdx.cend());
      assert(rdx.lower_bound(unknown_key) == rdx.end());
      assert(rdx.upper_bound(unknown_key) == rdx.end());
    }

    {
      // Test unknown key that comes in between map contents
      const std::string unknown_key = "wire";
      assert(c_rdx.find(unknown_key) == c_rdx.cend());
      assert(c_rdx.lower_bound(unknown_key) == c_rdx.find(std::string("worm")));
      assert(c_rdx.upper_bound(unknown_key) == c_rdx.find(std::string("worm")));

      assert(rdx.find(unknown_key) == rdx.cend());
      assert(rdx.lower_bound(unknown_key) == rdx.find(std::string("worm")));
      assert(rdx.upper_bound(unknown_key) == rdx.find(std::string("worm")));
    }
  }
}
