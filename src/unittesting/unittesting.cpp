#include "unittesting.hpp"

void squared_unittest::test_board_validity(const board* b){
  assert((b->me & b->opp) == bits64(0x0));
  bits64 center = (1ull << 27) | (1ull << 28) | (1ull << 35) | (1ull << 36);
  bits64 non_empty = b->me | b->opp;
  assert((non_empty & center) == center);
}

void squared_unittest::announce(const std::string& name)
{
  std::cout << "\nTesting " << name << "";
  std::cout.flush();
}

void squared_unittest::TODO()
{
  std::cout << "\bX";
}

  
void squared_unittest::test_bits64_operator_equals(const bits64* x,const bits64* y){
  bits64 t,s;

  t = *x;
  assert(t == *x);
  assert(t == t);
  assert(*x == *x);
  assert(*y == *y);
  
  t = 3ull;
  assert(t == bits64(3ull));
  
  t = bits64(5ull);
  assert(t == bits64(5ull));
}

void squared_unittest::test_bits64_operator_unequals(const bits64* x,const bits64* y){
  bits64 t = *x;
  t ^= 0x1;
  assert(t != *x);
  assert (((*x != *y) == !(*x == *y)));
}

void squared_unittest::test_bits64_compound_assign_or(const bits64* x,const bits64* y){
  bits64 t,s;
  t = *x;
  s = (t |= *y);
  assert(t == s);
  assert(t == (*x | *y));
}

void squared_unittest::test_bits64_compound_assign_and(const bits64* x,const bits64* y){
  bits64 t,s;
  t = *x;
  s = (t &= *y);
  assert(t == s);
  assert(t == (*x & *y));
}

void squared_unittest::test_bits64_compound_assign_xor(const bits64* x,const bits64* y){
  bits64 t,s;
  t = *x;
  s = (t ^= *y);
  assert(t == s);
  assert(t == (*x ^ *y));
}  
 
void squared_unittest::test_bits64_compound_assign_shift(const bits64* x){
  bits64 t;
  for(int n=0;n<63;++n){
    t = *x;
    t >>= n;
    assert(t == (*x >> n));
    
    t = *x;
    t <<= n;
    assert(t == (*x << n));
  }
}

void squared_unittest::test_bits64_operator_tilde(const bits64* x){
  bits64 t = ~*x;
  assert(bits64(t & *x) == bits64(0ull));
  assert(bits64(*x & ~*x) == bits64(0ull));
}

void squared_unittest::test_bits64_operator_or(const bits64* x,const bits64* y){
  bits64 t = *x | *y;
  assert(bits64(t & bits64(*x | *y)) == t);
  assert(bits64(*x | *x) == *x);
}
  
void squared_unittest::test_bits64_operator_and(const bits64* x,const bits64* y){
  bits64 t = *x & *y;
  assert(bits64(t & *x & *y) == t);
  assert(bits64(*x & *x) == *x);
}

void squared_unittest::test_bits64_operator_xor(const bits64* x,const bits64* y){
  bits64 t = *x ^ *y;
  assert(bits64(bits64(*x | *y) & ~bits64(*x & *y)) == t);
  assert(bits64(*x ^ *x) == bits64(0ull));
}

void squared_unittest::test_bits64_operator_bool(const bits64* x){
  assert(((bool)(*x)) == (*x != bits64(0ull)));
}

void squared_unittest::test_bits64_reset_all(const bits64* x){
  bits64 t = *x;
  t.reset_all();
  assert(t == bits64(0ull));
}

void squared_unittest::test_bits64_reset_before(const bits64* x){
  bits64 t,s;
  s = *x;
  for(int i=0; i<64; ++i){
    t = *x;
    t.reset_before(i);
    s &= ~(1ull << i);
    assert(t == s);
  }
  
  t = *x;
  t.reset_before(64);
  assert(t == bits64(0ull));
}

void squared_unittest::test_bits64_reset_after(const bits64* x){
  bits64 s,t;
  s = *x;
  for(int i=63; i>=0; --i){
    t = *x;
    t.reset_after(i);
    s &= ~(1ull << i);
    assert(t == s);
  }
  
  t = *x;
  t.reset_after(64);
  assert(t == bits64(0ull));
}  

void squared_unittest::test_bits64_reset(const bits64* x){
  bits64 t,s;
  for(int i=0; i<64; ++i){
    t = *x;
    s = *x & bits64(~(1ul << i));
    assert(t.reset(i) == s);
    assert(t == s);
  }
}

  
void squared_unittest::test_bits64_set(const bits64* x){
  bits64 t; 
  for(int i=0; i<63; ++i){
    t = *x;
    t.set(i);
    assert(t == (*x | bits64(1ull << i)));
  }
}

void squared_unittest::test_bits64_index_bit(const bits64* x){
  // first_index last_index first_bit last_bit
  bits64 t = *x;
  {
    int index = 0;
    while(index<64 && !t.test(index)){
      ++index;
    }
    assert(t.first_index() == index);
    assert(t.first_bit() == bits64(index==64 ? 0ull : (1ull << index)));
    
    
    index = 63;
    while(index>=0 && !t.test(index)){
      --index;
    }
    if(index == -1){
      index = 64;
    }
    assert(t.last_index() == index);
    assert(t.last_bit() == bits64(index==64 ? 0ull : (1ull << index)));
  }
}

void squared_unittest::test_bits64_counting(const bits64* x){
  // count any none all test
  int count = 0;
  bits64 t = *x;
  
  for(int i=0;i<64;++i){
    bits64 mask(1ull << i);
    if(t.test(i)){
      ++count;
    }
    assert(t.test(i) == ((t & mask) == mask));
  }
  assert(t.none() == (count==0));
  assert(t.any() == (count!=0));
  assert(t.all() == (count==64));
  
}

void squared_unittest::test_bits64_rotate(const bits64* x){
  // 0: unmodified
  // 1: left 
  // 2: right 
  // 3: 180 
  // 4: mirror vertically 
  // 5: mirror vertically + right
  // 6: mirror vertically + 180
  // 7: mirror vertically + left
  bits64 rotated[8];
  for(int i=0;i<8;++i){
    bits64 b = *x;
    rotated[i] = b.rotate(i);
  }
  
  assert(*x == rotated[0]);
  
  for(int py=0;py<8;++py){
    for(int px=0;px<8;++px){
      int p = 8*py + px;
      assert(x->test(p) == rotated[0].test(p));
      assert(x->test(p) == rotated[1].test(8*(7-px)+py));
      assert(x->test(p) == rotated[2].test(8*px+7-py));
      assert(x->test(p) == rotated[3].test(63-p));
      assert(x->test(p) == rotated[4].test(8*py+7-px));
      assert(x->test(p) == rotated[5].test(8*(7-px)+7-py));
      assert(x->test(p) == rotated[6].test(8*(7-py)+px));
      assert(x->test(p) == rotated[7].test(8*px+py));
    }
  }
}
  
void squared_unittest::test_bits64_is_subset_of_mask(const bits64* x,const bits64* y){
  if((*x & *y) == *x){
    assert(x->is_subset_of_mask(*y) == bits64(~0ull));
  }
  else{
    assert(x->is_subset_of_mask(*y) == bits64(0ull));
  }
}


void squared_unittest::test_bits64_default_ctor(){
  assert(bits64() == bits64(0ull));
}

void squared_unittest::test_bits64_static_const_members(){
  for(unsigned i=0;i<64;++i){
    assert(bits64::mask_set[i] == 1ull << i);
    assert(bits64::mask_reset[i] == ~(1ull << i));
  }
  assert(bits64::mask_set[64] == 0ull);
  assert(bits64::mask_reset[64] == ~0ull);
}


void squared_unittest::test_bits64_test_all(){
  bits64 t;
  t.set_all();
  assert(t == bits64(~0ull));
}
  
void squared_unittest::test_bits64_only_bit_index(){
  for(int i=0;i<64;++i){
    assert(bits64(1ull << i).only_bit_index() == i);
  }
  assert(bits64(0ull).only_bit_index() == 64);
}  

void squared_unittest::test_board_copy_ctor(const board* x){
  board b(*x);
  assert(b == *x);
}

void squared_unittest::test_board_ctor_string(const board* x){
  (void)x;
  TODO();
}

void squared_unittest::test_board_operator_assign(const board* x){
  board b;
  assert(b == (b = *x));
  assert(b == *x);
}

void squared_unittest::test_board_operator_equals(const board* x,const board* y){
  board b;
  assert(b == b);
  assert(*x == *x);
  assert(*y == *y);
  assert((*y == *x) == (y->me==x->me && y->opp==x->opp));
}

void squared_unittest::test_board_operator_unequals(const board* x,const board* y){
  assert((*x == *y) == !(*x != *y));
}
  
void squared_unittest::test_board_operator_less(const board* x,const board* y){
  board z;
  z.reset();
  z.do_random_moves(3);
  assert(!(z < z));
  assert(!(*x<z && z<*x));
  if(*x < z && z < *y){
    assert(*x < *y);
  }
  if(z < *x && *x < *y){
    assert(z < *y);
  }
  if(z != *x){
    assert((z < *x) || (*x < z));
  }
}

void squared_unittest::test_board_get_children(const board* x)
{
  board children[32];
  board* child_end = x->get_children(children);
  
  board same_children[32];
  board* same_children_end = same_children;
  for(int i=0;i<64;++i){
    if(x->is_valid_move(i)){
      *same_children_end = *x;
      same_children_end->do_move(i);
      ++same_children_end;
    }
  }
  
  assert(std::set<board>(children,child_end) ==
    std::set<board>(same_children,same_children_end));
  
}

void squared_unittest::test_board_get_children_with_moves(const board* x)
{
  bits64 valid_moves = x->get_valid_moves();
  board with[32],without[32];
  x->get_children(without);
  x->get_children(with,valid_moves);
  
  for(int i=0;i<valid_moves.count();++i){
    assert(with[i] == without[i]);
  }

}




void squared_unittest::test_board_switch_turn(const board* x){
  board b = *x;
  b.switch_turn();
  assert(b.me == x->opp);
  assert(b.opp == x->me);
}

void squared_unittest::test_board_is_valid_move(const board* x){
  board b = *x;
  for(int i=0;i<64;++i){
    assert(b.is_valid_move(i) == b.get_valid_moves().test(i));
  }
}

void squared_unittest::test_board_get_valid_moves(const board* x){
  bits64 found_moves = 0ull;
  for(int py=0;py<8;++py){
    for(int px=0;px<8;++px){
      int p = 8*py + px;
      if(x->get_non_empty_fields().test(p)){
        continue;
      }
      for(int dx=-1;dx<=1;++dx){
        for(int dy=-1;dy<=1;++dy){
          if(dx==0 && dy==0){
            continue;
          }
          int dist = 1;
          while(true){
            int qy = py + (dist*dy);
            int qx = px + (dist*dx);
            int q = 8*qy + qx;
            if(qy<0 || qy>=8 || qx<0 || qx>=8){
              break;
            }
            if(x->me.test(q)){
              if(dist >= 2){
                found_moves.set(p);
              }
              break;
            }
            else if(x->opp.test(q)){
              ++dist;
            }
            else{
              break;
            }
          }
        }
      }
    }
  }
  assert(x->get_valid_moves() == found_moves);
}


void squared_unittest::test_board_do_random_moves(const board* x){
  board b = *x;
  bits64 non_empty = b.get_non_empty_fields();
  b.do_random_moves(rand() % 15);
  test_board_validity(&b);
  assert((b.get_non_empty_fields() & non_empty) == non_empty);
}

void squared_unittest::test_board_reset(){
  board b;
  b.reset();
  test_board_validity(&b);
  assert(b.me == bits64((1ull << 28) | (1ull << 35)));
  assert(b.opp == bits64((1ull << 27) | (1ull << 36)));
}

void squared_unittest::test_board_count_discs(const board* x)
{
  assert(x->count_discs() == (x->me | x->opp).count());
}

void squared_unittest::test_board_count_empty_fields(const board* x)
{
  assert(x->count_empty_fields() == (~(x->me | x->opp)).count());
}

void squared_unittest::test_board_count_opponent_moves(const board* x)
{
  board b = *x;
  assert(x->count_opponent_moves() == b.switch_turn()->count_valid_moves());
}

void squared_unittest::test_board_count_valid_moves(const board* x)
{
  assert(x->count_valid_moves() == x->get_valid_moves().count());
}

void squared_unittest::test_board_do_move(const board* x)
{
  bits64 valid_moves = x->get_valid_moves();
  for(int i=0;i<64;++i){
    if(valid_moves.test(i)){
      board t = *x;
      bits64 t_undo = t.do_move(i);
      assert(t_undo == (x->me ^ t.opp).reset(i));
    }
  }
}

void squared_unittest::test_board_get_disc_diff(const board* x)
{
  int me = x->me.count();
  int opp = x->opp.count();
  int disc_diff = x->get_disc_diff();
  if(me < opp){
    assert(disc_diff == -64 + 2*me);
  }
  else if(opp < me){
    assert(disc_diff == 64 - 2*opp);
  }
  else{
    assert(disc_diff == 0);
  }
}

void squared_unittest::test_board_get_empty_fields(const board* x)
{
  assert(x->get_empty_fields() == ~(x->me | x->opp));
}

void squared_unittest::test_board_get_mobility(const board* x)
{
  int total = 0;
  for(int i=0;i<64;++i){
    if(x->is_valid_move(i)){
      board b = *x;
      total += b.do_move(i).count();
    }
  }
  assert(total == x->get_mobility(x->get_valid_moves()));
}

void squared_unittest::test_board_get_move_index(const board* x)
{
  bits64 valid_moves = x->get_valid_moves();
  for(int i=0;i<64;++i){
    if(valid_moves.test(i)){
      board b = *x;
      b.do_move(i);
      assert(x->get_move_index(&b) == i);
    }
  }
}

void squared_unittest::test_board_hash(const board* x, const board* y)
{
  size_t x_hash = board_hash()(*x);
  size_t y_hash = board_hash()(*y);
  if(*x == *y){
    assert(x_hash == y_hash);
  }
  else if(x_hash == y_hash){
    std::cout << "\bH";
  }
}

void squared_unittest::test_board_get_non_empty_fields(const board* x)
{
  assert(x->get_non_empty_fields() == (x->me | x->opp));
}

void squared_unittest::test_board_has_valid_moves(const board* x)
{
  assert(x->has_valid_moves() == x->get_valid_moves().any());
}

void squared_unittest::test_board_get_rotation(const board* x)
{
  assert(x->get_rotation(x) == 0);
  
  for(int i=0;i<8;i++){
    board b = *x;
    assert(board(*x).rotate(x->get_rotation(&b)) == *x); 
  }
  
  board b = *x;
  b.me ^= 0x1;
  b.opp ^= 0x1;
  assert(b.get_rotation(x) == -1);      
  
}

void squared_unittest::test_board_opponent_has_moves(const board* x)
{
  board b = *x;
  assert(b.switch_turn()->has_valid_moves() == x->has_valid_moves());
}

void squared_unittest::test_board_position_to_index(const board* x)
{
  (void)x;
  TODO();
}

void squared_unittest::test_bits64_get_word(const bits64* x)
{
  // HACKY
  assert(x->get_word() == *reinterpret_cast<const uint64_t*>(x));
}


void squared_unittest::test_board_rotate(const board* x)
{
  for(int i=0;i<8;++i){
    board rot = x->rotate(i);
    assert(rot.me == x->me.rotate(i));
    assert(rot.opp == x->opp.rotate(i));
  }
}

void squared_unittest::test_board_to_database_board(const board* x)
{
  (void)x;
  TODO();
}

void squared_unittest::test_board_to_database_string(const board* x)
{
  (void)x;
  TODO();
}

void squared_unittest::test_board_to_string(const board* x)
{
  (void)x;
  TODO();
}

void squared_unittest::test_board_undo_move(const board* x)
{
  (void)x;
  TODO();
}

void squared_unittest::test_board_xot()
{
  TODO();
}












void squared_unittest::test_bits64_all()
{  
  std::vector<std::pair<void(*)(),std::string>> noarg_funcs;
  add_to_fun_vec(noarg_funcs,test_bits64_default_ctor);
  add_to_fun_vec(noarg_funcs,test_bits64_static_const_members);
  add_to_fun_vec(noarg_funcs,test_bits64_test_all);
  add_to_fun_vec(noarg_funcs,test_bits64_only_bit_index);
  
  for(auto f: noarg_funcs){
    progress_bar pb(1,f.second);
    pb.step();
    (f.first)();
  }

  std::vector<bits64> test_input;
  test_input.push_back(bits64(0ull));
  test_input.push_back(bits64(~0ull));
  test_input.push_back(bits64(1ull));
  test_input.push_back(bits64(1ull << 63));
  test_input.push_back(bits64(0x3333333333333333));
  test_input.push_back(bits64(0xcccccccccccccccc));
  while(test_input.size() < N){
    test_input.push_back(bits64(rand_64()));
  }

  
  
  std::vector<std::pair<void(*)(const bits64*),std::string>> unary_funcs;
  add_to_fun_vec(unary_funcs,test_bits64_compound_assign_shift);
  add_to_fun_vec(unary_funcs,test_bits64_operator_tilde);
  add_to_fun_vec(unary_funcs,test_bits64_operator_bool);
  add_to_fun_vec(unary_funcs,test_bits64_reset_all);
  add_to_fun_vec(unary_funcs,test_bits64_reset_before);
  add_to_fun_vec(unary_funcs,test_bits64_reset_after);
  add_to_fun_vec(unary_funcs,test_bits64_reset);
  add_to_fun_vec(unary_funcs,test_bits64_set);
  add_to_fun_vec(unary_funcs,test_bits64_index_bit);
  add_to_fun_vec(unary_funcs,test_bits64_counting);
  add_to_fun_vec(unary_funcs,test_bits64_rotate);
  add_to_fun_vec(unary_funcs,test_bits64_get_word);

  
  for(auto f: unary_funcs){
    progress_bar pb(N,f.second);
    for(const auto& arg: test_input){
      pb.step();
      (f.first)(&arg);
    }
  }
  
  

  
  std::vector<std::pair<void(*)(const bits64*,const bits64*),std::string>> binary_funcs;
  add_to_fun_vec(binary_funcs,test_bits64_operator_equals);
  add_to_fun_vec(binary_funcs,test_bits64_operator_unequals);
  add_to_fun_vec(binary_funcs,test_bits64_compound_assign_or);
  add_to_fun_vec(binary_funcs,test_bits64_compound_assign_and);
  add_to_fun_vec(binary_funcs,test_bits64_compound_assign_xor);
  add_to_fun_vec(binary_funcs,test_bits64_operator_or);
  add_to_fun_vec(binary_funcs,test_bits64_operator_and);
  add_to_fun_vec(binary_funcs,test_bits64_operator_xor);
  add_to_fun_vec(binary_funcs,test_bits64_is_subset_of_mask);
  
  for(auto f: binary_funcs){
    progress_bar pb(N*N,f.second);
    for(const auto& lhs: test_input){
      for(const auto& rhs: test_input){
        pb.step();
        (f.first)(&lhs,&rhs);
      }
    }
  }
}  



    
void squared_unittest::test_board_all()
{
  
  std::vector<std::pair<void(*)(),std::string>> noarg_funcs;
  add_to_fun_vec(noarg_funcs,test_board_reset);
  
   for(auto f: noarg_funcs){
    progress_bar pb(1,f.second);
    pb.step();
    (f.first)();
  }

  
  
  
  std::vector<board> test_input;
  test_input.push_back(board());
  test_input.back().reset();
  test_input.push_back(board());
  test_input.back().me = 0ull;
  test_input.back().opp = ~0ull;
  test_input.push_back(board(test_input.back()));
  test_input.back().switch_turn();
  
  while(test_input.size() < N){
    test_input.push_back(board());
    test_input.back().reset();
    test_input.back().do_random_moves(rand() % 60);
  }

  
  
  std::vector<std::pair<void(*)(const board*),std::string>> unary_funcs;
  add_to_fun_vec(unary_funcs,test_board_copy_ctor); 
  add_to_fun_vec(unary_funcs,test_board_ctor_string);
  add_to_fun_vec(unary_funcs,test_board_switch_turn);
  add_to_fun_vec(unary_funcs,test_board_is_valid_move);
  add_to_fun_vec(unary_funcs,test_board_get_valid_moves);
  add_to_fun_vec(unary_funcs,test_board_do_random_moves);
  add_to_fun_vec(unary_funcs,test_board_operator_assign);
  add_to_fun_vec(unary_funcs,test_board_get_children);
  add_to_fun_vec(unary_funcs,test_board_get_children_with_moves);
  add_to_fun_vec(unary_funcs,test_board_has_valid_moves);
  add_to_fun_vec(unary_funcs,test_board_count_valid_moves);
  add_to_fun_vec(unary_funcs,test_board_opponent_has_moves);
  add_to_fun_vec(unary_funcs,test_board_count_opponent_moves);
  add_to_fun_vec(unary_funcs,test_board_get_empty_fields);
  add_to_fun_vec(unary_funcs,test_board_get_non_empty_fields);
  add_to_fun_vec(unary_funcs,test_board_count_discs);
  add_to_fun_vec(unary_funcs,test_board_count_empty_fields);
  add_to_fun_vec(unary_funcs,test_board_get_disc_diff);
  add_to_fun_vec(unary_funcs,test_board_do_move);
  add_to_fun_vec(unary_funcs,test_board_position_to_index);
  add_to_fun_vec(unary_funcs,test_board_undo_move);
  add_to_fun_vec(unary_funcs,test_board_to_string);
  add_to_fun_vec(unary_funcs,test_board_to_database_string);
  add_to_fun_vec(unary_funcs,test_board_get_move_index);
  add_to_fun_vec(unary_funcs,test_board_to_database_board);
  add_to_fun_vec(unary_funcs,test_board_rotate);
  add_to_fun_vec(unary_funcs,test_board_get_rotation);
  add_to_fun_vec(unary_funcs,test_board_get_mobility);
  add_to_fun_vec(unary_funcs,test_board_validity);
  
  for(auto f: unary_funcs){
    progress_bar pb(N,f.second);
    for(const auto& arg: test_input){
      pb.step();
      (f.first)(&arg);
    }
  }
  
  

  
  std::vector<std::pair<void(*)(const board*,const board*),std::string>> binary_funcs;
  add_to_fun_vec(binary_funcs,test_board_operator_equals);
  add_to_fun_vec(binary_funcs,test_board_operator_unequals);
  add_to_fun_vec(binary_funcs,test_board_operator_less);
  add_to_fun_vec(binary_funcs,test_board_hash);
 
  for(auto f: binary_funcs){
    progress_bar pb(N*N,f.second);
    for(const auto& lhs: test_input){
      for(const auto& rhs: test_input){
        pb.step();
        (f.first)(&lhs,&rhs);
      }
    }
  }
}

  

void squared_unittest::run()
{
  test_bits64_all();
  test_board_all();
  std::cout << '\n';
}
