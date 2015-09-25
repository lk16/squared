#pragma once

#include <iostream>
#include <cassert>
#include <sstream>

class bits64{
  uint64_t word;
  
  static const char counts[65536];
  static const uint64_t mask_before[65];
  static const uint64_t mask_after[65];  
  static const uint64_t rotation_masks[7][256];

public:
  
  static const uint64_t mask_set[65];
  static const uint64_t mask_reset[65];
  
  
  bits64();
  bits64(unsigned long long);
  
  ~bits64() = default;
  
  bits64& operator=(const bits64& that);

  bits64& operator|=(const bits64& that);
  bits64& operator&=(const bits64& that);
  bits64& operator^=(const bits64& that);
  bits64& operator>>=(int n);
  bits64& operator<<=(int n);

  bits64 operator~() const;

  bits64 operator|(const bits64& that) const;
  bits64 operator&(const bits64& that) const;
  bits64 operator^(const bits64& that) const;
  bits64 operator>>(int n) const;
  bits64 operator<<(int n) const;
  
  operator bool() const;
  
  // tests equality
  bool operator==(const bits64& that) const;
  
  // tests inequality
  bool operator!=(const bits64& that) const;
  
  // resets all bits
  void reset_all();
  
  // resets all bits after bit i, 64 resets everything
  void reset_after(int i);
  
  // resets all bits before bit i, 64 resets everything
  void reset_before(int i);
  
  // resets bit i
  bits64& reset(int i);
  
  // sets all bits
  void set_all();
  
  // sets bit i
  bits64& set(int i);
  
  // returns 64 if b==0ul, returns least significant bit otherwise
  int first_index() const;
  
  // returns 64 if b==0ul, returns most significant bit otherwise
  int last_index() const;
  
  // returns the least significant bit
  bits64 first_bit() const;
  
  // returnss the most significant bit
  bits64 last_bit() const;
  
  // returns number of set bits
  int count() const;
  
  // returns whether *this has no set bits
  bool none() const;
  
  // returns whether *this has at least one set bit
  bool any() const;
  
  // returns whether all bits in *this are set
  bool all() const;
  
  // test if bit i is set
  bool test(int i) const;
  
  // rotates *this: n shall be between 0 and 7, both inclusive
  bits64 rotate(int n) const;
  
  // returns ~0ull if that a subset of *this or 0ull otherwise
  bits64 is_subset_of_mask(const bits64& that) const;

  // returns index of bitset where at most 1 bit is set
  // 64 is returned if bitset == 0ull
  int only_bit_index() const;
  
  // converts *this to ascii art
  std::string to_ascii() const;
  
  // equivalent to std::cout << this->to_ascii()
  void show() const;
  
  // returns underlying 64 bit word
  uint64_t get_word() const;
};

inline bits64::bits64()
{
  word = 0ull;
}

inline bits64::bits64(long long unsigned int x)
{
  word = x;
}

inline uint64_t bits64::get_word() const
{
  return word;
}


inline int bits64::first_index() const
{
  if(word == 0){
    return 64;
  }
#if 0 
  return __builtin_ffsl(b) - 1;
#else
  static const int index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
    61, 58, 50, 42, 38, 29, 17,  4,
    62, 55, 59, 36, 53, 51, 43, 22,
    45, 39, 33, 30, 24, 18, 12,  5,
    63, 47, 56, 27, 60, 41, 37, 16,
    54, 35, 52, 21, 44, 32, 23, 11,
    46, 26, 40, 15, 34, 20, 31, 10,
    25, 14, 19,  9, 13,  8,  7,  6
  };
  return index64[((word & -word) * 0x03f79d71b4cb0a89) >> 58];
#endif
}

inline int bits64::last_index() const
{
  if(word == 0ul){
    return 64;  // __builtin_clz(0ull) is undefined
  }
  return 63 - __builtin_clzl(word);  
}


inline int bits64::count() const
{
  return counts[word & 0xFFFF] + 
  counts[(word >> 16) & 0xFFFF] +
  counts[(word >> 32) & 0xFFFF] +
  counts[word >> 48];
}

inline bits64 bits64::first_bit() const
{
  return word & -word;
}

inline bits64 bits64::last_bit() const
{
  if(word == 0ull){
    return word;
  }
  return 1ull << (63 - __builtin_clzl(word));
}

inline bits64 bits64::rotate(int n) const
{
  // bits64::rotation_masks[7][256]
  // 0: left 
  // 1: right 
  // 2: 180 
  // 3: mirror vertically 
  // 4: mirror vertically + right
  // 5: mirror vertically + 180
  // 6: mirror vertically + left
  bits64 res = 0ull;
  switch(n){
    case 0:
      res = *this;
      break;
    case 1:
      for(int i=0;i<8;++i){
        res |= rotation_masks[0][(get_word() >> (8*i)) & 0xFF] << i;
      }
      break;
    case 2:
      for(int i=0;i<8;++i){
        res |= rotation_masks[1][(get_word() >> (8*i)) & 0xFF] >> i; 
      }
      break;
    case 3:
      for(int i=0;i<8;++i){
        res |= rotation_masks[2][(get_word() >> (8*i)) & 0xFF] >> i*8; 
      }
      break;
    case 4:
      for(int i=0;i<8;++i){
        res |= rotation_masks[3][(get_word() >> (8*i)) & 0xFF] << i*8; 
      }
      break;
    case 5:
      for(int i=0;i<8;++i){
        res |= rotation_masks[4][(get_word() >> (8*i)) & 0xFF] >> i; 
      }
      break;
    case 6:
      for(int i=0;i<8;++i){
        res |= rotation_masks[5][(get_word() >> (8*i)) & 0xFF] >> i*8; 
      }
      break;
    case 7:
      for(int i=0;i<8;++i){
        res |= rotation_masks[6][(get_word() >> (8*i)) & 0xFF] << i; 
      }
      break;
    default: 
      assert(0 && "invalid rotation value");
      break;
  }
  
  
  
  
  
  
  
  return res;
}


inline bits64 bits64::is_subset_of_mask(const bits64& that) const
{
  return bits64(((uint64_t)(bool)(~that & *this))-1);
  //return ((*this & that) == *this) ? ~0ull : 0ull;
}

inline int bits64::only_bit_index() const
{
  static const int table[83] = {
    64, 0, 1,-1, 2,27,-1, 8,
     3,62,28,24,-1,-1, 9,17,
     4,56,63,47,29,-1,25,60,
    -1,54,-1,52,10,12,18,38,
     5,14,57,35,-1,20,48,-1,
    30,40,-1,-1,26, 7,61,23,
    -1,16,55,46,-1,59,53,51,
    11,37,13,34,19,-1,39,-1,
     6,22,15,45,58,50,36,33,
    -1,-1,21,44,49,32,-1,43,
    31,42,41
  };
  
  assert(count() <= 1);
  int res = table[word%83];
  assert(res != -1);
  return res;
}

inline bool bits64::test(int i) const
{
  assert(i>=0 && i<=63);
  return word & mask_set[i];
}

inline bits64 bits64::operator&(const bits64& that) const
{
  return this->word & that.word;
}

inline bits64 bits64::operator|(const bits64& that) const
{
  return this->word | that.word;
}

inline bits64 bits64::operator^(const bits64& that) const
{
  return this->word ^ that.word;
}

inline bits64& bits64::operator=(const bits64& that)
{
  this->word = that.word;
  return *this;
}

inline bits64 bits64::operator~() const
{
  return ~word;
}

inline bits64::operator bool() const
{
  return (bool)word;
}

inline bits64 bits64::operator<<(int n) const
{
  assert(n>=0 && n<=63);
  return word << n;
}

inline bits64 bits64::operator>>(int n) const
{
  assert(n>=0 && n<=63);
  return word >> n;
}

inline bits64& bits64::operator<<=(int n)
{
  assert(n>=0 && n<=63);
  *this = *this << n;
  return *this;
}

inline bits64& bits64::operator>>=(int n)
{
  assert(n>=0 && n<=63);
  *this = *this >> n;
  return *this;
}

inline bits64& bits64::operator|=(const bits64& that)
{
  *this = *this | that;
  return *this;
}

inline bits64& bits64::operator&=(const bits64& that)
{
  *this = *this & that;
  return *this;
}

inline bits64& bits64::operator^=(const bits64& that)
{
  *this = *this ^ that;
  return *this;
}

inline std::string bits64::to_ascii() const
{
  int x,y;
  std::stringstream ss;
  
  ss << "+-----------------+\n";
  
  /* middle */
  for(y=0;y<8;y++){
    ss << "| ";
    for(x=0;x<8;x++){
      if(test(y*8+x)){
          ss << "@ ";
      }
      else{
        ss << "- ";
      }
    }
    ss << "|\n";
  }
  
  /* bottom line */
  ss  << "+-----------------+\n";
  
  return ss.str();
}

inline bool bits64::operator!=(const bits64& that) const
{
  return !(*this == that);
}

inline bool bits64::operator==(const bits64& that) const
{
  return this->word == that.word;
}

inline bool bits64::all() const
{
  return word == 0xFFFFFFFFFFFFFFFF;
}

inline bool bits64::any() const
{
  return !none();
}

inline bool bits64::none() const
{
  return word == 0x0;
}

inline void bits64::reset_all()
{
  word = 0x0;
}

inline bits64& bits64::reset(int i)
{
  assert(i>=0 && i<=63);
  word &= bits64::mask_reset[i];
  return *this;
}

inline void bits64::reset_after(int i)
{
  assert(i>=0 && i<=64);
  word &= bits64::mask_before[i];
}

inline void bits64::reset_before(int i)
{
  assert(i>=0 && i<=64);
  word &= bits64::mask_after[i];
}

inline bits64& bits64::set(int i)
{
  assert(i>=0 && i<=63);
  word |= bits64::mask_set[i];
  return *this;
}

inline void bits64::set_all()
{
  word = 0xFFFFFFFFFFFFFFFF;
}


