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
    
    if(!b.has_valid_moves()){
      b.switch_turn();
      turn = (turn==b_player ? w_player : b_player);
      if(!b.has_valid_moves()){
        return b.get_disc_diff();
      }
    }
    
    board tmp;
    turn->do_move(&b,&tmp);
    b = tmp;
    
    turn = (turn==b_player ? w_player : b_player);
  }
}

void show_help(){
  std::cout << 
  "Invalid argument syntax, try one of these:  \n"
  "o---------------------o--------------------------------------------------------o\n"
  "| testing             | run the testing_area()                                 |\n"
  "| timing              | run the timing_area()                                  |\n"
  "| show                | show start position                                    |\n"
  "| show <board string> | show board                                             |\n"
  "| play                | play the windowed game                                 |\n"
  "| play <board string> | play the windowed game with given starting position    |\n"
  "| random x y z        | play the windowed game, x random moves, y,z bot levels |\n"
  "| <no args>           | play the windowed game                                 |\n"
  "o---------------------o--------------------------------------------------------o\n";
}


int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  srand(std::time(NULL));
  
  std::string str[10];
  for(int i=0;i < min<int>(10,argc);i++){
    str[i]  = argv[i];
  }
  
  
  if(argc>=2){
    if(str[1] == "testing"){
      testing_area();
      return 0;
    }
    if(str[1] == "timing"){
      timing_area();
      return 0;
    }
    if(str[1] == "show"){
      if(argc>=3){
        board(str[2]).show();
      }
      else{
        board b;
        b.reset();
        b.show();
      }
      return 0;
    }
    if(str[1] == "play"){
      if(argc>=3){
        main_window window(str[2]);
        Gtk::Main::run(*&window);
      }
      else{
        main_window window;
        Gtk::Main::run(window);
      }
      return 0;
    }
    if(str[1] == "random"){
      if(argc>=4){
        board b;
        b.reset();
        b = b.do_random_moves(fromstr<int>(str[2]));
        main_window window(b);
        int depth,perfect;
        depth = fromstr<int>(str[3]);
        perfect = max(16,2*depth);
        if(depth!=0){
          window.control.add_bot(BLACK,depth,perfect);
        }
        depth = fromstr<int>(str[4]);
        perfect = max(16,2*depth);
        if(depth!=0){
          window.control.add_bot(WHITE,depth,perfect);
        }
        Gtk::Main::run(window);
        return 0;
      }
    }
    show_help();
    
  }
  else{
    main_window window;
    Gtk::Main::run(window);
  }
  return 0;
}
