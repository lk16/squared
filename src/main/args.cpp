#include "main/args.hpp"

squared_args::squared_args(int argc,const char **argv):
  arg_parser_base<squared_args>(argc,argv),
  gc()
{
  func_map["--help"] = &squared_args::show_help;
  func_map["-h"] = &squared_args::show_help;
  func_map["--testing"] = &squared_args::testing_area_mask;
  func_map["-s"] = &squared_args::show_board;
  func_map["--learn"] = &squared_args::learn;
  func_map["-l"] = &squared_args::learn;
  func_map["--randomize"] = &squared_args::randomize_board;
  func_map["-r"] = &squared_args::randomize_board;
  func_map["--board"] = &squared_args::set_board;
  func_map["-b"] = &squared_args::set_board;
  func_map["--compress-book"] = &squared_args::compress_book;
  func_map["-cb"] = &squared_args::compress_book;
  func_map["-q"] = &squared_args::quit_if_game_over;
  func_map["-nb"] = &squared_args::no_book;
  func_map["--bot-type"] = &squared_args::set_bot_type;
  func_map["--loop"] = &squared_args::loop_game;
  func_map["--speed-test"] = &squared_args::speed_test;
  func_map["--pgn"] = &squared_args::process_pgn;
  func_map["--xot"] = &squared_args::use_xot;
  func_map["--lvl"] = &squared_args::set_level;
  func_map["-bbot"] = &squared_args::add_black_bot;
  func_map["-wbot"] = &squared_args::add_white_bot;
  
}

int squared_args::use_xot()
{
  gc->use_xot = true;
  return 1;
}

int squared_args::process_pgn()
{
  if(!has_enough_args(3)){
    return PARSING_ERROR;
  }
  gc->start_windowed_game = false;
  gc->pgn_task = new pgn_task_t;
  gc->pgn_task->search_depth = from_str<int>(std::string(get_arg(1)));
  gc->pgn_task->perfect_depth = from_str<int>(std::string(get_arg(2)));
  gc->pgn_task->filename = get_arg(3);
  return PARSING_IGNORE_OTHER_ARGS;
}


int squared_args::set_bot_type()
{
  if(!has_enough_args(1)){
    return PARSING_ERROR;
  }
  std::string name = get_arg(1);
  auto map = &bot_registration::bots();
  if(map->find(name) == map->end()){
    std::cout << "ERROR: no bot \"" << name << "\" found.\n";
    std::cout << "Found bots: ";
    for(auto it: *map){
      std::cout << it.first << " ";
    }
    std::cout << '\n';
    return PARSING_ERROR;
  } 
  gc->bot_type = name;
  return 2;
}

int squared_args::loop_game()
{
  gc->loop_game = true;
  return 1;
}

int squared_args::no_book()
{
  gc->use_book = false;
  return 1;
}


int squared_args::show_help(){
  std::cout << 
  "-h, --help\n"
  "show this help\n\n"
  "--testing:\n"
  "run the testing area, ignoring all other flags\n\n"
  "-b <board>, --board <board>\n"
  "use given board as start position, cannot be used with -r\n\n"
  "-r <number of moves>, --random <number of moves>\n"
  "use startboard after a given amount of random moves, cannot be used with -b\n\n"
  "-lb <search depth> <perfect depth>, -lw <search depth> <perfect depth>\n"
  "set the level of the black or white bot\n\n"
  "-s\n"
  "show given board in human readable format\n\n"
  "-l, --learn\n"
  "learn/improve the opening book, ignoring all other flags\n\n"
  "--set-[white|black]-valuation-file <file>\n"
  "Read field valuation information for specified bot from specified file.\n"
  "This can only be used after the appropriate -l[w|b] has been used.\n\n"
  "-q\n"
  "Quit program when game is finished\n\n";
  gc->start_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::show_board(){
  gc->start_windowed_game = false;
  gc->show_board_flag = true;
  return 1;
}

int squared_args::testing_area_mask(){
  gc->start_windowed_game = false;
  testing_area();
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::learn(){
  if(!has_enough_args(1)){
    return PARSING_ERROR;
  }
  gc->start_windowed_game = false;
  gc->learn_threads = from_str<int>(get_arg(1));
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::set_board(){
  if(!has_enough_args(1)){
    return PARSING_ERROR;
  }
  gc->board_state.b = board(get_arg(1));
  return 2;
}

int squared_args::set_level()
{
  if(!has_enough_args(2)){
    return PARSING_ERROR;
  }
  gc->search_depth = from_str<int>(get_arg(1));
  gc->perfect_depth = from_str<int>(get_arg(2));
  return 3;
}

int squared_args::add_black_bot()
{
  gc->add_bot(BLACK);
  return 1;
}


int squared_args::add_white_bot()
{
  gc->add_bot(WHITE);
  return 1;
}


int squared_args::randomize_board()
{
  if(!has_enough_args(1)){
    return PARSING_ERROR;    
  }
  gc->random_moves = from_str<int>(get_arg(1));
  return 2;
}

int squared_args::compress_book()
{
  gc->start_windowed_game = false;
  gc->compress_book = true;
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::quit_if_game_over()
{
  gc->quit_if_game_over = true;
  return 1;
}

int squared_args::speed_test()
{
  gc->run_speed_test = true;
  gc->start_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
}
