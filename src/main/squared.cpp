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
  bool use_book;
  
  squared_arg_t():
    gc()
  {
    show_flag = false;
    start_windowed_game = true;
    parser = NULL;
    use_book = true;
  }
    
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
  int minus_q_flag();
  int no_book();
  
};

int squared_arg_t::no_book()
{
  use_book = false;
  for(int i=0;i<2;i++){
    if(gc.bot[i]){
      gc.bot[i]->disable_book();
    }
  }
  return 1;
}


int squared_arg_t::show_help(){
  std::cout << 
  "-h, --help\n"
  "show this help\n\n"
  "Invalid argument syntax, try one of these:  \n\n"
  "--testing:\n"
  "run the testing area, ignoring all other flags\n\n"
  "-b <board>, --board <board>\n"
  "use given board as start position, cannot be used with -r\n\n"
  "-r <number of moves>, --random <number of moves>\n"
  "use startboard after a given amount of random moves, cannot be used with -b\n\n"
  "-lb <search depth> <perfect depth>, -lw <search depth><perfect depth>\n"
  "set the level of the black or white bot\n\n"
  "-s\n"
  "show given board in human readable format\n\n"
  "-l, --learn\n"
  "learn/improve the opening book, ignoring all other flags\n\n"
  "-q\n"
  "Quit program when game is finished\n\n";
  start_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
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
  gc.board_state.b = board(parser->get_arg(1));
  return 2;
}

int squared_arg_t::set_black_level()
{
  if(!parser->has_enough_args(2)){
    return PARSING_ERROR;
  }
  int lvl = from_str<int>(parser->get_arg(1));
  int perf_lvl = from_str<int>(parser->get_arg(2));
  gc.add_bot(BLACK,lvl,perf_lvl);
  if(!use_book){
    gc.bot[BLACK]->disable_book();
  }
  return 3;
}

int squared_arg_t::set_white_level()
{
  if(!parser->has_enough_args(2)){
    return PARSING_ERROR;
  }
  int lvl = from_str<int>(parser->get_arg(1));
  int perf_lvl = from_str<int>(parser->get_arg(2));
  gc.add_bot(WHITE,lvl,perf_lvl);
  if(!use_book){
    gc.bot[WHITE]->disable_book();
  }
  return 3;
}

int squared_arg_t::randomize_board()
{
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;    
  }
  gc.board_state.b = gc.board_state.b.do_random_moves(from_str<int>(parser->get_arg(1)));
  return 2;  
}

int squared_arg_t::compress_book()
{
  book_t book(BOOK_PATH + "book.csv");
  book.clean();
  start_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_arg_t::minus_q_flag()
{
  gc.quit_if_game_over = true;
  return 1;
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
  parser.func_map["-q"] = &squared_arg_t::minus_q_flag;
  parser.func_map["-nb"] = &squared_arg_t::no_book;
  
  
  if(!parser.parse()){
    return 0;
  }
  
  squared_arg_t arg_data = parser.t;

  if(arg_data.show_flag){
    arg_data.gc.board_state.b.show();
  }
  
  if(arg_data.start_windowed_game){
    Gtk::Main kit(argc,argv);
     
    main_window window;
    arg_data.gc.mw = &window;
    window.control = &arg_data.gc;
    window.update_fields();
    arg_data.gc.connect_timeout_signal();
    Gtk::Main::run(window);
  }
  
  return 0;
}
