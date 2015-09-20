#include "unittesting.hpp"

void check_board(const board* b){
  assert((b->me & b->opp) == bits64(0x0));
  bits64 center = (1ull << 27) | (1ull << 28) | (1ull << 35) | (1ull << 36);
  bits64 non_empty = b->me | b->opp;
  assert((non_empty & center) == center);
}

void run_bits64_tests(const bits64* x,const bits64* y){
  bits64 t,s;
  
  t = *x;
  assert(t == *x);
  
  t = *x;
  t ^= 0x1;
  assert(t != *x);
  
  assert(t == t);
  assert(*x == *x);
  assert(*y == *y);
  
  t = 3ull;
  assert(t == bits64(3ull));
  
  t = bits64(5ull);
  assert(t == bits64(5ull));
  
  t = *x;
  s = (t |= *y);
  assert(t == s);
  assert(t == (*x | *y));
  
  
  t = *x;
  s = (t &= *y);
  assert(t == s);
  assert(t == (*x & *y));
  
  t = *x;
  s = (t ^= *y);
  assert(t == s);
  assert(t == (*x ^ *y));
  
  
  for(int n=0;n<63;++n){
    t = *x;
    t >>= n;
    assert(t == (*x >> n));
    
    t = *x;
    t <<= n;
    assert(t == (*x << n));
  }
  
  t = ~*x;
  assert(bits64(t & *x) == bits64(0ull));

  assert(bits64(*x & ~*x) == bits64(0ull));
  
  t = *x | *y;
  assert(bits64(t & bits64(*x | *y)) == t);
  
  assert(bits64(*x | *x) == *x);
  
  t = *x & *y;
  assert(bits64(t & *x & *y) == t);
  
  assert(bits64(*x & *x) == *x);
  
  t = *x ^ *y;
  assert(bits64(bits64(*x | *y) & ~bits64(*x & *y)) == t);
  
  assert(bits64(*x ^ *x) == bits64(0ull));
}


void squared_unittesting()
{   
  const int n_tests = 10000;
  
  for(unsigned i=0;i<64;++i){
    assert(bits64::mask_set[i] == 1ull << i);
    assert(bits64::mask_reset[i] == ~(1ull << i));
  }
  assert(bits64::mask_set[64] == 0ull);
  assert(bits64::mask_reset[64] == ~0ull);
  
  
  for(int i=0; i<n_tests; ++i){
    bits64 x(rand_64());
    bits64 y(rand_64());
    run_bits64_tests(&x,&y);
    run_bits64_tests(&y,&x);
  }
  
  
  std::cout << "OK\n";
}
