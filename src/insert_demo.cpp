#include <iostream>
#include "radix.hpp"

// Interactive demo
int main() {

  std::cout << "This demo creates a radix tree, allows inserting words,\n";
  std::cout << "and visualises the resulting tree. You may add one or\n";
  std::cout << "more words at a time\n";
  std::cout << "\nTo exit this demo, type END\n" << std::endl;

  xsm::radix<bool> rdx;
  bool exit = false;
  while (!exit) {
    std::string word;
    std::cout << "Enter word to insert in radix tree: ";
    std::cin >> word;

    if (word == "END"){
      exit = true;
    }
    else {
      rdx.emplace(word,true);
      rdx.print();
    }
  }
}

