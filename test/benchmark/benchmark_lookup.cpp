#include <vector>
#include <functional>
#include <algorithm>
#include "commonwords.hpp"
#include "benchmark.hpp"
#include "radix.hpp"

int main(){

  // TODO:
  // All lower_bound
  // All upper_bound
  // All find
  // All contains
  // All equal_range
  // All count

  // Import common words
  std::vector<std::string> common_words;
  bool import_success;
  std::tie(common_words, import_success) = commonwords::readWords();
  if (!import_success){
    commonwords::failedImport();
  }

  // Fill radix
  typedef xsm::radix<int> radix;
  //typedef std::map<std::string,int> radix;
  radix rdx;

  for (const std::string& word : common_words){
    rdx.emplace(word, 1);
  }

  // find()
  std::function<void()> find = [common_words, rdx]() mutable {
    for (const std::string& word : common_words){
      rdx.find(word);
    }
  };

  // find() const
  std::function<void()> const_find = [common_words, rdx]() {
    for (const std::string& word : common_words){
      rdx.find(word);
    }
  };

  // template find()
  std::function<void()> find_template = [common_words, rdx]() mutable {
    for (const std::string& word : common_words){
      rdx.find(word.c_str());
    }
  };

  // template find() const
  std::function<void()> const_find_template = [common_words, rdx]() {
    for (const std::string& word : common_words){
      const char *array = &word[0];
      rdx.find(array);
    }
  };

  benchmark::time(find, "radix find");
  benchmark::time(const_find, "radix find const");
  benchmark::time(find_template, "radix template find");
  benchmark::time(const_find_template, "radix template find const");
    
}

