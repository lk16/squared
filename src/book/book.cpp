#include "book/book.hpp"
#include "bots/bot_ali.hpp"


book_t::book_t(const std::string& _filename):
  csv_file(_filename)
{
  load();
}

std::string book_t::get_filename() const
{
  return csv_file.get_name();
}


bool book_t::is_correct_entry(const std::string& bs,const book_t::value& bv) const
{
  board b(bs);
  
  return true
    && in_bounds<int>(bv.depth,0,60) 
    && in_bounds<int>(bv.best_move,0,63)
    && (bs.length() == 32)
    && ((b.me & b.opp) == 0ull)
    && (((b.me | b.opp) & 0x0000001818000000) == 0x0000001818000000)
    && b.is_valid_move(bv.best_move)
    && (b == board(b.to_database_string()));
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
  return bits64_find_first(before->get_non_empty_fields() ^ after->get_non_empty_fields());
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
  
  for(auto it: book_stats){
    std::cout << "Book contains " << it.second << " boards at depth ";
    std::cout << it.first << std::endl;
  }
}


void book_t::learn(bot_base* bot)
{
  int learn_level = MIN_LEARN_DEPTH;
   
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
     
        board before(book_iter->first);
        
        board after = learn_move(bot,&before,learn_level,n_left);
        
        add(&before,&after,learn_level);
        
        book_iter->second.best_move = get_move_index(&before,&after);
        book_iter->second.depth = learn_level;
        
        n_left--;
      }
    }
    
    learn_level++;
  }
  
}

void book_t::add(const board* before,const board* after,int depth)
{
  std::string str = before->to_database_string();
  board before_normalized(str);
  int rot = before->get_rotation(&before_normalized);
  board after_normalized = after->rotate(rot);
  
  
  citer it = data.find(str);
  if(true 
    && (before->count_discs() < ENTRY_MAX_DISCS)
    && (depth >= MIN_LEARN_DEPTH)
    && ((it == data.end()) || (depth > it->second.depth))
  ){
    
    csv::line_t book_line;
    int move = get_move_index(&before_normalized,&after_normalized);
        
    book_line.push_back(str);
    book_line.push_back(to_str<int>(depth));
    book_line.push_back(to_str<int>(move));
    
    csv_file.append_line(book_line);
    
    data[str] = value(move,depth);
  }
}




board book_t::learn_move(bot_base* bot,const board* b,int depth,int n_left){
  board after;
  std::cout << b->to_database_string() << " at depth " << depth << '\t';
  std::cout.flush();
  
  bot->stats.start_timer();
  
  bot->do_move(b,&after);
  
  bot->stats.stop_timer();
  
  std::cout << (int)bot->stats.get_seconds() << " s\t";
  std::cout << bot->stats.get_nodes_per_second() << "n/s\t";
  if(n_left != -1){
    std::cout << n_left-1 << " left\t";
  }
  std::cout << std::endl << std::flush;
  return after;
}

void book_t::clean() const
{
  /* test if file exists, return if not */
  if(access(get_filename().c_str(), F_OK) == -1){
    std::cout << "file \"" << get_filename() << "\" not found." << std::endl;
    return;    
  }
    
  /* create backup file, to prevent losing data */
  std::rename(get_filename().c_str(),(get_filename() + ".bak").c_str());

  /* fill new file with data */
  csv new_book_file(get_filename());  
  
  for(auto it: data){
    if(is_correct_entry(it.first,it.second)){
      csv::line_t book_line;
      book_line.push_back(it.first);
      book_line.push_back(to_str<int>(it.second.depth));
      book_line.push_back(to_str<int>(it.second.best_move));
      new_book_file.append_line(book_line);
    }
  }
    
  /* remove backup file */
  std::remove((get_filename() + ".bak").c_str());
  
  std::cout << "Successfully cleaned the book." << std::endl;
  
}

book_t::value book_t::lookup(const board* b,int min_depth)
{
  value res(NOT_FOUND,0);
  std::string key = b->to_database_string();
  citer it = data.find(key);
  if(it == data.end() || (it->second.depth < min_depth)){
    return res;
  }
  res = it->second;
  bits64 move_bit = bits64_set[res.best_move];
  int rot = b->to_database_board().get_rotation(b);
  res.best_move = bits64_find_first(bits64_rotate(move_bit,rot));
  return res;
}

void book_t::reload()
{
  data.clear();
  csv_file.get_file()->seekg(std::ios_base::beg);
  load();
}

void book_t::load()
{
  int errors = 0;
  while(true){
    csv::line_t line = csv_file.get_line();
    if(csv_file.get_file()->fail()){
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
  
  csv_file.get_file()->clear();
  
  if(errors > 0){
    std::cout << "Found " << errors;
    std::cout << " incorrect entries in ";
    std::cout << '\"' << get_filename() << '\"' << std::endl;
  }
}
