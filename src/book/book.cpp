#include "book/book.hpp"
#include "bots/bot_ali.hpp"


book_t::book_t(const std::string& _filename):
  filename(_filename)
{
  
  /* book format:
   * board in database format,depth,best_move index (0 .. 63)
   */
  
  csv book_file(_filename);
  while(true){
    csv::line_t line = book_file.get_line();
    if(book_file.get_file()->fail()){
      break; 
    }
    book_t::value bv;
    bv.depth = fromstr<int>(line[1]);
    bv.best_move = fromstr<int>(line[2]);
    data[line[0]] = bv;
  }
}



int book_t::get_move_index(const board* before, const board* after)
{
  std::bitset<64> diff = before->get_non_empty_fields();
  diff ^= after->get_non_empty_fields();
  return find_first_set_64(diff.to_ulong());
}

void book_t::print_stats() const
{
  citer book_it;
  std::map<int,int> book_stats;
  
  for(book_it = data.begin(); book_it!=data.end(); book_it++){
    int n = book_it->second.depth;
    if(book_stats.find(n) == book_stats.end()){
      book_stats[n] = 1;
    }
    else{
      book_stats[n]++;
    }
  }
  
  for(std::map<int,int>::const_iterator it=book_stats.begin();it!=book_stats.end();it++){
    std::cout << "Book contains " << it->second << " boards at depth ";
    std::cout << it->first << std::endl;
  }
}


void book_t::add_from_moves_file(const std::string& fname)
{
  
  csv moves_file(fname);
  
  while(true){
    csv::line_t line = moves_file.get_line();
    if(moves_file.get_file()->fail()){
      break;
    }

    citer book_it = data.find(line[0]);
    if(book_it == data.end()){
      
      
      // add dummy data to be evaluated later
      // since depth = 0, this will never be used by any decent bot
      value bv;
      bv.best_move = 0;
      bv.depth = 0;
      data[line[0]] = bv;
      
    }
  }
}




void book_t::learn(bot_base* bot)
{
  int learn_level = min_learn_depth;

  csv book_file(filename);
   
  citer book_citer;
  
  bot->disable_shell_output();
  
  print_stats();
  
  while(true){
    int n_left = 0;
    bot->set_search_depth(learn_level,learn_level);
    
    for(book_citer = data.begin();book_citer!=data.end();book_citer++){
      if(book_citer->second.depth < learn_level){
        n_left++;
      }
    }
    
    iter book_iter;
    for(book_iter = data.begin();book_iter!=data.end();book_iter++){
     
      if(book_iter->second.depth < learn_level){
     
        int index = learn_move(bot,book_iter->first,learn_level,n_left);
        
        add_to_book_file(book_iter->first,learn_level,index);
        
        book_iter->second.best_move = index;
        book_iter->second.depth = learn_level;
        
        n_left--;
      }
    }
    
    learn_level++;
  }
  
}

void book_t::add_to_book_file(const std::string& b, int depth, int move)
{
  csv book_file(filename);
  
  csv::line_t book_line;
  book_line.push_back(b);
  book_line.push_back(tostr<int>(depth));
  book_line.push_back(tostr<int>(move));
  book_file.append_line(book_line);
}




int book_t::learn_move(bot_base* bot,const std::string& board_str,int depth,int n_left){
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
  if(n_left != -1){
    std::cout << " (" << n_left-1 << " left)";
  }
  std::cout << std::endl << std::flush;
  return get_move_index(&before,&after);
}

void book_t::remove_obsolete_lines() const
{
  /* test if file exists, return if not */
  if(access(filename.c_str(), F_OK) == -1){
    return;    
  }
  
  /* create backup file, to prevent losing data */
  std::rename(filename.c_str(),(filename + ".bak").c_str());

  /* fill new file with data */
  csv book_file(filename);  
  for(citer it=data.begin();it!=data.end();it++){
    csv::line_t book_line;
    book_line.push_back(it->first);
    book_line.push_back(tostr<int>(it->second.depth));
    book_line.push_back(tostr<int>(it->second.best_move));
    book_file.append_line(book_line);
  }
  
  /* remove backup file */
  std::remove((filename + ".bak").c_str());
}

