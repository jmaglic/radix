#include <vector>
#include <functional>
#include <algorithm>
#include "commonwords.hpp"
#include "benchmark.hpp"
#include "radix.hpp"

int main(){

  // Import common words
  std::vector<std::string> common_words;
  bool import_success;
  std::tie(common_words, import_success) = commonwords::readWords();
  if (!import_success){
    commonwords::failedImport();
  }

  // Fill first radix
  typedef xsm::radix<int> radix;
  //typedef std::map<std::string,int> radix;
  radix rdx;

  for (const std::string& word : common_words){
    rdx.emplace(word, 1);
  }

  // Create second
  radix rdx_2nd;
  auto it = rdx_2nd.end();

  std::function<void()> extract_then_insert = [common_words, rdx, rdx_2nd, it]() mutable {
    // Extract from one tree and insert into other with hint

    for (const std::string& word : common_words){
      auto nh = rdx.extract(word);
      it = rdx_2nd.insert(it, std::move(nh));
    }
  };
    
}

