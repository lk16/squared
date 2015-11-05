#pragma once

#include <iostream>
#include <iomanip>

class progress_bar{
  static const int bar_size = 100;
  int bar_printed_chars = 0;
  int total_steps;
  int steps_done;
  
  
public:
  progress_bar(int n,const std::string& func_name){ 
    std::cout << "\n" << std::setw(40) << func_name << "  ";
    std::cout.flush();
    total_steps = n;
    steps_done = 0;
    bar_printed_chars = 0;
  }
  
  void step(){
    ++steps_done;
    const int needed = (int)((((float)steps_done)/total_steps)*bar_size);
    while(needed != bar_printed_chars){
      std::cout << '.';
      ++bar_printed_chars;
    }
    std::cout.flush();
  }
  
  ~progress_bar(){
    while(bar_printed_chars < bar_size){
      std::cout << '.';
      ++bar_printed_chars;
    }
    std::cout.flush();
  }
  
};