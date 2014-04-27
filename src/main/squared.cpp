#include <gtkmm.h>

#include <map>

#include "gui/main_window.hpp"
#include "util/testing.hpp"
#include "main/learn.hpp"


class arg_parser{
  
  static const int IGNORE_OTHER_ARGS = -1;
  static const int ERROR = -2;
  
  const char **current_arg,**end_arg;
  
  // check whether there are enough remaining arguments
  // --set-value value -> use n=1
  bool has_enough_args(int n){
    if(current_arg + n >= end_arg){
      error_flag = true;
      return false;
    }  
    return true;
  }
  
  const char* get_arg(int n){
    return *(current_arg + n);
  }
  
  typedef std::map<std::string,int(arg_parser::*)()> func_map_t;
  
  // maps arguments to a function that modifies the argument state
  // that function should return how many arguments it has used
  // or IGNORE_OTHER_ARGS or ERROR when appropriate
  func_map_t func_map;
  
  void fill_func_map();
  
public:
  game_control gc;
  bool show_flag;
  bool error_flag;
  bool start_windowed_game;
  
  arg_parser(int _argc,const char** _argv):
    current_arg(_argv+1),
    end_arg(_argv+_argc)
  {
    show_flag = false;
    error_flag = false;
    start_windowed_game = true;
  }
  
  //returns false on errors
  bool parse();
  
  int show_help();
  int show_board();
  int testing_area_mask();
  int learn();
  int set_board();
  int randomize_board();
  int set_white_level();
  int set_black_level();
  
};


int arg_parser::show_help(){
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
  "show given board in human readable format\n\n"
  "-l, --learn\n"
  "learn/improve the opening book, ignoring all other flags\n\n";
  start_windowed_game = false;
  return -1;
}

int arg_parser::show_board(){
  start_windowed_game = false;
  show_flag = true;
  return 1;
}

int arg_parser::testing_area_mask(){
  start_windowed_game = false;
  testing_area();
  return IGNORE_OTHER_ARGS;
}

int arg_parser::learn(){
  book_t book(BOOK_PATH + "book.csv");
  bot_ali bot(-1,-1); 
  book.learn(&bot);
  return IGNORE_OTHER_ARGS;
}

int arg_parser::set_board(){
  if(!has_enough_args(1)){
    return ERROR;
  }
  gc.current = board(get_arg(1));
  return 2;
}



void arg_parser::fill_func_map()
{
  func_map["--help"] = &arg_parser::show_help;
  func_map["-h"] = &arg_parser::show_help;
  
  func_map["--testing"] = &arg_parser::testing_area_mask;
  
  func_map["-s"] = &arg_parser::show_board;
  
  func_map["--learn"] = &arg_parser::learn;
  func_map["-l"] = &arg_parser::learn;
  
  func_map["--randomize"] = &arg_parser::randomize_board;
  func_map["-r"] = &arg_parser::randomize_board;
  
  func_map["-lb"] = &arg_parser::set_black_level;
  func_map["-lw"] = &arg_parser::set_white_level;
  
  func_map["--board"] = &arg_parser::set_board;
  func_map["-b"] = &arg_parser::set_board;
}


int arg_parser::set_black_level()
{
  if(!has_enough_args(1)){
    return ERROR;
  }
  int lvl = fromstr<int>(get_arg(1));
  gc.add_bot(BLACK,lvl,max(16,2*lvl));
  return 2;
}

int arg_parser::set_white_level()
{
  if(!has_enough_args(1)){
    return ERROR;
  }
  int lvl = fromstr<int>(get_arg(1));
  gc.add_bot(BLACK,lvl,max(16,2*lvl));
  return 2;
}

int arg_parser::randomize_board()
{
  if(!has_enough_args(1)){
    return ERROR;    
  }
  gc.current = gc.current.do_random_moves(fromstr<int>(get_arg(1)));
  return 2;  
}

bool arg_parser::parse()
{
  fill_func_map();
  
  func_map_t::const_iterator it;
  while(!error_flag && current_arg<end_arg){
    it = func_map.find(std::string(*current_arg));
    if(it != func_map.end()){
      int diff = (this->*(it->second))();
      if(diff == ERROR){
        error_flag = true;
      }
      else if(diff == IGNORE_OTHER_ARGS){
        break;
      }
      else{
        current_arg += diff;
      }
    }
    else{
      error_flag = true;
    }
  }
  if(error_flag){
    std::cout << "Invalid argument syntax. Try the -h flag for help\n";
  }
  return !error_flag;
}



int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  srand(std::time(NULL));
  
  arg_parser parser(argc,(const char**)argv);
  
  if(!parser.parse()){
    return 0;
  }
  

  if(parser.show_flag){
    parser.gc.current.show();
  }
  
  if(parser.start_windowed_game){
    main_window window(parser.gc);
    Gtk::Main::run(window);
  }
  
  return 0;

  

  /*
  int i = 1;
  
  while(i<argc){
    if(str[i] == "-h" || str[i] == "--help"){
      show_help(NULL);
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
  
  */
}
