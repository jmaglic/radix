#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace commonwords{
  std::pair<std::vector<std::string>,bool> readWords(){
  
    std::vector<std::string> common_words;
  
    std::ifstream infile("../res/english-most-common-10000-words");
    bool open_success = infile.is_open();
  
    std::string line;
    while (std::getline(infile, line)) {
      common_words.push_back(line);
    }
  
    return std::make_pair(common_words, open_success);
  }
  
  void failedImport(){
    std::string msg = std::string("Common Words File could not be read. Make sure you are") +
      std::string(" running the linked executable from a build directory.");
    std::cout << msg << std::endl;
  }
}
