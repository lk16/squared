#include <gtkmm.h>

#include <map>
#include "gui/main_window.hpp"
#include "util/testing.hpp"
#include "util/args.hpp"

struct squared_arg_t
{
  
  arg_parser<squared_arg_t>* parser;
  
  game_control gc;
  bool show_flag;
  bool start_windowed_game;
  
  squared_arg_t():
    gc()
  {
    show_flag = false;
    start_windowed_game = true;
    parser = NULL;
  }
  
  void fill_func_map();
  
  // modifiers
  int show_help();
  int show_board();
  int testing_area_mask();
  int learn();
  int set_board();
  int randomize_board();
  int set_white_level();
  int set_black_level();
  int compress_book();
  
};


int squared_arg_t::show_help(){
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

int squared_arg_t::show_board(){
  start_windowed_game = false;
  show_flag = true;
  return 1;
}

int squared_arg_t::testing_area_mask(){
  start_windowed_game = false;
  testing_area();
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_arg_t::learn(){
  book_t book(BOOK_PATH + "book.csv");
  bot_ali bot(-1,-1); 
  book.learn(&bot);
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_arg_t::set_board(){
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;
  }
  gc.current = board(parser->get_arg(1));
  return 2;
}

int squared_arg_t::set_black_level()
{
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;
  }
  int lvl = fromstr<int>(parser->get_arg(1));
  gc.add_bot(BLACK,lvl,max(16,2*lvl));
  return 2;
}

int squared_arg_t::set_white_level()
{
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;
  }
  int lvl = fromstr<int>(parser->get_arg(1));
  gc.add_bot(BLACK,lvl,max(16,2*lvl));
  return 2;
}

int squared_arg_t::randomize_board()
{
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;    
  }
  gc.current = gc.current.do_random_moves(fromstr<int>(parser->get_arg(1)));
  return 2;  
}

int squared_arg_t::compress_book()
{
  book_t book(BOOK_PATH + "book.csv");
  book.remove_obsolete_lines();
  start_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
}



int main(int argc,char **argv){
  srand(std::time(NULL));
  
  arg_parser<squared_arg_t> parser(argc,(const char**)argv);
  
  parser.func_map["--help"] = &squared_arg_t::show_help;
  parser.func_map["-h"] = &squared_arg_t::show_help;
  parser.func_map["--testing"] = &squared_arg_t::testing_area_mask;
  parser.func_map["-s"] = &squared_arg_t::show_board;
  parser.func_map["--learn"] = &squared_arg_t::learn;
  parser.func_map["-l"] = &squared_arg_t::learn;
  parser.func_map["--randomize"] = &squared_arg_t::randomize_board;
  parser.func_map["-r"] = &squared_arg_t::randomize_board;
  parser.func_map["-lb"] = &squared_arg_t::set_black_level;
  parser.func_map["-lw"] = &squared_arg_t::set_white_level;
  parser.func_map["--board"] = &squared_arg_t::set_board;
  parser.func_map["-b"] = &squared_arg_t::set_board;
  parser.func_map["--compress-book"] = &squared_arg_t::compress_book;
  parser.func_map["-cb"] = &squared_arg_t::compress_book;
  
  
  if(!parser.parse()){
    return 0;
  }
  
  squared_arg_t arg_data = parser.t;

  if(arg_data.show_flag){
    arg_data.gc.current.show();
  }
  
  if(arg_data.start_windowed_game){
    Gtk::Main kit(argc,argv);
    main_window window(arg_data.gc);
    Gtk::Main::run(window);
  }
  
  return 0;
}
