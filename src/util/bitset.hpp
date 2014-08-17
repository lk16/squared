#pragma once

#include <iostream>

typedef unsigned long long bits64;

extern const int bits_counts[65536];

extern const bits64 bits64_set[65];
extern const bits64 bits64_reset[65];
extern const bits64 bits64_before[65];
extern const bits64 bits64_after[65];

// returns 64 if b==0ul, returns least significant bit otherwise
inline int bits64_find_first(bits64 b){
  if(b == 0ul){
    return 64;
  }
  return __builtin_ffsl(b) - 1;
}

// returns 64 if b==0ull, returns most significant bit otherwise
inline int bits64_find_last(bits64 b){
  if(b == 0ul){
    // __builtin_clz(0ull) is undefined
    return 64;
  }
  return 63 - __builtin_clzl(b);  
}

// returns number of set bits in b
inline int count_64(bits64 b){
#if 0
  return __builtin_popcountll(b);
#else
  return bits_counts[b & 0xFFFF] + 
  bits_counts[(b >> 16) & 0xFFFF] +
  bits_counts[(b >> 32) & 0xFFFF] +
  bits_counts[(b >> 48) & 0xFFFF];
#endif
}



// thanks to http://www-cs-faculty.stanford.edu/~knuth/fasc1a.ps.gz
inline bits64 mirror_vertical_line(bits64 x){
  
  bits64 y;
  
  // reflect x against vertical center line
  y = (x ^ (x >>  7)) & 0x0101010101010101; x ^= y ^ (y <<  7);
  y = (x ^ (x >>  5)) & 0x0202020202020202; x ^= y ^ (y <<  5);
  y = (x ^ (x >>  3)) & 0x0404040404040404; x ^= y ^ (y <<  3);
  y = (x ^ (x >>  1)) & 0x0808080808080808; x ^= y ^ (y <<  1);
  
  
  return x; 
}

// thanks to http://www-cs-faculty.stanford.edu/~knuth/fasc1a.ps.gz
inline bits64 rotate_left(bits64 x){
  
  bits64 y;
  
  // reflect x against diagonal line going through bits 1<<7 and 1<<56
  y = (x ^ (x >> 63)) & 0x0000000000000001; x ^= y ^ (y << 63);
  y = (x ^ (x >> 54)) & 0x0000000000000102; x ^= y ^ (y << 54);
  y = (x ^ (x >> 45)) & 0x0000000000010204; x ^= y ^ (y << 45);
  y = (x ^ (x >> 36)) & 0x0000000001020408; x ^= y ^ (y << 36);
  y = (x ^ (x >> 27)) & 0x0000000102040810; x ^= y ^ (y << 27);
  y = (x ^ (x >> 18)) & 0x0000010204081020; x ^= y ^ (y << 18);
  y = (x ^ (x >>  9)) & 0x0001020408102040; x ^= y ^ (y <<  9);
  return mirror_vertical_line(x);
}

inline bits64 bits64_rotate(bits64 x,int n)
{
  if(n & 4){
    x = mirror_vertical_line(x);
  }
  
  for(int i=0;i<(n & 3);i++){
    x = rotate_left(x);
  }
  
  return x;
}

void bits64_show(bits64 b);