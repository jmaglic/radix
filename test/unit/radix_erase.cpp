#include "radix.hpp"

// Use of erase()
int main() {
  {
    xsm::radix<bool> rdx({{"well", true}, {"water", true}, {"waste", true}, {"wa", true}});
    
    // Validate insertion
    assert(rdx.contains("well"));
    assert(rdx.contains("water"));
    assert(rdx.contains("waste"));
    assert(rdx.contains("wa"));
    assert(rdx.size() == 4);

    // Basic erase
    auto it = rdx.erase(rdx.find("wa"));
    assert(!rdx.contains("wa"));
    assert(it == rdx.find("waste"));
    assert(rdx.size() == 3);

    it = rdx.erase(rdx.find("well"));
    assert(!rdx.contains("well"));
    assert(it == rdx.end());
    assert(rdx.size() == 2);
    
    it = rdx.erase(rdx.find("water"));
    assert(!rdx.contains("water"));
    assert(it == rdx.end());
    assert(rdx.size() == 1);
    
    it = rdx.erase(rdx.find("waste"));
    assert(!rdx.contains("waste"));
    assert(it == rdx.end());
    assert(rdx.size() == 0);
    assert(rdx.empty());

    // Correct return iterator
    rdx.emplace("hello", true);
    rdx.emplace("hell", true);
    rdx.emplace("he", true);
    assert(rdx.size() == 3);

    it = rdx.erase(rdx.find("he"));
    assert(it->first == "hell");
    it = rdx.erase(it);
    assert(it->first == "hello");

    assert(rdx.contains("hello"));
    assert(!rdx.contains("hell"));
    assert(!rdx.contains("he"));
    assert(rdx.size() == 1);

  }
  {
    xsm::radix<bool> rdx;
    
    // erase(iterator)
    rdx.emplace("hello",true);
    rdx.erase(rdx.begin());
    assert(!rdx.contains("hello"));
    assert(rdx.size() == 0);

    // erase(const iterator)
    rdx.emplace("hello",true);
    rdx.erase(rdx.cbegin());
    assert(!rdx.contains("hello"));
    assert(rdx.size() == 0);

    // Range erase
    rdx.insert(std::make_pair("a",true));
    rdx.insert(std::make_pair("aa",true));
    rdx.insert(std::make_pair("aaa",true));
    rdx.insert(std::make_pair("aaaa",true));

    auto it = rdx.erase(rdx.find("a"), rdx.find("aaa"));
    for (auto elem : rdx){
      assert(!rdx.contains("a"));
      assert(!rdx.contains("aa"));
      assert(rdx.contains("aaa"));
      assert(rdx.contains("aaaa"));
    }
    assert(it->first == "aaa");

    // Erase using key
    assert(1 == rdx.erase("aaa"));
    assert(0 == rdx.erase("aaa"));
  }
  return 0;
}

