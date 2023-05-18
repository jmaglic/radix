#include <vector>
#include <functional>
#include <algorithm>
#include "commonwords.hpp"
#include "benchmark.hpp"
#include "radix.hpp"

int main(){

  // TODO:
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

  
  // lower_bound()
  std::function<void()> lower_bound = [common_words, rdx]() mutable {
    for (const std::string& word : common_words){
      rdx.lower_bound(word);
    }
  };

  // lower_bound() const
  std::function<void()> const_lower_bound = [common_words, rdx]() {
    for (const std::string& word : common_words){
      rdx.lower_bound(word);
    }
  };

  // template lower_bound()
  std::function<void()> lower_bound_template = [common_words, rdx]() mutable {
    for (const std::string& word : common_words){
      rdx.lower_bound(word.c_str());
    }
  };

  // template lower_bound() const
  std::function<void()> const_lower_bound_template = [common_words, rdx]() {
    for (const std::string& word : common_words){
      const char *array = &word[0];
      rdx.lower_bound(array);
    }
  };

  
  // upper_bound()
  std::function<void()> upper_bound = [common_words, rdx]() mutable {
    for (const std::string& word : common_words){
      rdx.upper_bound(word);
    }
  };

  // upper_bound() const
  std::function<void()> const_upper_bound = [common_words, rdx]() {
    for (const std::string& word : common_words){
      rdx.upper_bound(word);
    }
  };

  // template upper_bound()
  std::function<void()> upper_bound_template = [common_words, rdx]() mutable {
    for (const std::string& word : common_words){
      rdx.upper_bound(word.c_str());
    }
  };

  // template upper_bound() const
  std::function<void()> const_upper_bound_template = [common_words, rdx]() {
    for (const std::string& word : common_words){
      const char *array = &word[0];
      rdx.upper_bound(array);
    }
  };


  // contains() const
  std::function<void()> const_contains = [common_words, rdx]() {
    for (const std::string& word : common_words){
      rdx.contains(word);
    }
  };

  // template contains() const
  std::function<void()> const_contains_template = [common_words, rdx]() {
    for (const std::string& word : common_words){
      const char *array = &word[0];
      rdx.contains(array);
    }
  };

  benchmark::time(find, "radix find");
  benchmark::time(const_find, "radix find const");
  benchmark::time(find_template, "radix template find");
  benchmark::time(const_find_template, "radix template find const");
    
  benchmark::time(lower_bound, "radix lower_bound");
  benchmark::time(const_lower_bound, "radix lower_bound const");
  benchmark::time(lower_bound_template, "radix template lower_bound");
  benchmark::time(const_lower_bound_template, "radix template lower_bound const");

  benchmark::time(upper_bound, "radix upper_bound");
  benchmark::time(const_upper_bound, "radix upper_bound const");
  benchmark::time(upper_bound_template, "radix template upper_bound");
  benchmark::time(const_upper_bound_template, "radix template upper_bound const");

  benchmark::time(const_contains, "radix contains const");
  benchmark::time(const_contains_template, "radix template contains const");
}

