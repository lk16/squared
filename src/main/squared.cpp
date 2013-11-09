#include <gtkmm.h>

#include "gui/main_window.hpp"
#include "misc/testing.hpp"
#include "misc/timing.hpp"




// returns positive values if white won
int play_game(bot_base* b_player,bot_base* w_player){
 
  if(b_player->get_color()!=-1 || w_player->get_color()!=1){
    std::cout << "Invalid color configuration in play_game()\n";
    return 0;
  }
  
  board b;
  b.reset();
  
  bot_base* turn = b_player;
  
  while(true){
    
    if(!b.has_children()){
      b.switch_turn();
      turn = (turn==b_player ? w_player : b_player);
      if(!b.has_children()){
        return b.get_disc_diff();
      }
    }
    
    board tmp;
    turn->do_move(&b,&tmp);
    b = tmp;
    
    turn = (turn==b_player ? w_player : b_player);
  }
}


int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  
  
  if(argc>=2){
    std::string argv1(argv[1]);
    if(argv1 == "testing"){
      testing_area();
    }
    else if(argv1 == "timing"){
      timing_area();
    }
    else{
      std::cout << "Invalid argument: " << argv1 << '\n' << 
      "Try one of these:\n"
      "----------.-----------------------\n"
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
