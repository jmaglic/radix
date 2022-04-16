#include <iostream>
#include <map>
#include "unittest.cpp"

int main(){

  std::cout << "Insert:\t" << (TestInsert()? "yey" : "ney") << "\n";
  std::cout << "Overwrite:\t" << (TestOverwrite()? "yey" : "ney") << "\n";
  std::cout << "Contains:\t" << (TestContains()? "yey" : "ney") << "\n";
  std::cout << "Dereference:\t" << (TestDeref()? "yey" : "ney") << "\n";
  std::cout << "Assignment:\t" << (TestAssignment()? "yey" : "ney") << "\n";
  std::cout << "Clear:\t" << (TestClear()? "yey" : "ney") << "\n";
  std::cout << "Erase:\t" << (TestErase()? "yey" : "ney") << "\n";
  std::cout << "Iterator:\t" << (TestIterator()? "yey" : "ney") << "\n";

  std::cout << std::flush;


  xsm::radix<bool> rdx;
  bool success = true;
  // Emplace entry
  {
    auto [it,emplaced] = rdx.emplace("hello",true);
    success &= it->first == "hello";
    success &= it->second;
    success &= emplaced;
  }
  // Attempt emplacing an element with an existing key
  {
    auto [it,emplaced] = rdx.emplace("hello",false);
    success &= it->first == "hello";
    success &= it->second;
    success &= !emplaced;
  }
  // New key is prefix of existing key
  {
    auto [it,emplaced] = rdx.emplace("hell",true);
    success &= it->first == "hell";
    success &= it->second;
    success &= emplaced;
  }
  // Keys diverge
  {
    auto [it,emplaced] = rdx.emplace("hero",true);
    success &= it->first == "hero";
    success &= it->second;
    success &= emplaced;
  }
  // Non-leaf node gets converted
  {
    success &= !rdx.contains("he");
    auto [it,emplaced] = rdx.emplace("he",true);
    success &= it->first == "he";
    success &= it->second;
    success &= emplaced;
  }
  // Existing key is a prefix of new key
  {
    auto [it,emplaced] = rdx.emplace("hellow",true);
    success &= it->first == "hellow";
    success &= it->second;
    success &= emplaced;
  }


  /*
  auto it = rdx.begin();
  std::cout << it->first << std::endl;
  std::cout << (it++)->first << std::endl;
  std::cout << it->first << std::endl;
  std::cout << (++it)->first << std::endl;
  std::cout << it->first << std::endl;
*/
  
  /*
  for (auto elem : rdx){
    std::cout << elem.first << std::endl;
  }

  auto it = rdx.begin();
  std::cout << it->first << std::endl;
  std::advance(it,2);
  std::cout << it->first << std::endl;
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

