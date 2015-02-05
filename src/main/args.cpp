#include "main/args.hpp"

squared_arg_t::squared_arg_t():
  gc()
{
  show_flag = false;
  start_windowed_game = true;
  parser = NULL;
  use_book = true;
}

void squared_arg_t::init_map()
{
  parser->func_map["--help"] = &squared_arg_t::show_help;
  parser->func_map["-h"] = &squared_arg_t::show_help;
  parser->func_map["--testing"] = &squared_arg_t::testing_area_mask;
  parser->func_map["-s"] = &squared_arg_t::show_board;
  parser->func_map["--learn"] = &squared_arg_t::learn;
  parser->func_map["-l"] = &squared_arg_t::learn;
  parser->func_map["--randomize"] = &squared_arg_t::randomize_board;
  parser->func_map["-r"] = &squared_arg_t::randomize_board;
  parser->func_map["-lb"] = &squared_arg_t::set_black_level;
  parser->func_map["--set-white-valuation-file"] = &squared_arg_t::set_white_valuation;
  parser->func_map["--set-black-valuation-file"] = &squared_arg_t::set_black_valuation;
  parser->func_map["-lw"] = &squared_arg_t::set_white_level;
  parser->func_map["--board"] = &squared_arg_t::set_board;
  parser->func_map["-b"] = &squared_arg_t::set_board;
  parser->func_map["--compress-book"] = &squared_arg_t::compress_book;
  parser->func_map["-cb"] = &squared_arg_t::compress_book;
  parser->func_map["-q"] = &squared_arg_t::minus_q_flag;
  parser->func_map["-nb"] = &squared_arg_t::no_book;
  parser->func_map["-tb"] = &squared_arg_t::train_bot_ali;
  parser->func_map["--bot-type"] = &squared_arg_t::set_bot_type;
  parser->func_map["--loop"] = &squared_arg_t::loop_game;
  parser->func_map["--speed-test"] = &squared_arg_t::speed_test;
  parser->func_map["--pgn"] = &squared_arg_t::process_pgn;
}

int squared_arg_t::process_pgn()
{
  if(!parser->has_enough_args(3)){
    return PARSING_ERROR;
  }
  start_windowed_game = false;
  pgn p(parser->get_arg(3));
  bot_base* bot = bot_registration::bots()[gc.bot_type]();
  int level[2];
  level[0] = from_str<int>(parser->get_arg(1));
  level[1] = from_str<int>(parser->get_arg(2));
  bot->set_search_depth(level[0],level[1]);
  bot->disable_shell_output();
  std::cout << p.analyse(bot,true,true);
  delete bot;
  return PARSING_IGNORE_OTHER_ARGS;
}


int squared_arg_t::set_bot_type()
{
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;
  }
  std::string name = parser->get_arg(1);
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

int squared_arg_t::loop_game()
{
  gc.loop_game = true;
  return 1;
}


int squared_arg_t::train_bot_ali()
{
  start_windowed_game = false;
  bot_ali ali;
  ali.set_search_depth(6,12);
  int v[] = {347,-47,-24,-33,-119,-35,-26,-23,-7,3};
  ali.set_location_values(v);
  bot_ali_trainer(&ali).run();
  return PARSING_IGNORE_OTHER_ARGS;
}


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
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;
  }
  start_windowed_game = false;
  int threads = from_str<int>(parser->get_arg(1));
  book_t book(BOOK_PATH + gc.bot_type + "_book.csv");
  book.learn(gc.bot_type,threads);
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_arg_t::set_board(){
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;
  }
  gc.board_state.b = board(parser->get_arg(1));
  return 2;
}

int squared_arg_t::set_level(int color)
{
  if(!parser->has_enough_args(2)){
    return PARSING_ERROR;
  }
  int lvl = from_str<int>(parser->get_arg(1));
  int perf_lvl = from_str<int>(parser->get_arg(2));
  gc.add_bot(color,lvl,perf_lvl);
  if(!use_book){
    gc.bot[color]->disable_book();
  }
  return 3;
}


int squared_arg_t::set_black_level()
{
  return set_level(BLACK);
}

int squared_arg_t::set_white_level()
{
  return set_level(WHITE);
}

int squared_arg_t::randomize_board()
{
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;    
  }
  board* b = &gc.board_state.b;
  int move_count = from_str<int>(parser->get_arg(1));
  *b = b->do_random_moves(move_count);
  return 2;  
}

int squared_arg_t::compress_book()
{
  book_t book(BOOK_PATH + gc.bot_type + "_book.csv");
  book.clean();
  start_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_arg_t::set_valuation(int color){
  if(!parser->has_enough_args(1)){
    return PARSING_ERROR;
  }

  bot_ali* ali = dynamic_cast<bot_ali*>(gc.bot[color]);
  if(!ali){ 
    return PARSING_ERROR;
  }
  if(!ali->set_location_values_from_file(parser->get_arg(1))){
    return PARSING_ERROR;
  } else {
    ali->disable_book();
    return 2;
  }
}

int squared_arg_t::set_white_valuation(){
  return set_valuation(WHITE);
}
int squared_arg_t::set_black_valuation(){
  return set_valuation(BLACK);
}

int squared_arg_t::minus_q_flag()
{
  gc.quit_if_game_over = true;
  return 1;
}

int squared_arg_t::speed_test()
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
