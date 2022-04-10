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

  std::cout << std::flush;

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

