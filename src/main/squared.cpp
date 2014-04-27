#include <gtkmm.h>

#include "gui/main_window.hpp"
#include "util/testing.hpp"
#include "main/learn.hpp"

void show_help(){
  std::cout << 
  "-h, --help\n"
  "show this help\n\n"
  "Invalid argument syntax, try one of these:  \n\n"
  "--testing:\n"
  "run the testing area, ignoring all other flags\n\n"
  "-b <board>, --board <board>\n"
  "use given board as start position, cannot be used with -r\n\n"
  "-r <integer>, --random <integer>\n"
  "use startboard after given amount of random moves, cannot be used with -b\n\n"
  "-lb <integer>, -lw <integer>\n"
  "set the level of the black or white bot, -1 is disabled\n\n"
  "-s\n"
  "show given board (or start position if none given) in human readable format\n\n"
  "--learn\n"
  "learn/improve the opening book, ignoring all other flags\n\n";
}


int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  srand(std::time(NULL));
  
  std::vector<std::string> str;
  for(int i=0;i<argc;i++){
    str.push_back(std::string(argv[i]));
  }
  
  game_control gc;
  
  bool h_flag = false;
  bool syntax_error_flag = false;
  bool board_is_initialized = false;
  
  int i = 1;
  
  while(i<argc){
    if(str[i] == "-h" || str[i] == "--help"){
      show_help();
      return 0;
    }
    else if(str[i] == "--testing"){
      testing_area();
      return 0;
    }
    else if(str[i] == "-s"){
      h_flag = true;
      i++;
    }
    else if(str[i] == "--learn"){
      book_t book(BOOK_PATH + "book.csv");
      book.add_from_moves_file(BOOK_PATH + "moves.csv");
      bot_ali bot(-1,-1); 
      book.learn(&bot);
      return 0;
    }
    else if(str[i] == "-b" || str[i] == "--board"){
      if(i+1 >= argc || board_is_initialized){
        syntax_error_flag = true;
        break;
      }
      gc.current = board(str[i+1]);
      board_is_initialized = true;
      i+=2;
    }
    else if(str[i] == "-r" || str[i] == "--random"){
      if(i+1 >= argc || board_is_initialized){
        syntax_error_flag = true;
        break;
      }
      gc.current = gc.current.do_random_moves(fromstr<int>(str[i+1]));
      board_is_initialized = true;
      i+=2;
    }
    else if(str[i] == "-lb"){
      if(i+1 >= argc){
        syntax_error_flag = true;
        break;
      }
      int d = fromstr<int>(str[i+1]);
      int pd = max(16,2*d);
      gc.add_bot(BLACK,d,pd);
      i+=2;
    }
    else if(str[i] == "-lw"){
      if(i+1 >= argc){
        syntax_error_flag = true;
        break;
      }
      int d = fromstr<int>(str[i+1]);
      int pd = max(16,2*d);
      gc.add_bot(WHITE,d,pd);
      i+=2;
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
    gc.current.show();
    return 0;
  }
  main_window window(gc);
  Gtk::Main::run(window);
  return 0;
}
