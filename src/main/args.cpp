#include "main/args.hpp"

squared_args::squared_args(int argc,const char **argv):
  arg_parser_base<squared_args>(argc,argv),
  gc()
{
  add_modifier("--testing",&squared_args::testing_area_mask,"","Run test area. For developers only.");
  duplicate_modifier("-t","--testing");
  
  add_modifier("--show",&squared_args::show_board,"","Print a specified board to standard output and exit. Useful for developers.");
  duplicate_modifier("-s","--show");
  
  add_modifier("--learn",&squared_args::learn,"<num_threads>","Learn the book with.");
  duplicate_modifier("-l","--learn");
  
  add_modifier("--random-moves",&squared_args::randomize_board,"<num_moves>","Transform board by doing random moves.");
  duplicate_modifier("-r","--random-moves");
  
  add_modifier("--board",&squared_args::set_board,"<board_string>,","Set the start board.");
  duplicate_modifier("-b","--board");
  
  add_modifier("--compress-book",&squared_args::compress_book,"","Remove incorrect and duplicate entries from the book.");
  duplicate_modifier("-cb","--compress-book");
  
  add_modifier("-q",&squared_args::quit_if_game_over,"","Quit the program after one game.");
  
  add_modifier("-nb",&squared_args::no_book,"","Disable the opening book.");
  
  add_modifier("--bot-type",&squared_args::set_bot_type,"<bot_name>","Specify the bot type to use.");
  
  add_modifier("--loop",&squared_args::loop_game,"","Restart game automatically after a game has finished.");
  
  add_modifier("--speed-test",&squared_args::speed_test,"","Run the speed test. Useful for developers.");
  
  add_modifier("--pgn",&squared_args::process_pgn,"<path_to_pgn>","Process PGN files.");
  
  add_modifier("--xot",&squared_args::use_xot,"","Run a xot game.");
  
  add_modifier("-lvl",&squared_args::set_level,"<search_depth> <perfect_depth>","Set the bot level. Needs two integers behind it.");
  
  add_modifier("-bbot",&squared_args::add_black_bot,"","Enable a white bot. Can be used in combination with black bot.");
  
  add_modifier("-wbot",&squared_args::add_white_bot,"","Enable a black bot. Can be used in combination with white bot.");
  
  add_modifier("--tournament",&squared_args::tournament_flag,"Mix of {-lvl <search_depth> <perfect_depth>} and any bot name","Runs a tournament between bots. Change in level applies to bots specified after the bot names only.");
}

int squared_args::use_xot()
{
  gc->use_xot = true;
  return 1;
}

int squared_args::process_pgn()
{
  if(!has_enough_subargs(1)){
    return PARSING_ERROR;
  }
  gc->pgn_task = new pgn_task_t;
  gc->pgn_task->filename = get_subarg(1);
  gc->run_windowed_game = false;
  return PARSING_IGNORE_OTHER_ARGS;
}


int squared_args::set_bot_type()
{
  if(!has_enough_subargs(1)){
    return PARSING_ERROR;
  }
  std::string name = get_subarg(1);
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

int squared_args::show_board(){
  gc->show_board_flag = true;
  return 1;
}

int squared_args::testing_area_mask(){
  gc->run_windowed_game = false;
  testing_area();
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::learn(){
  if(!has_enough_subargs(1)){
    return PARSING_ERROR;
  }
  gc->run_windowed_game = false;
  gc->learn_threads = from_str<int>(get_subarg(1));
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::set_board(){
  if(!has_enough_subargs(1)){
    return PARSING_ERROR;
  }
  gc->board_string = get_subarg(1);
  return 2;
}

int squared_args::set_level()
{
  if(!has_enough_subargs(2)){
    return PARSING_ERROR;
  }
  gc->search_depth = from_str<int>(get_subarg(1));
  gc->perfect_depth = from_str<int>(get_subarg(2));
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
  if(!has_enough_subargs(1)){
    return PARSING_ERROR;    
  }
  gc->random_moves = from_str<int>(get_subarg(1));
  return 2;
}

int squared_args::compress_book()
{
  gc->run_windowed_game = false;
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
  gc->run_windowed_game = false;
  gc->run_speed_test = true;
  return PARSING_IGNORE_OTHER_ARGS;
}

int squared_args::tournament_flag()
{
  gc->tournament = new tournament_t;
  int n = 1; 
  
  while(n != count_remaining_args()){
    std::string arg = get_subarg(n);
    if(arg == "-lvl"){
      if(!has_enough_subargs(n+2)){
        std::cerr << "-lvl needs two sub arguments.\n";
        return PARSING_ERROR;
      }
      gc->search_depth = from_str<int>(get_subarg(n+1));
      gc->perfect_depth = from_str<int>(get_subarg(n+2));
      n+=3;
      continue;
    }
    auto it = bot_registration::bots().find(arg);
    if(it == bot_registration::bots().end()){
      std::cerr << "No bot named \"" << arg << "\"\n";
      return PARSING_ERROR;
    }
    gc->tournament->add_entrant(arg,gc->search_depth,gc->perfect_depth);
    ++n;
  }
  
  
  
  
  
  
  return PARSING_IGNORE_OTHER_ARGS;
}
