#include <cassert>
#include <algorithm>
#include "commonwords.hpp"
#include "radix.hpp"

int main() {

  // Import 10000 words
  auto [common_words, import_success] = commonwords::readWords();
  if (!import_success){
    commonwords::failedImport();
  }

  typedef xsm::radix<int> radix_int;

  // Pass value as const lvalue reference of type value_type
  {
    radix_int rdx;

    for (const std::string& word : common_words){

      const radix_int::value_type cval(word, 123);
      auto [it, inserted] = rdx.insert(cval);

      assert(it->first == word);
      assert(it->second);
    }
    assert(rdx.size() == common_words.size() || rdx.size() == common_words.size()+1);
  }
    
  // Pass value as rvalue reference of type value_type
  {
    radix_int rdx;

    for (const std::string& word : common_words){

      auto [it, inserted] = rdx.insert(radix_int::value_type(word, 123));

      assert(it->first == word);
      assert(it->second);
    }
    assert(rdx.size() == common_words.size() || rdx.size() == common_words.size()+1);
  }
    
  // Pass value as rvalue reference of template type
  {
    radix_int rdx;

    for (const std::string& word : common_words){

      auto [it, inserted] = rdx.insert(std::make_pair(word, 123));

      assert(it->first == word);
      assert(it->second);
    }
    assert(rdx.size() == common_words.size() || rdx.size() == common_words.size()+1);
  }

  // Insert from array
  {
    typedef xsm::radix<bool> radix_bool;
  
    radix_bool rdx;
  
    std::pair<std::string,bool> value_array[3];
  
    std::string key_list[] = {"hey", "hello", "hi"};
  
    for (size_t i = 0; i < std::size(value_array); ++i){
      value_array[i] = std::make_pair(key_list[i], true);
    }
  
    rdx.insert(std::begin(value_array), std::end(value_array));
  
    std::sort(std::begin(value_array), std::end(value_array));
  
    size_t i = 0;
    for (auto e : rdx){
      assert(e.first == value_array[i].first);
      ++i;
    }
  }

  // Insert from initialiser list
  {
    xsm::radix<int> rdx; 

    std::string key_list[] = {"what", "a", "cool", "way", "to", "initialise", "my", "map"};

    rdx.insert({ 
      { key_list[0], 12 }, 
      { key_list[1], 4 }, 
      { key_list[2], 9 }, 
      { key_list[3], -42 }, 
      { key_list[4], 129 }, 
      { key_list[5], 0 }, 
      { key_list[6], 2 }, 
      { key_list[7], 4 } 
    });

    std::sort(std::begin(key_list), std::end(key_list));

    size_t i = 0;
    for (auto e : rdx){
      assert(e.first == key_list[i]);
      ++i;
    }
  }
    
  return 0;
}

