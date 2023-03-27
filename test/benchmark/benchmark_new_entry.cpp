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

  // emplace
  {
    xsm::radix<bool> rdx;

    std::function<void()> emplace_many = [common_words, rdx]() mutable {
      for (const std::string word : common_words){
        rdx.emplace(word, true);
      }
    };

    benchmark::time(emplace_many, "radix emplace");
  }

  // insert
  {
    xsm::radix<bool> rdx;

    std::function<void()> insert_many = [common_words, rdx]() mutable {
      for (const std::string word : common_words){
        rdx.insert(std::make_pair(word, true));
      }
    };

    benchmark::time(insert_many, "radix insert");
  }

  std::sort(common_words.begin(), common_words.end());

  // Best hint
  {
    xsm::radix<bool> rdx;

    std::function<void()> emplace_many_best_hint = [common_words, rdx]() mutable {
      rdx.emplace(common_words[0], true);
      for (const std::string word : common_words){
        rdx.emplace_hint(rdx.end(), word, true);
      }
    };

    benchmark::time(emplace_many_best_hint, "radix emplace_hint best hint");
  }

  // Poor hint
  {
    xsm::radix<bool> rdx;

    std::function<void()> emplace_many_poor_hint = [common_words, rdx]() mutable {
      rdx.emplace(common_words[0], true);
      for (const std::string word : common_words){
        rdx.emplace_hint(rdx.begin(), word, true);
      }
    };

    benchmark::time(emplace_many_poor_hint, "radix emplace_hint poor hint");
  }

}

