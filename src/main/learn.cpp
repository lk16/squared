#include "main/learn.hpp"

int get_move_index(const board* before, const board* after)
{
  std::bitset<64> diff = before->get_non_empty_fields();
  diff ^= after->get_non_empty_fields();
  return find_first_set_64(diff.to_ulong());
}



book_t read_book()
{
  /* book format:
   * board in database format,depth,best_move index (0 .. 63)
   */
  
  book_t res;
  csv book_file(BOOK_PATH + "book.csv");
  while(true){
    csv::line_t line = book_file.get_line();
    if(book_file.get_file()->fail()){
      return res; 
    }
    book_value bv;
    bv.depth = fromstr<int>(line[1]);
    bv.best_move = fromstr<int>(line[2]);
    res[line[0]] = bv;
  }
  return res;
}


void learn_book()
{
  const int min_learn_depth = 8;
  
  int learn_level = min_learn_depth;
  book_t book = read_book();
  
  bot_ali bot(COLOR_UNDEFINED,learn_level,learn_level);
  bot.disable_shell_output();
  
  
  csv book_file(BOOK_PATH + "book.csv");
  book_t::iterator book_it;
  
  csv moves_file(BOOK_PATH + "moves.csv");
  
  
  while(true){
    csv::line_t line = moves_file.get_line();
    if(moves_file.get_file()->fail()){
      break;
    }
    
    // if this board is not found or not evaluated at a deep enough level
    // evaluate it now
    book_it = book.find(line[0]);
    if(book_it == book.end() || (book_it->second.depth < learn_level)){
     int index = learn_move(&bot,line[0],learn_level,-1,-1);
      
      csv::line_t book_line;
      book_line.push_back(line[0]);
      book_line.push_back(tostr<int>(learn_level));
      book_line.push_back(tostr<int>(index));
      book_file.append_line(book_line);
      
      book_value bv;
      bv.best_move = index;
      bv.depth = learn_level;
      book[line[0]] = bv;
      
    }
  }
  
    
  
    
  while(true){
    int n = 0;
    int total = 0;
    learn_level++;
    bot.set_search_depth(learn_level,learn_level);
    
    for(book_it = book.begin();book_it!=book.end();book_it++){
      if(book_it->second.depth < learn_level){
        total++;
      }
    }
    
    for(book_it = book.begin();book_it!=book.end();book_it++){
     
      if(book_it->second.depth < learn_level){
     
        int index = learn_move(&bot,book_it->first,learn_level,n,total);
     
        csv::line_t book_line;
        book_line.push_back(book_it->first);
        book_line.push_back(tostr<int>(learn_level));
        book_line.push_back(tostr<int>(index));
        book_file.append_line(book_line);
        
        book_it->second.best_move = index;
        book_it->second.depth = learn_level;
        
        n++;
      }
    }
    
  }
  
}

int learn_move(bot_base* bot,const std::string& board_str,int depth,int n,int total){
  timeval start,end;
  board after,before(board_str);
  std::cout << "learning " << board_str;
  std::cout << " at depth " << depth << " ... ";
  std::cout.flush();
  gettimeofday(&start,NULL);
  bot->do_move(&before,&after);
  gettimeofday(&end,NULL);
  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0)); 
  
  
  std::cout << "Took " << std::setw(5) << time_diff;
  std::cout << " seconds!";
  if(n != -1){
    std::cout << " (" << n+1 << '/' << total << ')';
  }
  std::cout << std::endl << std::flush;
  return get_move_index(&before,&after);
}


