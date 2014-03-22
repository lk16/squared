#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "util/string.hpp"

// class to easily manipulate csv files
// this simple implementation does not handle escaped characters, quotes or newlines
class csv{
  
  std::fstream file;
  std::string filename;
  
public:
  csv(const std::string& filename);
  
  
  
  void append_line(const std::vector<std::string>& x);
  
  std::vector<std::vector<std::string> > get_content();

  void set_content(std::vector<std::vector<std::string> >& x);



};
