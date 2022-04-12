#include <vector>
#include <array>
#include <algorithm>
#include "radix.hpp"

bool TestInsert();
bool TestOverwrite();
bool TestContains();
bool TestDeref();
bool TestAssignment();
bool TestClear();

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
    all_correct &= (it->first == words[i]);
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

// Use of contains()
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

// Use of -> and * operators
bool TestDeref(){
  xsm::radix<std::string> rdx;
  auto it = rdx.insert(std::make_pair("name", "gordon")).first;

  bool success = true;
  // operator->
  success &= it->first == "name";
  success &= it->second == "gordon";
  // operator*
  success &= (*it).first == "name";
  success &= (*it).second == "gordon";
  // mutability
  it->second = "william";
  success &= rdx.at("name") == "william";
  // mutability
  (*it).second = "elija";
  success &= rdx.at("name") == "elija";

  return success;
}

// Copy/Move constructors/assignment operators
bool TestAssignment(){
  // Init rdx
  xsm::radix<int> rdx;

  std::vector<std::string> keys = {"aaaa", "aabb", "aa", "abc", "abd"};
  std::vector<int> values = {1,1,1,1,1};

  for (size_t i = 0; i < keys.size(); ++i){
    rdx[keys[i]] = values[i];
  }
  
  std::sort(keys.begin(), keys.end());
  keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

  // Copy constructor
  xsm::radix<int> c_rdx = rdx;

  bool success = true;
  auto c_it = c_rdx.begin();
  for (auto it = rdx.begin(); it != rdx.end(); ++it){
    success &= it->first == c_it->first;
    success &= it->second == c_it->second;
    success &= it != c_it;
    ++c_it;
  }

  // Move assignment
  auto comp_it = rdx.begin();

  c_rdx = std::move(rdx);

  success &= comp_it == c_rdx.begin();

  // rdx should be empty
  success &= rdx.begin() == rdx.end();

  size_t i = 0;
  for (c_it = c_rdx.begin(); c_it != c_rdx.end(); ++c_it){
    success &= c_it->first == keys[i];
    ++i;
  }
 
  // Copy assignment
  comp_it = c_rdx.begin();

  rdx = c_rdx;
  
  success &= comp_it != rdx.begin();

  c_it = c_rdx.begin();
  for (auto it = rdx.begin(); it != rdx.end(); ++it){
    success &= it->first == c_it->first;
    success &= it->second == c_it->second;
    success &= it != c_it;
    ++c_it;
  }

  // Move constructor
  xsm::radix<int> n_rdx = std::move(rdx);
  
  success &= rdx.begin() == rdx.end();

  i = 0;
  for (auto n_it = n_rdx.begin(); n_it != n_rdx.end(); ++n_it){
    success &= n_it->first == keys[i];
    ++i;
  }
  return success;
}

// Use of clear() and empty()
bool TestClear(){
  xsm::radix<bool> rdx;
  bool success = true;
  success &= rdx.empty();
  
  rdx.insert("something",true);
  success &= !rdx.empty();

  rdx.clear();
  success &= rdx.empty();
  return success;
}

// Use of erase()
bool TestErase(){
  xsm::radix<bool> rdx;

  rdx.insert("well",true);
  rdx.insert("water",true);
  rdx.insert("waste",true);
  rdx.insert("wa",true);

  // Validate insertion
  bool success = true;
  success &= rdx.contains("well");
  success &= rdx.contains("water");
  success &= rdx.contains("waste");
  success &= rdx.contains("wa");
  success &= rdx.size() == 4;

  // Basic erase
  rdx.erase(rdx.find("wa"));
  success &= !rdx.contains("wa");
  success &= rdx.size() == 3;

  rdx.erase(rdx.find("well"));
  success &= !rdx.contains("well");
  success &= rdx.size() == 2;
  
  rdx.erase(rdx.find("water"));
  success &= !rdx.contains("water");
  success &= rdx.size() == 1;
  
  rdx.erase(rdx.find("waste"));
  success &= !rdx.contains("waste");
  success &= rdx.size() == 0;

  // Correct return iterator
  rdx.insert("hello", true);
  rdx.insert("hell", true);
  rdx.insert("he", true);
  success &= rdx.size() == 3;

  xsm::radix<bool>::iterator it = rdx.erase(rdx.find("he"));
  success &= it->first == "hell";
  it = rdx.erase(it);
  success &= it->first == "hello";

  success &= rdx.contains("hello");
  success &= !rdx.contains("hell");
  success &= !rdx.contains("he");
  success &= rdx.size() == 1;

  return success;
}

bool TestIterator(){
  xsm::radix<bool> rdx;
  std::vector<std::string> keys = {"a", "aa", "ab", "abb", "abc", "abd", "abde", "abdf"};
  bool success = true;

  // Insert
  size_t i = 0;
  for (i = 0; i < keys.size(); ++i){
    rdx.insert(keys[i], true);
  }

  // begin and end
  i = 0;
  for (auto it = rdx.begin(); it != rdx.end(); ++it){
    success &= it->first == keys[i];
    ++i;
  }

  // rbegin and rend
  i = keys.size()-1;
  for (auto it = rdx.rbegin(); it != rdx.rend(); ++it){
    success &= it->first == keys[i];
    --i;
  }

  const xsm::radix<bool> c_rdx = rdx;

  // begin and end (const radix)
  i = 0;
  for (auto it = c_rdx.begin(); it != c_rdx.end(); ++it){
    success &= it->first == keys[i];
    ++i;
  }

  // rbegin and rend (const radix)
  i = keys.size()-1;
  for (auto it = c_rdx.rbegin(); it != c_rdx.rend(); ++it){
    success &= it->first == keys[i];
    --i;
  }

  // cbegin and cend
  i = 0;
  for (auto it = rdx.cbegin(); it != rdx.cend(); ++it){
    success &= it->first == keys[i];
    ++i;
  }

  // crbegin and crend
  i = keys.size()-1;
  for (auto it = rdx.crbegin(); it != rdx.crend(); ++it){
    success &= it->first == keys[i];
    --i;
  }
  return success;
}

