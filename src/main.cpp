#include <iostream>
#include <map>
#include "unittest.cpp"

int main(){

  std::cout << "Insert: " << (TestInsert()? "yey" : "ney") << std::endl;
  std::cout << "Overwrite: " << (TestOverwrite()? "yey" : "ney") << std::endl;
  std::cout << "Contains: " << (TestContains()? "yey" : "ney") << std::endl;
  std::cout << "Dereference: " << (TestDeref()? "yey" : "ney") << std::endl;
  std::cout << "Assignment: " << (TestAssignment()? "yey" : "ney") << std::endl;
  std::cout << "Clear: " << (TestClear()? "yey" : "ney") << std::endl;


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

