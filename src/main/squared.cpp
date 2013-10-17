#include <gtkmm.h>

#include <vector>
#include <set>

#include "gui/main_window.hpp"


void testing_area(){
  
  board b;
  std::bitset<64> moves;
  
  b.discs[WHITE].reset();
  b.discs[BLACK].reset();
  b.turn = BLACK;
  
  b.discs[BLACK].set(0);
  b.discs[WHITE].set(9);
  b.discs[WHITE].set(18);
  b.discs[WHITE].set(27);
  b.discs[WHITE].set(36);
  b.discs[WHITE].set(45);
  
  b.show();
  
  b.get_possible_moves_experimental(&moves);
  
  show_bitset(moves);  
  
}

void timing_area(){
  
  board b;
  b.reset();

  timeval start,end;
  
  const int max = 1000000000;
  gettimeofday(&start,NULL);
  for(int i=0;i<max;i++){
    // do something
  }
  
  gettimeofday(&end,NULL);
  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0));
  
  std::cout << max << " / " << time_diff << " = " << (max/time_diff) << '\n'; 
  
}





int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  main_window window;
  
  // has to be done
  board::init_constants();
  
  
  if(argc>=2){
    std::string argv1(argv[1]);
    if(argv1=="testing"){
      testing_area();
    }
    else if(argv1=="timing"){
      timing_area();
    }
    else{
      std::cout << "Invalid parameter: " << argv1 << '\n';
    }
  }
  else{
    Gtk::Main::run(window);
  }
  return 0;
}
