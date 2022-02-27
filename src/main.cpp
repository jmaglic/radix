#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include "radix.hpp"

bool TestInsert();
int main(){

  std::cout << "Insert: " << (TestInsert()? "yey" : "ney") << std::endl;

  xsm::radix<std::string> rdx;

  rdx.insert("name", "jasmin");

  std::cout << rdx.at("name") << std::endl;

  // Interactive demonstration
  /*
  xsm::radix<bool> rdx;
  bool exit = false;
  while (!exit) {
    std::string word;
    std::cout << "Enter: ";
    std::cin >> word;

    bool success = rdx.insert(word,true);
    std::cout << "Insert " << (success? "succeeded" : "failed") << "\n";
    rdx.print();
  }
  */
}

// Insertion and tree structure
bool TestInsert(){
  xsm::radix<bool> rdx;
  // Insert words with different methods
  std::vector<std::string> words = {"hello", "hell", "here", "hate", "love", "hell"};
  rdx.insert(std::vector<std::string>{words[0], words[1], words[2]}, true);
  rdx.insert(std::make_pair(words[3], true));
  rdx.insert(words[4], true);
  rdx.insert(words[5], true);
  // Print the tree structure
  //rdx.print();
  // Sort word vector for comparison
  std::sort(words.begin(), words.end());
  // Remove duplicate keys
  words.erase(std::unique(words.begin(), words.end()), words.end());

  size_t i = 0;
  bool all_correct = true;
  for (xsm::radix<bool>::iterator it = rdx.begin(); it != rdx.end(); ++it){
    all_correct &= (it.GetKey() == words[i]);
    ++i;
  }
  return all_correct;
}
