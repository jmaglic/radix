#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include "radix.hpp"

int main(){

  // Insertion and tree structure
  xsm::radix<bool> rdx;
  std::vector<std::string> words = {"hello", "hell", "here", "hate", "love"};
  rdx.insert(std::vector<std::string>{words[1], words[2], words[3]}, true);
  rdx.insert(std::make_pair(words[4], true));
  rdx.insert(words[5], true);

  rdx.print();

  sort(words.begin(), words.end());
  for (auto word : words){
    std::cout << word << std::endl;
  }

  for (xsm::radix<bool>::iterator it = rdx.begin(); it != rdx.end(); ++it){
    std::cout << it.GetKey() << std::endl;
  }

  std::map<std::string,int> map;

  std::pair<std::map<std::string,int>::iterator,bool> res;

  res = map.insert(std::make_pair("hello",3));
  std::cout << (res.second? "yes" : "no") << std::endl;
  std::cout << res.first->second << std::endl;
  res = map.insert(std::make_pair("hello",4));
  std::cout << (res.second? "yes" : "no") << std::endl;
  std::cout << res.first->second << std::endl;

  for (auto it : map){
    std::cout << it.second << std::endl;
  }
}

