#include <gtkmm.h>

#include <vector>
#include <set>

#include "gui/main_window.hpp"


void testing_area(){
  
  show_bitset(std::bitset<64>(0x0000FCFCFCFCFCFC)); // +7
  show_bitset(std::bitset<64>(0x00003F3F3F3F3F3F)); // +9
  show_bitset(std::bitset<64>(0xFCFCFCFCFCFC0000)); // -9
  show_bitset(std::bitset<64>(0x3F3F3F3F3F3F0000)); // -7
  show_bitset(std::bitset<64>(0xFCFCFCFCFCFCFCFC)); // -1
  show_bitset(std::bitset<64>(0x3F3F3F3F3F3F3F3F)); // +1
  show_bitset(std::bitset<64>(0x0000FFFFFFFFFFFF)); // +8
  show_bitset(std::bitset<64>(0xFFFFFFFFFFFF0000)); // -8
  
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
