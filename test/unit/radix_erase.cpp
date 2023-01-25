#include "radix.hpp"

// Use of erase()
int main() {
  bool success = true;
  {
    xsm::radix<bool> rdx;
    
    rdx.insert("well",true);
    rdx.insert("water",true);
    rdx.insert("waste",true);
    rdx.insert("wa",true);

    // Validate insertion
    success &= rdx.contains("well");
    success &= rdx.contains("water");
    success &= rdx.contains("waste");
    success &= rdx.contains("wa");
    success &= rdx.size() == 4;

    // Basic erase
    rdx.erase(rdx.find("wa"));
    success &= !rdx.contains("wa");
    success &= rdx.size() == 3;

    rdx.erase(rdx.find("well"));
    success &= !rdx.contains("well");
    success &= rdx.size() == 2;
    
    rdx.erase(rdx.find("water"));
    success &= !rdx.contains("water");
    success &= rdx.size() == 1;
    
    rdx.erase(rdx.find("waste"));
    success &= !rdx.contains("waste");
    success &= rdx.size() == 0;

    // Correct return iterator
    rdx.insert("hello", true);
    rdx.insert("hell", true);
    rdx.insert("he", true);
    success &= rdx.size() == 3;

    xsm::radix<bool>::iterator it = rdx.erase(rdx.find("he"));
    success &= it->first == "hell";
    it = rdx.erase(it);
    success &= it->first == "hello";

    success &= rdx.contains("hello");
    success &= !rdx.contains("hell");
    success &= !rdx.contains("he");
    success &= rdx.size() == 1;
  }
  {
    xsm::radix<bool> rdx;
    
    // erase(iterator)
    rdx.insert("hello",true);
    rdx.erase(rdx.begin());
    success &= !rdx.contains("hello");
    success &= rdx.size() == 0;

    // erase(const iterator)
    rdx.insert("hello",true);
    rdx.erase(rdx.cbegin());
    success &= !rdx.contains("hello");
    success &= rdx.size() == 0;

    // Range erase
    rdx.insert(std::make_pair("a",true));
    rdx.insert(std::make_pair("aa",true));
    rdx.insert(std::make_pair("aaa",true));
    rdx.insert(std::make_pair("aaaa",true));

    auto it = rdx.erase(rdx.find("a"), rdx.find("aaa"));
    for (auto elem : rdx){
      success &= !rdx.contains("a");
      success &= !rdx.contains("aa");
      success &= rdx.contains("aaa");
      success &= rdx.contains("aaaa");
    }
    success &= it->first == "aaa";

    // Erase using key
    success &= 1 == rdx.erase("aaa");
    success &= 0 == rdx.erase("aaa");
  }
  return success? 0 : -1;
}

