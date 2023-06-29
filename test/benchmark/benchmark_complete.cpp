#include <vector>
#include "radix.hpp"
#include "commonwords.hpp"
#include "benchmark.hpp"

int main() {

  // Import 10000 words
  auto [common_words, import_success] = commonwords::readWords();
  if (!import_success){
    commonwords::failedImport();
  }

  xsm::radix<int> rdx;

  size_t n_words = common_words.size();

  std::vector<std::string>::const_iterator first = common_words.begin();
  std::vector<std::string>::const_iterator middle = common_words.begin() + n_words/2;
  std::vector<std::string>::const_iterator last = common_words.end();

  std::vector<std::string> firsthalf(first, middle);
  std::vector<std::string> secondhalf(first, middle);

  for (auto word : firsthalf) {
    rdx.emplace(word, 1);
  }

  // Benchmark for complete
  std::function<void()> complete_many = [secondhalf, rdx]() mutable {
    for (auto word : secondhalf) {
      rdx.complete(word);
    }
  };

  benchmark::time(complete_many, "Complete");

  // Benchmark for const complete
  const xsm::radix<int> c_rdx = rdx;

  std::function<void()> complete_many_const = [secondhalf, c_rdx]() mutable {
    for (auto word : secondhalf) {
      c_rdx.complete(word);
    }
  };

  benchmark::time(complete_many_const, "Complete const");
}

