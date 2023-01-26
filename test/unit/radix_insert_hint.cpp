#include <string>
#include <cassert>
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

    // Insert first word manually to get first hint iterator
    auto [it, insert_success] = rdx.insert(std::make_pair("first", 12));

    for (const std::string& word : common_words){

      const radix_int::value_type cval(word, 123);
      it = rdx.insert(it, cval);

      assert(it->first == word);
      assert(it->second);
    }
    assert(rdx.size() == common_words.size() || rdx.size() == common_words.size()+1);
  }
    
  // Pass value as rvalue reference of type value_type
  {
    radix_int rdx;

    // Insert first word manually to get first hint iterator
    auto [it, insert_success] = rdx.insert(std::make_pair("first", 12));

    for (const std::string& word : common_words){

      it = rdx.insert(it, radix_int::value_type(word, 123));

      assert(it->first == word);
      assert(it->second);
    }
    assert(rdx.size() == common_words.size() || rdx.size() == common_words.size()+1);
  }
    
  // Pass value as rvalue reference of template type
  {
    radix_int rdx;

    // Insert first word manually to get first hint iterator
    auto [it, insert_success] = rdx.insert(std::make_pair("first", 12));

    for (const std::string& word : common_words){

      it = rdx.insert(it, std::make_pair(word, 123));

      assert(it->first == word);
      assert(it->second);
    }
    assert(rdx.size() == common_words.size() || rdx.size() == common_words.size()+1);
  }
    
  return 0;
}
