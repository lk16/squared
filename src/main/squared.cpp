#include <gtkmm.h>

#include "gui/main_window.hpp"
#include "main/testing.hpp"
#include "main/args.hpp"


int main(int argc,char **argv){
  srand(std::time(NULL));
  
  squared_args parsed_args(argc,(const char**)argv);

  
  game_control control;
  
  parsed_args.gc = &control;
  
  if(!parsed_args.parse()){
    return 1;
  }
  
  control.run();

  return 0;
}
