#include <iostream>
#include <map>
#include "radix.hpp"

int main(){
  // Insertion and tree structure
  xsm::radix<bool> rdx;
  rdx.insert("hello", true);
  rdx.insert(std::make_pair("hell", true));
  rdx.insert("here", true);
  rdx.insert("hate", true);
  rdx.insert("love", true);

  rdx.print();

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

