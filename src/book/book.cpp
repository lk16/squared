#include "book/book.hpp"
#include "bots/bot_ali.hpp"


book_t::book_t(const std::string& _filename):
  filename(_filename)
{
  
  /* book format:
   * board in database format,depth,best_move index (0 .. 63)
   */
  
  int errors = 0;
  csv book_file(_filename);
  while(true){
    csv::line_t line = book_file.get_line();
    if(book_file.get_file()->fail()){
      break; 
    }
    if(line.size() < value::NEEDED_COLUMNS){
      errors++;
      continue;
    }
    book_t::value bv(line);
    
    if(!is_correct_entry(line[0],bv)){
      errors++;
      continue;
    }
    data[line[0]] = bv;
  }
  
  if(errors > 0){
    std::cout << "Found " << errors;
    std::cout << " incorrect entries in ";
    std::cout << '\"' << _filename << '\"' << std::endl;
  }
  
}

bool book_t::is_correct_entry(const std::string& bs,const book_t::value& bv) const
{
  if(out_bounds<int>(bv.depth,0,60) || out_bounds<int>(bv.best_move,0,63)){
    return false;
  }
  if(bs.length() != 32){
    return false;
  }
  
  board b(bs);
  if((b.me ^ b.opp) != 0ull){
    return false;
  }
  
  if(!b.is_valid_move(bv.best_move)){
    return false;
  }
    
  return true;
}

book_t::value::value(const csv::line_t& line)
{
  depth = from_str<int>(line[1]);
  best_move = from_str<int>(line[2]);
}

book_t::value::value(int bm, int d)
{
  best_move = bm;
  depth = d;
}


int book_t::get_move_index(const board* before, const board* after)
{
  std::bitset<64> diff = before->get_non_empty_fields();
  diff ^= after->get_non_empty_fields();
  return bits64_find_first(diff.to_ulong());
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
  book_line.push_back(to_str<int>(depth));
  book_line.push_back(to_str<int>(move));
  book_file.append_line(book_line);
}




int book_t::learn_move(bot_base* bot,const std::string& board_str,int depth,int n_left){
  board after,before(board_str);
  std::cout << board_str << " at depth " << depth << '\t';
  std::cout.flush();
  
  bot->stats.start_timer();
  
  bot->do_move(&before,&after);
  
  bot->stats.stop_timer();
  
  std::cout << (int)bot->stats.get_seconds() << " s\t";
  std::cout << bot->stats.get_nodes_per_second() << "n/s\t";
  if(n_left != -1){
    std::cout << n_left-1 << " left\t";
  }
  std::cout << std::endl << std::flush;
  return get_move_index(&before,&after);
}

void book_t::remove_obsolete_lines() const
{
  /* test if file exists, return if not */
  if(access(filename.c_str(), F_OK) == -1){
    std::cout << "file \"" << filename << "\" not found." << std::endl;
    return;    
  }
    
  /* create backup file, to prevent losing data */
  std::rename(filename.c_str(),(filename + ".bak").c_str());

  /* fill new file with data */
  csv book_file(filename);  
  
  for(citer it=data.begin();it!=data.end();it++){
    csv::line_t book_line;
    book_line.push_back(it->first);
    book_line.push_back(to_str<int>(it->second.depth));
    book_line.push_back(to_str<int>(it->second.best_move));
    
    if(
      board(it->first).is_valid_move(it->second.best_move)
      && it->first.length() == 32      
    ){
      book_file.append_line(book_line);
    }
  }
    
  /* remove backup file */
  std::remove((filename + ".bak").c_str());
  
  std::cout << "Successfully cleaned the book." << std::endl;
  
}

