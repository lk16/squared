#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "util/string.hpp"
// class to easily manipulate csv files
// this simple implementation does not handle escaped characters, 
// quotes or newlines within quotes
class csv{
  
  std::string filename;
  const int line_buff_size;
  char* line_buff;
  std::fstream file;
  int read_lines,written_lines;
  
  
  
public:
  
  
  typedef std::vector<std::string> line_t;
  typedef std::vector<line_t> content_t;
  
  
  csv(const std::string& filename);
  ~csv();
  
  
  
  void append_line(const line_t& x);
  line_t get_line();
  
  
  content_t get_content();
  void set_content(const content_t & x);
  
  // returns file
  std::fstream* get_file();

  int get_read_lines() const;
  int get_written_lines() const;

  std::string get_name() const;
};
