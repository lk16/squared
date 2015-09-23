#include "unittesting.hpp"

void squared_unittest::check_board(const board* b){
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

void squared_unittest::announce_step()
{
  std::cout << '.';
  std::cout.flush();
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
  
void squared_unittest::test_bits64_vertical_line(const bits64* x){
  // mirror_veritcal_line
  bits64 t = x->mirror_vertical_line();
  for(int ty=0;ty<8;++ty){
    for(int tx=0;tx<8;++tx){
      assert(x->test(8*ty+tx) == t.test(8*ty+(7-tx)));
    }
  }
  
  
  // rotate_left
  t = x->rotate_left();
  for(int ty=0;ty<8;++ty){
    for(int tx=0;tx<8;++tx){
      assert(x->test(8*ty+tx) == t.test(8*(7-tx)+ty));
    }
  }
}

void squared_unittest::test_bits64_rotate(const bits64* x){
  (void)x;
  std::cout << "x";
}
  
void squared_unittest::test_bits64_is_subset_of_mask(const bits64* x,const bits64* y){
  if((*x & *y) == *x){
    assert(x->is_subset_of_mask(*y) == bits64(~0ull));
  }
  else{
    assert(x->is_subset_of_mask(*y) == bits64(0ull));
  }
}

void squared_unittest::test_bits64_to_ascii(const bits64* x){
  (void)x;
  std::cout << "x";
}


#if 0
void board_test(const board* x,const board* y){
  check_board(x);
  check_board(y);
  
  // copy ctor
  board b(*x);
  assert(b == *x);
  
  //TODO ctor from string
  
  // operator=
  assert(b == (b = *y));
  assert(b == *y);
  // operator==
  assert(b == b);
  assert(*x == *x);
  assert(*y == *y);
  assert((*y == *x) == (y->me==x->me && y->opp==x->opp));
  
  // operator!=
  assert((*x == *y) == !(*x != *y));
  
  // operator<
  {
    const int pn = 5;
    board p[pn];
    for(int i=0;i<pn;++i){
      p[i].reset();
      p[i].do_random_moves(9);
    }
    p[pn-2] = *x;
    p[pn-1] = *y;
    for(int i=0;i<pn;++i){
      assert(!(p[i] < p[i]));
      for(int j=0;j<pn;++j){
        assert(!(p[i]<p[j] && p[j]<p[i]));
        assert(p[i]<p[j] || p[i]==p[j] || p[j]<p[i]);
        for(int k=0;k<pn;++k){
          if(p[i] < p[j] && p[j] < p[k]){
            assert(p[i] < p[k]);
          }
        }
      }
    }
  }
  
  // switch
  b = *x;
  b.switch_turn();
  assert(b.me == x->opp);
  assert(b.opp == x->me);
  
  // is_valid_move
  b = *x;
  for(int i=0;i<64;++i){
    assert(b.is_valid_move(i) == b.get_valid_moves().test(i));
  }
  
  // TODO get_valid_moves
  {
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
    if(x->get_valid_moves() != found_moves){
      x->get_valid_moves().show();
      found_moves.show();
      std::cout << x->to_ascii_art(0) << '\n';
      exit(0);
    }
  }
  // do_random_moves
  {
    b = *x;
    bits64 non_empty = b.get_non_empty_fields();
    b.do_random_moves(rand() % 15);
    assert((b.get_non_empty_fields() & non_empty) == non_empty);
  }
}
#endif

void squared_unittesting()
{
  squared_unittest().run();
}


void squared_unittest::run()
{
  const int N = 10;
  
  { // bits64
  
    announce("bits64 default ctor");
    assert(bits64() == bits64(0ull));
    
    announce("bits64 public static const members");
    for(unsigned i=0;i<64;++i){
      assert(bits64::mask_set[i] == 1ull << i);
      assert(bits64::mask_reset[i] == ~(1ull << i));
    }
    assert(bits64::mask_set[64] == 0ull);
    assert(bits64::mask_reset[64] == ~0ull);
    
    announce("bits64::test_all()");
    {
      bits64 t;
      t.set_all();
      assert(t == bits64(~0ull));
    }
    

    announce("bits64::only_bit_index()");
    // only_bit_index
    for(int i=0;i<64;++i){
      assert(bits64(1ull << i).only_bit_index() == i);
    }
    assert(bits64(0ull).only_bit_index() == 64);
    
    
    
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
      std::cout << "\nRunning " << f.second << " ";
      for(const auto& lhs: test_input){
        for(const auto& rhs: test_input){
          (f.first)(&lhs,&rhs);
          std::cout << ".";
        }
      }
    }
 
    std::vector<std::pair<void(*)(const bits64*),std::string>> unary_funcs;
    add_to_fun_vec(unary_funcs,test_bits64_to_ascii);
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
    add_to_fun_vec(unary_funcs,test_bits64_vertical_line);
    add_to_fun_vec(unary_funcs,test_bits64_rotate);
 
    
    for(auto f: unary_funcs){
      std::cout << "\nRunning " << f.second << " ";
      for(const auto& arg: test_input){
        (f.first)(&arg);
        std::cout << ".";
      }
    }
    
    
  }
  
#if 0
  { // board
    std::vector<board> board_vec;
    for(int i=0;i<6;++i){
      board_vec.push_back(board());
      board_vec.back().reset();
      board_vec.back().do_random_moves(i);
    }
    

    
    const board *i,*j;
    const board *start = board_vec.data();
    const board *end = start + board_vec.size();
    announce("board random moves");
    for(i=start;i!=end;++i){
      for(j=start;j!=end;++j){
        test_board(i,j);
        announce_step();
        board_test(j,i);
        announce_step();
      }
    }
    
    {
      announce("board reset");
      board b;
      b.reset();
      assert(b.me == bits64((1ull << 28) | (1ull << 35)));
      assert(b.opp == bits64((1ull << 27) | (1ull << 36)));
    }

    // TODO xot
  }
#endif 
  std::cout << "\nOK\n";
}
