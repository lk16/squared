#include <gtkmm.h>

#include <vector>
#include <set>

#include "gui/main_window.hpp"


void testing_area(){

  for(int i=0;i<8;i++){
    for(int j=0;j<6;j++){
      show_bitset(board::capture[i][j] | std::bitset<64>(1ul << board::capture_start[i][j]));
    }
  }
  return;
  
  board b,copy;
  std::bitset<64> undo,undo_exp,moves;
  int move;
  
  int wrong = 0;
  
  srand(time(NULL));
  
  for(int i=0;i<100;i++){
    
    b.randomize();
    assert((b.me & b.opp).none());
    
    b.get_valid_moves(&moves);
    
    copy = b;
    
    while(true){
      move = find_first_set_64(moves.to_ulong());
      if(move == -1){
        break;
      }
      
      b.do_move(move,&undo);
      b = copy;
      b.do_move_experimental(move,&undo_exp);
      b = copy;
      
      
      if(undo != undo_exp){
        std::cout << "test failed:\n";
        wrong++;
      
    
        b.show();
        std::cout << "move:\n";
        show_bitset(std::bitset<64>(1ul << move));
        
        std::cout << "working:\n";
        show_bitset(undo);
        std::cout << "experimental:\n";
        show_bitset(undo_exp);
        
        return;
      }
      
      
      moves.reset(move);
    }
    
    if(wrong==0){
      std::cout << "yay!\n";
    }
    else{
      std::cout << "boo!\n";
    }
    
  }
  
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
