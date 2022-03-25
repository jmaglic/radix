#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include "radix.hpp"

bool TestInsert();
bool TestOverwrite();
bool TestContains();
int main(){

  
  std::cout << "Insert: " << (TestInsert()? "yey" : "ney") << std::endl;
  std::cout << "Overwrite: " << (TestOverwrite()? "yey" : "ney") << std::endl;
  std::cout << "Contains: " << (TestContains()? "yey" : "ney") << std::endl;

  struct Hello{
    Hello() : val(1) {};
    Hello(int i) : val(i) {};
    int val;
  };


  std::map<std::string,Hello> map;

  auto it_b = map.insert(std::make_pair("mick", Hello()));

  std::cout << it_b.first->second.val << std::endl;
  it_b.first->second = Hello(2);
  auto pair = it_b.first;
  pair->second = Hello(3);
  std::cout << it_b.first->second.val << std::endl;
  std::cout << map["mick"].val << std::endl;

  /*
  xsm::radix<std::string> rdx;
  xsm::radix<std::string>::iterator it = rdx.insert(std::make_pair("name", "gordon")).first;

  bool success = true;
  success &= it->first == "name";
  success &= it->second == "gordon";
//  success &= *it->first == "name";
//  success &= *it->second == "gordon";

  std::cout << (success? "yey" : "ney") << std::endl;
*/

  /*
  xsm::radix<std::string> rdx;

  std::vector<std::string> keys = {"job", "age", "name"};
  std::vector<std::string> values = {"lawyer", "31", "jeremy"};

  for (size_t i = 0; i < keys.size(); ++i){
    rdx[keys[i]] = values[i];
  }

  bool all_correct = true;
  for (size_t i = 0; i < keys.size(); ++i){
    all_correct &= rdx.at(keys[i]) == values[i];
  }

  std::cout << (all_correct? "yey" : "ney") << std::endl;
*/

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

// Changing value using at()
bool TestOverwrite(){
  bool success = true;
  xsm::radix<std::string> rdx;
  std::string name1 = "jasmin";
  std::string name2 = "alexandra";
  rdx.insert("name", name1);

  success &= rdx.at("name") == name1;

  rdx.at("name") = name2;

  success &= rdx.at("name") == name2;
  return success;
}

bool TestContains(){
  bool success = true;
  xsm::radix<bool> rdx;
  rdx.insert("hello",true);
  
  success &= rdx.contains("hello");
  success &= !rdx.contains("hell");
  
  rdx.insert("hell",true);

  success &= rdx.contains("hell");

  return success;
}

