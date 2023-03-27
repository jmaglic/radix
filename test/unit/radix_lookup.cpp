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

      radix::const_iterator it_lr, it_ur;
      std::tie(it_lr, it_ur) = c_rdx.equal_range(e.first);
      assert(it_lr == c_rdx.lower_bound(e.first));
      assert(it_ur == c_rdx.upper_bound(e.first));

      auto it = rdx.find(e.first);
      assert(it->first == e.first);
      assert(it->second == e.second);

      assert(it == rdx.lower_bound(e.first));
      assert(++it == rdx.upper_bound(e.first));

      std::tie(it_lr, it_ur) = rdx.equal_range(e.first);
      assert(it_lr == rdx.lower_bound(e.first));
      assert(it_ur == rdx.upper_bound(e.first));
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
  
  // Testing lookup for radix and const radix with template key and custom comparator
  {   

    using StartsWithK = xsm::comp::StartsWithK;

    typedef xsm::radix<int, xsm::comp::CompK> radix;
    //typedef const std::map<std::string,int, xsm::comp::CompK> radix;

    std::initializer_list<std::pair<const std::string,int>> init_list 
      = {{"king", 123}, {"knight", 22}, {"rook", 93}, {"queen", 2}, {"bishop", 3}, {"pawn", 2}};
    radix rdx(init_list);

    const radix c_rdx = rdx;

    assert(rdx.find("king") == rdx.find(std::string("king")));
    assert(rdx.find(StartsWithK()) == rdx.find(std::string("king")));
    assert(c_rdx.find("king") == c_rdx.find(std::string("king")));
    assert(c_rdx.find(StartsWithK()) == c_rdx.find(std::string("king")));

    assert(rdx.lower_bound("king") == rdx.find(std::string("king")));
    assert(rdx.lower_bound(StartsWithK()) == rdx.find(std::string("king")));
    assert(c_rdx.lower_bound("king") == c_rdx.find(std::string("king")));
    assert(c_rdx.lower_bound(StartsWithK()) == c_rdx.find(std::string("king")));

    assert(rdx.upper_bound("king") == rdx.find(std::string("knight")));
    assert(rdx.upper_bound(StartsWithK()) == rdx.find(std::string("pawn")));
    assert(c_rdx.upper_bound("king") == c_rdx.find(std::string("knight")));
    assert(c_rdx.upper_bound(StartsWithK()) == c_rdx.find(std::string("pawn")));

    radix::const_iterator it_lr, it_ur;
    std::tie(it_lr, it_ur) = rdx.equal_range(StartsWithK());
    assert(it_lr == rdx.lower_bound(StartsWithK()));
    assert(it_ur == rdx.upper_bound(StartsWithK()));
    std::tie(it_lr, it_ur) = c_rdx.equal_range(StartsWithK());
    assert(it_lr == c_rdx.lower_bound(StartsWithK()));
    assert(it_ur == c_rdx.upper_bound(StartsWithK()));
  }
}
