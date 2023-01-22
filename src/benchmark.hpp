#ifndef benchmark_hpp
#define benchmark_hpp

#include <iostream>
#include <chrono>
#include <functional>
#include <string>

namespace benchmark{
  void time(std::function<void()> func, std::string bm_name){
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    func();
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Benchmark " << bm_name << ": "
      << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() 
      << " us" << std::endl;
  }
}

#endif
