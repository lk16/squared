#pragma once

typedef unsigned long long bits;

extern const int bits_counts[65536];

// returns -1 if ul==0ul, returns least significant bit otherwise
inline int find_first_set_64(bits b){
  return __builtin_ffsl(b) - 1;
}


inline int find_last_set_64(bits b){
  if(b == 0ul){
    // __builtin_clz(0ul) is undefined
    return -1;
  }
  return 63 - __builtin_clzl(b);  
}

inline int count_64(bits b){
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
inline bits mirror_vertical_line(const bits b){
  
  bits x=b,y;
  
  // reflect x against vertical center line
  y = (x ^ (x >>  7)) & 0x0101010101010101; x ^= y ^ (y <<  7);
  y = (x ^ (x >>  5)) & 0x0202020202020202; x ^= y ^ (y <<  5);
  y = (x ^ (x >>  3)) & 0x0404040404040404; x ^= y ^ (y <<  3);
  y = (x ^ (x >>  1)) & 0x0808080808080808; x ^= y ^ (y <<  1);
  
  
  return x; 
}

// thanks to http://www-cs-faculty.stanford.edu/~knuth/fasc1a.ps.gz
inline bits rotate_left(const bits b){
  
  bits x=b,y;
  
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