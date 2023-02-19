
#include "radix.hpp"

int main(){
    
  typedef xsm::radix<int> radix;

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

    radix rdx({{"a", 1}, {"aa", 2}, {"ab", 3}});

    auto nh = rdx.extract("a");
    auto it = rdx.insert(rdx.find("ab"), std::move(nh));

    assert(it->first == "a");
    assert(it->second == 1);
  }

}

