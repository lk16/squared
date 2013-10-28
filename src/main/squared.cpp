#include <gtkmm.h>

#include "gui/main_window.hpp"

void testing_area(){

  board b;
  
  
  srand(time(NULL));
  
  b.reset();
  b.check_do_move_experimental();
  
  
  

  
}





void timing_area(){
  
  board b;
  b.reset();

  timeval start,end;
  
  std::bitset<64> dummy;
  
  const int max = 1000000;
  gettimeofday(&start,NULL);
  
  for(int i=0;i<max;i++){
    b.do_move(19,&dummy);
    b.reset();
  }
  
  show_bitset(dummy);
  
  gettimeofday(&end,NULL);
  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0));
  
  std::cout << max << " / " << time_diff << " = " << (max/time_diff) << '\n'; 
  
}





int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  main_window window;
  
  
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
