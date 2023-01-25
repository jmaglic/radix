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

  // Insert 10000 words using the previous insert as hint
  {
    xsm::radix<int> rdx;

    // Insert first word manually, because of issues with providing a hint when rdx is empty
    auto [it, insert_success] = rdx.insert(std::make_pair("first", 12));

    for (const std::string& word : common_words){

      it = rdx.insert(it, std::make_pair(word, 260));
      assert(it->first == word);
      assert(it->second);
    }
    assert(rdx.size() == common_words.size() || rdx.size() == common_words.size()+1);
  }
    
  return 0;
}
