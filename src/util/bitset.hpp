#pragma once

#include <bitset>

// returns -1 if ul==0ul, returns least significant bit otherwise
inline int find_first_set_64(unsigned long ul){
  return __builtin_ffsl(ul) - 1;
}


// thanks to http://www-cs-faculty.stanford.edu/~knuth/fasc1a.ps.gz
inline std::bitset<64> mirror_vertical_line(const std::bitset<64>& b){
  unsigned long long x=b.to_ulong(),y;
  
    // reflect x against vertical center line
  y = (x ^ (x >>  7)) & 0x0101010101010101; x ^= y ^ (y <<  7);
  y = (x ^ (x >>  5)) & 0x0202020202020202; x ^= y ^ (y <<  5);
  y = (x ^ (x >>  3)) & 0x0404040404040404; x ^= y ^ (y <<  3);
  y = (x ^ (x >>  1)) & 0x0808080808080808; x ^= y ^ (y <<  1);
  
  
  return std::bitset<64>(x); 
}

// thanks to http://www-cs-faculty.stanford.edu/~knuth/fasc1a.ps.gz
inline std::bitset<64> rotate_left(std::bitset<64>& b) {
  unsigned long long x=b.to_ulong(),y;
  
 
  // reflect x against diagonal line going through bits 1<<7 and 1<<56
  y = (x ^ (x >> 63)) & 0x0000000000000001; x ^= y ^ (y << 63);
  y = (x ^ (x >> 54)) & 0x0000000000000102; x ^= y ^ (y << 54);
  y = (x ^ (x >> 45)) & 0x0000000000010204; x ^= y ^ (y << 45);
  y = (x ^ (x >> 36)) & 0x0000000001020408; x ^= y ^ (y << 36);
  y = (x ^ (x >> 27)) & 0x0000000102040810; x ^= y ^ (y << 27);
  y = (x ^ (x >> 18)) & 0x0000010204081020; x ^= y ^ (y << 18);
  y = (x ^ (x >>  9)) & 0x0001020408102040; x ^= y ^ (y <<  9);
  std::bitset<64> tmp(x);
  return mirror_vertical_line(tmp);
}