#include "main/args.hpp"

int main(int argc,char **argv){
  srand(std::time(NULL));
  
  squared_args parsed_args(argc,(const char**)argv);
  game_control control;
  
  parsed_args.gc = &control;
  if(parsed_args.parse()){
    control.run();
    return 0;
  }
  return 1;
}
