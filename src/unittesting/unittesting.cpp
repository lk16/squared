#include "unittesting.hpp"

void board_check(const board* b){
  assert((b->me & b->opp) == bits64(0x0));
  bits64 center = (1ull << 27) | (1ull << 28) | (1ull << 35) | (1ull << 36);
  bits64 non_empty = b->me | b->opp;
  assert((non_empty & center) == center);
}

void squared_unittest::announce(const std::string& name)
{
  std::cout << "\nTesting " << name << " ";
  std::cout.flush();
}

void squared_unittest::announce_step()
{
  std::cout << '.';
  std::cout.flush();
}

void run_bits64_tests(const bits64* x,const bits64* y){
  bits64 t,s;
  
  // operator==
  t = *x;
  assert(t == *x);
  assert(t == t);
  assert(*x == *x);
  assert(*y == *y);
  
  t = 3ull;
  assert(t == bits64(3ull));
  
  t = bits64(5ull);
  assert(t == bits64(5ull));
  
  // operator!=
  t = *x;
  t ^= 0x1;
  assert(t != *x);
   
  // operator|=
  t = *x;
  s = (t |= *y);
  assert(t == s);
  assert(t == (*x | *y));
  
  // operator&=
  t = *x;
  s = (t &= *y);
  assert(t == s);
  assert(t == (*x & *y));
  
  // operator^=
  t = *x;
  s = (t ^= *y);
  assert(t == s);
  assert(t == (*x ^ *y));
  
  // operator<<= operator>>=
  for(int n=0;n<63;++n){
    t = *x;
    t >>= n;
    assert(t == (*x >> n));
    
    t = *x;
    t <<= n;
    assert(t == (*x << n));
  }
  
  // operator~
  t = ~*x;
  assert(bits64(t & *x) == bits64(0ull));

  assert(bits64(*x & ~*x) == bits64(0ull));
  
  // operator|
  t = *x | *y;
  assert(bits64(t & bits64(*x | *y)) == t);
  
  assert(bits64(*x | *x) == *x);
  
  // operator&
  t = *x & *y;
  assert(bits64(t & *x & *y) == t);
  
  assert(bits64(*x & *x) == *x);
  
  // operator^
  t = *x ^ *y;
  assert(bits64(bits64(*x | *y) & ~bits64(*x & *y)) == t);
  
  assert(bits64(*x ^ *x) == bits64(0ull));
  
  // operator bool
  t = bits64(1ull);
  assert(t); 
  
  t = bits64(0ull);
  assert(!t); 
  
  // reset_all  
  t = *x;
  t.reset_all();
  assert(t == bits64(0ull));
  
  // reset_before
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
  
  // reset_after
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
  
  // reset
  for(int i=0; i<64; ++i){
    t = *x;
    s = *x & bits64(~(1ul << i));
    assert(t.reset(i) == s);
    assert(t == s);
  }
  
  // set_all
  t = *x;
  t.set_all();
  assert(t == bits64(~0ull));
  
  // set
  for(int i=0; i<63; ++i){
    t = *x;
    t.set(i);
    assert(t == (*x | bits64(1ull << i)));
  }
  
  // first_index last_index first_bit last_bit
  t = *x;
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
  
  // count any none all test
  {
    int count = 0;
    t = *x;
    
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
  
  // mirror_veritcal_line
  t = x->mirror_vertical_line();
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
  
  // TODO rotate
  
  // is_subset_of_mask
  if((*x & *y) == *x){
    assert(x->is_subset_of_mask(*y) == bits64(~0ull));
  }
  else{
    assert(x->is_subset_of_mask(*y) == bits64(0ull));
  }
}

void board_test(const board* x,const board* y){
  board_check(x);
  board_check(y);
  
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
    b = *x;
    bits64 found_moves = 0ull;
    for(int py=0;py<8;++py){
      for(int px=0;px<8;++px){
        int p = 8*py + px;
        if(b.get_non_empty_fields().test(p)){
          continue;
        }
        for(int dx=-1;dx<=1;++dx){
          for(int dy=-1;dy<=1;++dy){
            if(dx==0 && dy==0){
              continue;
            }
            int dist = 1;
            // TODO code here
            
          }
        }
      }
    }
    assert(b.get_valid_moves() == found_moves);
  }
  // do_random_moves
  {
    b = *x;
    bits64 non_empty = b.get_non_empty_fields();
    b.do_random_moves(rand() % 15);
    assert((b.get_non_empty_fields() & non_empty) == non_empty);
  }
}

void squared_unittesting()
{
  squared_unittest().run();
}


void squared_unittest::run()
{
  const int n_tests = 10;
  
  { // bits64
  
    announce("bits64 default ctor");
    assert(bits64() == bits64(0ull));
    
    announce("bits64 static const members");
    for(unsigned i=0;i<64;++i){
      assert(bits64::mask_set[i] == 1ull << i);
      assert(bits64::mask_reset[i] == ~(1ull << i));
    }
    assert(bits64::mask_set[64] == 0ull);
    assert(bits64::mask_reset[64] == ~0ull);
    
    announce("bits64 with random data");
    for(int i=0; i<n_tests; ++i){
      bits64 x(rand_64());
      bits64 y(rand_64());
      run_bits64_tests(&x,&y);
      announce_step();
      run_bits64_tests(&y,&x);
      announce_step();
    }
    
    announce("bits64 special cases");
    std::vector<bits64> v;
    v.push_back(bits64(0ull));
    v.push_back(bits64(~0ull));
    v.push_back(bits64(1ull));
    v.push_back(bits64(1ull << 63));
    v.push_back(bits64(0x3333333333333333));
    v.push_back(bits64(0xcccccccccccccccc));

    const bits64 *pi,*pj;
    for(pi=v.data();pi!=v.data()+v.size();++pi){
      for(pj=v.data();pj!=v.data()+v.size();++pj){
        run_bits64_tests(pi,pj);
        announce_step();
      }
    }
    
    announce("bits64 only bit index");
    // only_bit_index
    for(int i=0;i<64;++i){
      assert(bits64(1ull << i).only_bit_index() == i);
    }
    assert(bits64(0ull).only_bit_index() == 64);
    
    // TODO to_ascii
    
  }
  
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
        board_test(i,j);
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
  
  std::cout << "\nOK\n";
}
