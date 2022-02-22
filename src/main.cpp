#include <iostream>
#include "radix.hpp"

int main(){
  // Insertion and tree structure
  xsm::radix<bool> rdx;
  rdx.insert("hello", true);
  rdx.insert("hell", true);
  rdx.insert("here", true);
  rdx.insert("hate", true);
  rdx.insert("love", true);

  rdx.print();

  for (xsm::radix<bool>::iterator it = rdx.begin(); it != rdx.end(); ++it){
    std::cout << it.GetKey() << std::endl;
  }
}

