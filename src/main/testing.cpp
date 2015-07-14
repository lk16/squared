#include "testing.hpp"
#include "bots/bot_ali.hpp"
#include "util/csv.hpp"

#include <future>
#include <vector>
#include <set>
#include <unistd.h>

void print_time_diff(timeval start,timeval end,int runs,const std::string& name){
  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0)); 
  std::cout << name << '\t' << runs << " / " << time_diff; 
  std::cout << " = " << big_number(runs/time_diff) << " per sec avg\n"; 
}

void testing_area()
{
  std::cout << "static const int dir_mask[8][64] = {\n";
  for(int dx=-1;dx<=1;++dx){
    for(int dy=-1;dy<=1;++dy){
      if(dx!=0 || dy!=0){
        std::cout << "\t{\n";
        for(int i=0;i<64;i++){
          bits64 res = 0ull;
          int y = i/8;
          int x = i%8;
          for(;x!=-1 && x!=8 && y!=-1 && y!=8;x+=dx,y+=dy){
            res |= 1ull << (8*y+x);
          }
          std::cout << "\t\t0x" << std::hex << res << ",\n";
        }
        std::cout << "},\n";
      }
    }
  }
}
