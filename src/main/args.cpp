#include "main/args.hpp"

squared_args::squared_args(int argc,const char **argv):
  arg_parser_base<squared_args>(argc,argv),
  gc()
{
  show_flag = false;
  start_windowed_game = true;
  use_book = true;
  
  func_map["--help"] = &squared_args::show_help;
  func_map["-h"] = &squared_args::show_help;
  func_map["--testing"] = &squared_args::testing_area_mask;
  func_map["-s"] = &squared_args::show_board;
  func_map["--learn"] = &squared_args::learn;
  func_map["-l"] = &squared_args::learn;
  func_map["--randomize"] = &squared_args::randomize_board;
  func_map["-r"] = &squared_args::randomize_board;
  func_map["-lb"] = &squared_args::set_black_level;
  func_map["-lw"] = &squared_args::set_white_level;
  func_map["--board"] = &squared_args::set_board;
  func_map["-b"] = &squared_args::set_board;
  func_map["--compress-book"] = &squared_args::compress_book;
  func_map["-cb"] = &squared_args::compress_book;
  func_map["-q"] = &squared_args::minus_q_flag;
  func_map["-nb"] = &squared_args::no_book;
  func_map["--bot-type"] = &squared_args::set_bot_type;
  func_map["--loop"] = &squared_args::loop_game;
  func_map["--speed-test"] = &squared_args::speed_test;
  func_map["--pgn"] = &squared_args::process_pgn;
  func_map["--xot"] = &squared_args::use_xot;
}

int squared_args::use_xot()
{
  gc.board_state.b.xot();
  return 1;
}

int squared_args::process_pgn()
{
  if(!has_enough_args(3)){
    return PARSING_ERROR;
  }
  start_windowed_game = false;
  pgn p(get_arg(3));
  bot_base* bot = bot_registration::bots()[gc.bot_type]();
  int level[2];
  level[0] = from_str<int>(get_arg(1));
  level[1] = from_str<int>(get_arg(2));
  bot->set_search_depth(level[0],level[1]);
  bot->disable_shell_output();
  std::cout << p.analyse(bot,true,true);
  delete bot;
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
  gc.bot_type = name;
  return 2;
}

int squared_args::loop_game()
{
  gc.loop_game = true;
  return 1;
}

int squared_args::no_book()
{
  use_book = false;
  for(int i=0;i<2;i++){
    if(gc.bot[i]){
      gc.bot[i]->disable_book();
    }
  }
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
  start_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::show_board(){
  start_windowed_game = false;
  show_flag = true;
  return 1;
}

int squared_args::testing_area_mask(){
  start_windowed_game = false;
  testing_area();
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::learn(){
  if(!has_enough_args(1)){
    return PARSING_ERROR;
  }
  start_windowed_game = false;
  int threads = from_str<int>(get_arg(1));
  book_t book(BOOK_PATH + gc.bot_type + "_book.csv");
  book.learn(gc.bot_type,threads);
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::set_board(){
  if(!has_enough_args(1)){
    return PARSING_ERROR;
  }
  gc.board_state.b = board(get_arg(1));
  return 2;
}

int squared_args::set_level(int color)
{
  if(!has_enough_args(2)){
    return PARSING_ERROR;
  }
  int lvl = from_str<int>(get_arg(1));
  int perf_lvl = from_str<int>(get_arg(2));
  gc.add_bot(color,lvl,perf_lvl);
  if(!use_book){
    gc.bot[color]->disable_book();
  }
  return 3;
}


int squared_args::set_black_level()
{
  return set_level(BLACK);
}

int squared_args::set_white_level()
{
  return set_level(WHITE);
}

int squared_args::randomize_board()
{
  if(!has_enough_args(1)){
    return PARSING_ERROR;    
  }
  board* b = &gc.board_state.b;
  int move_count = from_str<int>(get_arg(1));
  *b = b->do_random_moves(move_count);
  return 2;  
}

int squared_args::compress_book()
{
  book_t book(BOOK_PATH + gc.bot_type + "_book.csv");
  book.clean();
  start_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::minus_q_flag()
{
  gc.quit_if_game_over = true;
  return 1;
}

int squared_args::speed_test()
{
  bot_base* bot = bot_registration::bots()[gc.bot_type]();
  bot->disable_shell_output();
  bot->disable_book();
  std::cout << "testing speed of bot_" << gc.bot_type << " on this board:\n";
  std::cout << gc.board_state.b.to_ascii_art(gc.board_state.turn);
  for(int i=1;i<=60;i++){
    board b,dummy;
    b = gc.board_state.b;
    bot->set_search_depth(i,i);
    bot->do_move(&b,&dummy);
    long long speed = bot->stats.get_nodes_per_second();
    std::cout << "At depth " << i << ":\t" << big_number(speed) << " nodes/s ";
    std::cout << "\ttook " << bot->stats.get_seconds() << " seconds.\n";
    if(bot->stats.get_seconds() > 10.0){
      break;
    }
  }
  start_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
}
