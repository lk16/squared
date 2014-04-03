#include "main/learn.hpp"

book_t read_book()
{
  book_t res;
  csv bookfile(BOOK_PATH + "book.csv");
  if(bookfile.get_file()->fail()){
    // cannot read or open file (it might not exist)
    // return empty book
    return res;
  }
  while(bookfile.get_file()->good()){
    csv::line_t line = bookfile.get_line();
    book_value bv;
    bv.depth = fromstr<int>(line[1]);
    bv.best_move = fromstr<int>(line[2]);
    res[line[0]] = bv;
  }
  return res;
}


void learn_book()
{
  const int min_learn_level = 5;
  std::cout << "learning book\n";
  
  int learn_level = min_learn_level;
  book_t book = read_book();
  while(true){
    bot_ali bot(COLOR_UNDEFINED,learn_level,learn_level);
    csv movesfile(BOOK_PATH + "moves.csv");
    // read each line
    // if not in book, calculate heur for learn_level
    
    learn_level++;
  }
}
