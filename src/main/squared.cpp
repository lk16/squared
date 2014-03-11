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
  "-h, --help\n"
  "show this help\n\n"
  "Invalid argument syntax, try one of these:  \n\n"
  "--testing:\n"
  "run the testing area, ignoring all other flags\n\n"
  "--timing:\n"
  "run the timing area, ignoring all other flags\n\n"
  "-b <board>, --board <board>\n"
  "use given board as start position, cannot be used with -r\n\n"
  "-r <integer>, --random <integer>\n"
  "use startboard after given amount of random moves, cannot be used with -b\n\n"
  "-lb <integer>, -lw <integer>\n"
  "set the level of the black or white bot, -1 is disabled\n\n"
  "-s\n"
  "show given board (or start position if none given) in human readable format\n\n";
}


int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  srand(std::time(NULL));
  
  std::vector<std::string> str;
  for(int i=0;i<argc;i++){
    str.push_back(std::string(argv[i]));
  }
  
  int i = 1;
  board b;
  bool h_flag = false;
  bool syntax_error_flag = false;
  bool board_is_initialized = false;
  b.reset();
  
  
  while(i<argc){
    if(str[i] == "-h" || str[i] == "--help"){
      show_help();
      return 0;
    }
    else if(str[i] == "--testing"){
      testing_area();
      return 0;
    }
    else if(str[i] == "--timing"){
      timing_area();
      return 0;
    }
    else if(str[i] == "-s"){
      h_flag = true;
      i++;
    }
    else if(str[i] == "-b" || str[i] == "--board"){
      if(i+1 >= argc || board_is_initialized){
        syntax_error_flag = true;
        break;
      }
      b = board(str[i+1]);
      board_is_initialized = true;
      i+=2;
    }
    else if(str[i] == "-r" || str[i] == "--random"){
      if(i+1 >= argc || board_is_initialized){
        syntax_error_flag = true;
        break;
      }
      // b.reset() is done earlier
      b = b.do_random_moves(fromstr<int>(str[i+1]));
      board_is_initialized = true;
      i+=2;
    }
    else if(str[i] == "-lb" || str[i] == "lw"){
      syntax_error_flag = true;
      break;
      // TODO this is not implemented yet
    }
    else{
      syntax_error_flag = true;
      break;
    }
  }
  if(syntax_error_flag){
    std::cout << "Invalid argument syntax. Type " << argv[0] << " -h for help\n";
    return 0;
  }
  if(h_flag){
    b.show();
    return 0;
  }
  main_window window(b);
  Gtk::Main::run(window);
  return 0;
}
