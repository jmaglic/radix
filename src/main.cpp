#include <iostream>
#include <map>
#include <chrono>
#include <vector>
#include "commonwords.hpp"

#include "radix.hpp"

int main(){

  auto [common_words, import_success] = importCommonWords();

  for (auto const& w : common_words){
    std::cout << w << std::endl;
  }

/*
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  xsm::radix<bool> rdx;

  std::ifstream infile("../build/eng10k");

  std::string line;
  if (infile.is_open()){
    while (std::getline(infile, line)) {
      rdx.emplace(line,true);
    }
  }
  else {
    std::cout << "Could not open" << std::endl;
  }
  infile.close();

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
  


  std::chrono::steady_clock::time_point begin_hint = std::chrono::steady_clock::now();
  
  xsm::radix<bool> rdx_hint;
  //std::map<std::string,bool> rdx_hint;

  rdx_hint.emplace("a", true);

  for (auto const& x : rdx){
    std::cout << x.first << std::endl;
    rdx_hint.emplace_hint(rdx_hint.end(), x.first, true);
  }

  std::chrono::steady_clock::time_point end_hint = std::chrono::steady_clock::now();

  std::cout << "Time difference = " 
    << std::chrono::duration_cast<std::chrono::microseconds>(end_hint - begin_hint).count() << "[µs]" << std::endl;
*/
  /*
  struct IsEven {};
  struct IsOdd {};

  struct Comp{
    using is_transparent = void;
    bool operator()(int lhs, int rhs) const {
      return lhs < rhs;
    }
    bool operator()(int lhs, IsEven rhs) const {
      return lhs%2;
    }
    bool operator()(IsEven lhs, int rhs) const {
      return rhs%2;
    }
    bool operator()(int lhs, IsOdd rhs) const {
      return (lhs+1)%2;
    }
    bool operator()(IsOdd lhs, int rhs) const {
      return (rhs+1)%2;
    }
  };

  std::map<int,bool,Comp> map;
  map.emplace(1,true);
  map.emplace(2,true);
  map.emplace(3,true);
  map.emplace(4,true);

  //std::cout << map.lower_bound(IsEven())->first << std::endl;
  //std::cout << map.upper_bound(IsEven())->first << std::endl;

  */




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

}

