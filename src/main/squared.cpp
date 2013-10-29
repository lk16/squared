#include <gtkmm.h>

#include "gui/main_window.hpp"

void testing_area(){

  board b;
  
  
  srand(time(NULL));
  
  b.reset();
  b.check_do_move_experimental();
  
  
  

  
}





void timing_area(){
  

  timeval start,end;
  
  std::bitset<64> dummy;
  
  const int max = 1000000;
  gettimeofday(&start,NULL);
  
  for(int i=0;i<max;i++){
    board b;
    b.do_move(19,&dummy);
  }
  
  
  gettimeofday(&end,NULL);
  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0));
  
  std::cout << "Working:\t" << max << " / " << time_diff;
  std::cout << " = " << big_number(max/time_diff) << " per sec avg\n"; 
  
  gettimeofday(&start,NULL);
  
  for(int i=0;i<max;i++){
    board b;
    b.do_move_experimental(19,&dummy);
  }
  
  
  gettimeofday(&end,NULL);
  
  time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0));
  
  std::cout << "Experimental:\t" << max << " / " << time_diff;
  std::cout << " = " << big_number(max/time_diff) << " per sec avg\n"; 
  
}





int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  
  
  if(argc>=2){
    std::string argv1(argv[1]);
    if(argv1=="testing"){
      testing_area();
    }
    else if(argv1=="timing"){
      timing_area();
    }
    else{
      std::cout << "Invalid argument: " << argv1 << '\n' << 
      "Try one of these:\n"
      "testing   | run the testing_area()\n"
      "timing    | run the timing_area()\n"
      "<no args> | run the windowed game\n";
      
    }
  }
  else{
    main_window window;
    Gtk::Main::run(window);
  }
  return 0;
}
