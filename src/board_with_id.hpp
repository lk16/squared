#include "board.hpp"

struct board_with_id
{
  const board* b;
  int id;
  
  board_with_id();
  board_with_id(const board* b,int id);
  board_with_id(const board_with_id& bwi);
  board_with_id& operator=(const board_with_id& rhs);
  bool operator<(const board_with_id& rhs);
};

inline board_with_id::board_with_id()
{
  b = NULL;
  id = 0;
}



inline board_with_id::board_with_id(const board* b,int id):
  b(b),
  id(id)
{
}

inline board_with_id::board_with_id(const board_with_id& bwi):
  b(new board(*bwi.b)),
  id(bwi.id)
{
}

inline board_with_id& board_with_id::operator=(const board_with_id& bwi)
{
  b = bwi.b;
  id = bwi.id;
  return *this;
}

inline bool board_with_id::operator<(const board_with_id& rhs)
{
  return b->heuristic < rhs.b->heuristic;
}
