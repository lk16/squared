#include "testing.hpp"
#include "bots/bot_ali.hpp"
#include "util/csv.hpp"
#include "util/math.hpp"


void print_time_diff(timeval start,timeval end,int runs,const std::string& name){
  
  double time_diff = (end.tv_sec + (end.tv_usec / 1000000.0)) -
  (start.tv_sec + (start.tv_usec / 1000000.0)); 
  std::cout << name << '\t' << runs << " / " << time_diff; 
  std::cout << " = " << big_number(runs/time_diff) << " per sec avg\n"; 
}

bits64 get_dir_mask(int,int);

void testing_area()
{
#if 0  
  std::cout << "const bits64 board::dir_mask[64][8] = {\n";
  for(int i=0;i<64;i++){
    std::cout << "\t{\n";
    for(int d=0;d<8;++d){
      std::cout << "\t\t0x" << std::hex << get_dir_mask(d,i) << (d==8 ? "" : ",") << "\n";
    }
    std::cout << "}" <<  (i==63 ? "":",") << "\n";
  }
  std::cout << "};\n";
#endif
  
}

bits64 rshift(bits64 lhs,int rhs){
  if(rhs >= 0){
    return lhs >> rhs;
  }
  return lhs << (-rhs);
}

bits64 lshift(bits64 lhs,int rhs){
  if(rhs >= 0){
    return lhs << rhs;
  }
  return lhs >> (-rhs);
}


// dir = 5, field_id = 4

bits64 get_dir_mask(int dir,int field_id){
  bits64 line;
  
  bits64 left_border_mask,right_border_mask;
  
  switch(field_id%8){
    case 0: right_border_mask = 0xFEFEFEFEFEFEFEFE; break;
    case 1: right_border_mask = 0xFCFCFCFCFCFCFCFC; break;
    case 2: right_border_mask = 0xF8F8F8F8F8F8F8F8; break;
    case 3: right_border_mask = 0xF0F0F0F0F0F0F0F0; break;
    case 4: right_border_mask = 0xE0E0E0E0E0E0E0E0; break;
    case 5: right_border_mask = 0xC0C0C0C0C0C0C0C0; break;
    default: right_border_mask = 0x0; break;
  }
  
  switch(field_id%8){
    case 2: left_border_mask = 0x0303030303030303; break;
    case 3: left_border_mask = 0x0707070707070707; break;
    case 4: left_border_mask = 0x0F0F0F0F0F0F0F0F; break;
    case 5: left_border_mask = 0x1F1F1F1F1F1F1F1F; break;
    case 6: left_border_mask = 0x3F3F3F3F3F3F3F3F; break;
    case 7: left_border_mask = 0x7F7F7F7F7F7F7F7F; break;
    default: left_border_mask = 0x0; break;
  }
  
  line = 0x0;
  
  switch(dir){
    case 0:
      /* left up */
      if((field_id%8 > 1) && (field_id/8 > 1)){
        line = (0x0040201008040201 >> (63-field_id)) & left_border_mask;
      }
    break;
    case 1:
      /* up */
      if(field_id/8 > 1){
        line = 0x0080808080808080l >> (63-field_id);
      }
    break;
    case 2:
      /* right up */
      if((field_id%8 < 6) && (field_id/8 > 1)){
        line = rshift(0x0002040810204080,56-field_id) & right_border_mask;
      }
    break;
    case 3:
      /* left */
      if(field_id%8 > 1){
        line = (0x7F00000000000000l >> (63-field_id)) & left_border_mask;
      }
    break;
    case 4:
      /* right */
      if(field_id%8 < 6){
        line = (0x00000000000000FEl << field_id) & right_border_mask;
      }
    break;
    case 5:
      /* left down */
      if((field_id%8 > 1) && (field_id/8 < 6)){
        line = lshift(0x0102040810204000,field_id-7) & left_border_mask;
      }
    break;
    case 6:
      /* down */
      if(field_id/8 < 6){
        line = 0x0101010101010100l << field_id;
      }
    break;
    case 7:
      /* right down */
      if((field_id%8 < 6) && (field_id/8 < 6)){
        line = (0x8040201008040200 << field_id) & right_border_mask;
      }
    break;
    default:
      std::cout << "WTF?!?\n";
      assert(0);
      break;
  }
  return line;
}