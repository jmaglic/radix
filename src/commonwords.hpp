#include <fstream>
#include <string>
#include <vector>

std::pair<std::vector<std::string>,bool> importCommonWords(){

  std::vector<std::string> common_words;

  std::ifstream infile("../res/english-most-common-10000-words");
  bool open_success = infile.is_open();

  std::string line;
  while (std::getline(infile, line)) {
    common_words.push_back(line);
  }

  return std::make_pair(common_words, open_success);
}

