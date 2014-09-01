#include "util/pgn.hpp"

pgn::pgn(const std::string& file_name)
{
  std::ifstream file(file_name);
  const int line_size = 1024;
  char line[line_size];
  
  while(file.good()){
    file.getline(line,line_size);
    if(line[0] == '['){
      std::string line_str(line);
      int key_end = line_str.find_first_of(' ',0);
      std::string key = line_str.substr(1,key_end-1);
      int value_start = line_str.find_first_of('\"',key_end-1) + 1;
      int value_end = line_str.find_first_of('\"',value_start+1);
      std::string value = line_str.substr(value_start,value_end-value_start);
      data[key] = value;
    }
    else{
      for(std::string& s: str_explode(line,' ')){
        int move = position_to_index(s);
        if(move >= 0){
          moves.push_back(move);
        }
      }
    }
  }
}

int pgn::position_to_index(const std::string& str) const
{
  if(str == "--"){
    return PASS;
  }
  if(str.length()==2 && in_bounds(str[0],'a','h') && in_bounds(str[1],'1','8')){
    return 8*(str[1] - '1') + str[0] - 'a';
  }
  return UNKNOWN;
}

std::string pgn::index_to_position(int index) const
{
  if(out_bounds(index,0,63)){
    return "??";
  }
  char s[3];
  s[0] = 'a' + (index%8);
  s[1] = '1' + (index/8);
  s[2] = '\0';
  return std::string(s);
}



void pgn::show() const
{
  board b;
  b.reset();
  std::cout << b.to_ascii_art();
  for(int i: moves){
    b.do_move(i);
    std::cout << b.to_ascii_art();
  }
}

std::string pgn::get_data(const std::string& key)
{
  auto it = data.find(key);
  return (it == data.end()) ? "not found" : it->second;
}



std::string pgn::analyse(bot_base* bot, bool analyse_white, bool analyse_black)
{
  (void)analyse_black;
  (void)analyse_white;
  
  std::stringstream ss;
  ss << ".------------------------------------.\n";
  ss << "|        Squared game analsyis       |\n";
  ss << "|   Written by LK16, 31 august 2014  |\n";
  ss << "|   http://github.com/lk16/squared   |\n";
  ss << "'------------------------------------'\n";
  ss << "\n";
  ss << "Disclaimer: this software is very new and may contain bugs!\n";
  ss << "Read this harmless text file at your own risk!\n";
  ss << '\n';  
  ss << "White: " << get_data("White") << " (Rating: ";
  ss << get_data("WhiteRating") << ")\n";
  ss << "Black: " << get_data("Black") << " (Rating: ";
  ss << get_data("BlackRating") << ")\n";
  ss << "Date: " << get_data("Date") << '\n';
  ss << "\n";
  ss << "Evaluating bot: bot_" << bot->get_name() << '\n';
  ss << "Search depth: " << bot->get_search_depth() << '\n';
  ss << "Perfect search depth: " << bot->get_perfect_depth() << '\n';
  
  
  
  std::string players[2] = {
    get_data("Black"),
    get_data("White")
  };
  
  int turn = 0;
  auto move_it = moves.begin();
  board b,player_board,bot_board;
  b.reset();
  
  const int d = bot->get_search_depth();
  const int pd = bot->get_perfect_depth();
  
  
  while(true){
    if(b.count_valid_moves() == 0){
      ss << players[turn] << " has to pass\n";
      turn = 1 - turn;
      b.switch_turn();
      continue;
    }
    ss << "\n\n\n";
    player_board = b;
    player_board.do_move(*move_it);
    bot->do_move(&b,&bot_board);
    
    ss << b.to_ascii_art(turn) << '\n';
    ss << players[turn] << " plays " << index_to_position(*move_it) << "\n";
    ss << player_board.to_ascii_art(1-turn);
    
    bool right_move = false;
    
    if(player_board.to_database_board() == bot_board.to_database_board()){
      right_move = true;
    }
    else{
      bot->set_search_depth(d-1,pd);
      board dummy;
      bot->do_move(&player_board,&dummy);
      int heur1 = bot->get_last_move_heur();
      bot->do_move(&bot_board,&dummy);
      int heur2 = bot->get_last_move_heur();
      if(heur1 == heur2){
        right_move = true;
      }
      bot->set_search_depth(d,pd);
    }
    
    
    
    if(right_move){
      ss << "Bot: right move!\n";
    }
    else{
      int move = bot_board.get_move_index(&b);
      ss << "\nBot: wrong move, I would have played ";
      ss << index_to_position(move) << '\n';
      ss << bot_board.to_ascii_art(1-turn);
    }
    
    move_it++;
    b = player_board;
    if(move_it == moves.end()){
      break;
    }
    turn = 1 - turn;
  }
  
  int count[2];
  count[0] = bits64_count(b.opp);
  count[1] = bits64_count(b.me);
  
  ss << '\n';
  
  if(count[0] > count[1]){
    ss << players[turn] << " wins: ";
  }
  else if(count[0] < count[1]){
    ss << players[1-turn] << " wins: ";
  }
  else{
    ss << "The game ended in draw: ";
  }
  ss << count[0] << '-' << count[1] << '\n';
  
  
  return ss.str();
}
