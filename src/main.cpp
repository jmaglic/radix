#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include "radix.hpp"

bool TestInsert();
int main(){

  std::cout << "Insert: " << (TestInsert()? "yey" : "ney") << std::endl;

}

// Insertion and tree structure
bool TestInsert(){
  xsm::radix<bool> rdx;
  // Insert words with different methods
  std::vector<std::string> words = {"hello", "hell", "here", "hate", "love"};
  rdx.insert(std::vector<std::string>{words[0], words[1], words[2]}, true);
  rdx.insert(std::make_pair(words[3], true));
  rdx.insert(words[4], true);
  // Print the tree structure
  //rdx.print();
  // Sort word vector for comparison
  sort(words.begin(), words.end());

  size_t i = 0;
  bool all_correct = true;
  for (xsm::radix<bool>::iterator it = rdx.begin(); it != rdx.end(); ++it){
    all_correct &= (it.GetKey() == words[i]);
    ++i;
  }
  return all_correct;
}
