#include "game/board.hpp"


#define BOARD_MOVE_FIELD_BODY(field_id) \
bits64 board::do_move_##field_id(){\
  bits64 line,flipped = 0ull;\
  int end;\
  \
  bits64 left_border_mask,right_border_mask;\
  left_border_mask = right_border_mask = 0x0;\
  \
  switch(field_id%8){\
    case 0: right_border_mask = 0xFEFEFEFEFEFEFEFE; break;\
    case 1: right_border_mask = 0xFCFCFCFCFCFCFCFC; break;\
    case 2: right_border_mask = 0xF8F8F8F8F8F8F8F8; break;\
    case 3: right_border_mask = 0xF0F0F0F0F0F0F0F0; break;\
    case 4: right_border_mask = 0xE0E0E0E0E0E0E0E0; break;\
    case 5: right_border_mask = 0xC0C0C0C0C0C0C0C0; break;\
    default: right_border_mask = 0x0; break;\
  }\
  \
  switch(field_id%8){\
    case 2: left_border_mask = 0x0303030303030303; break;\
    case 3: left_border_mask = 0x0707070707070707; break;\
    case 4: left_border_mask = 0x0F0F0F0F0F0F0F0F; break;\
    case 5: left_border_mask = 0x1F1F1F1F1F1F1F1F; break;\
    case 6: left_border_mask = 0x3F3F3F3F3F3F3F3F; break;\
    case 7: left_border_mask = 0x7F7F7F7F7F7F7F7F; break;\
    default: left_border_mask = 0x0; break;\
  }\
  \
  /*right_border_mask = 0xFEFEFEFEFEFEFEFE;\
  left_border_mask  = 0x7F7F7F7F7F7F7F7F;\
  */\
  \
  \
  /* down */\
  if(field_id/8 < 6){\
    line = 0x0101010101010100l << field_id;\
    end = bits64_find_first(line & me);\
    line &= bits64_before[end];\
    if((opp & line) == line){\
      flipped |= line;\
    }\
  }\
  \
  /* up */\
  if(field_id/8 > 1){\
    line = 0x0080808080808080l >> (63-field_id);\
    end = bits64_find_last(line & me);\
    line &= bits64_after[end];\
    if((opp & line) == line){\
      flipped |= line;\
    }\
  }\
  \
  /* left */\
  if(field_id%8 > 1){\
    line = (0x7F00000000000000l >> (63-field_id)) & left_border_mask;\
    end = bits64_find_last(line & me);\
    line &= bits64_after[end];\
    if((opp & line) == line){\
      flipped |= line;\
    }\
  }\
  \
  /* right */\
  if(field_id%8 < 6){\
    line = (0x00000000000000FEl << field_id) & right_border_mask;\
    end = bits64_find_first(line & me);\
    line &= bits64_before[end];\
    if((opp & line) == line){\
      flipped |= line;\
    }\
  }\
  \
  /* right down */\
  if((field_id%8 < 6) && (field_id/8 < 6)){\
    line = (0x0040201008040201 << field_id) & right_border_mask;\
    end = bits64_find_first(line & me);\
    line &= bits64_before[end];\
    if((opp & line) == line){\
      flipped |= line;\
    }\
  }\
  \
  /* left up */\
  if((field_id%8 > 1) && (field_id/8 > 1)){\
    line = (0x8040201008040200 >> (63-field_id)) & left_border_mask;\
    end = bits64_find_last(line & me);\
    line &= bits64_after[end];\
    if((opp & line) == line){\
      flipped |= line;\
    }\
  }\
  \
  /* right up */\
  if((field_id%8 < 6) && (field_id/8 > 1)){\
    if(field_id<=56){\
      line = (0x0002040810204080 >> (56-field_id)) & right_border_mask;\
    }\
    else{\
      line = (0x0002040810204080 << (field_id-56)) & right_border_mask;\
    }\
    end = bits64_find_last(line & me);\
    line &= bits64_after[end];\
     if((opp & line) == line){\
      flipped |= line;\
    }\
  }\
  \
  /* left down */\
  if((field_id%8 > 1) && (field_id/8 < 6)){\
    if(field_id>=7){\
      line = (0x0102040810204000 << (field_id-7)) & left_border_mask;\
    }\
    else{\
      line = (0x0102040810204000 >> (7-field_id)) & left_border_mask;\
    }\
    end = bits64_find_first(line & me);\
    line &= bits64_before[end];\
    if((opp & line) == line){\
      flipped |= line;\
    }\
  }\
  \
  me |= bits64_set[field_id] | flipped;\
  opp &= ~me;\
  switch_turn();  \
  return flipped;\
}



BOARD_MOVE_FIELD_BODY(A1);
BOARD_MOVE_FIELD_BODY(A2);
BOARD_MOVE_FIELD_BODY(A3);
BOARD_MOVE_FIELD_BODY(A4);
BOARD_MOVE_FIELD_BODY(A5);
BOARD_MOVE_FIELD_BODY(A6);
BOARD_MOVE_FIELD_BODY(A7);
BOARD_MOVE_FIELD_BODY(A8);
BOARD_MOVE_FIELD_BODY(B1);
BOARD_MOVE_FIELD_BODY(B2);
BOARD_MOVE_FIELD_BODY(B3);
BOARD_MOVE_FIELD_BODY(B4);
BOARD_MOVE_FIELD_BODY(B5);
BOARD_MOVE_FIELD_BODY(B6);
BOARD_MOVE_FIELD_BODY(B7);
BOARD_MOVE_FIELD_BODY(B8);
BOARD_MOVE_FIELD_BODY(C1);
BOARD_MOVE_FIELD_BODY(C2);
BOARD_MOVE_FIELD_BODY(C3);
BOARD_MOVE_FIELD_BODY(C4);
BOARD_MOVE_FIELD_BODY(C5);
BOARD_MOVE_FIELD_BODY(C6);
BOARD_MOVE_FIELD_BODY(C7);
BOARD_MOVE_FIELD_BODY(C8);
BOARD_MOVE_FIELD_BODY(D1);
BOARD_MOVE_FIELD_BODY(D2);
BOARD_MOVE_FIELD_BODY(D3);
BOARD_MOVE_FIELD_BODY(D4);
BOARD_MOVE_FIELD_BODY(D5);
BOARD_MOVE_FIELD_BODY(D6);
BOARD_MOVE_FIELD_BODY(D7);
BOARD_MOVE_FIELD_BODY(D8);
BOARD_MOVE_FIELD_BODY(E1);
BOARD_MOVE_FIELD_BODY(E2);
BOARD_MOVE_FIELD_BODY(E3);
BOARD_MOVE_FIELD_BODY(E4);
BOARD_MOVE_FIELD_BODY(E5);
BOARD_MOVE_FIELD_BODY(E6);
BOARD_MOVE_FIELD_BODY(E7);
BOARD_MOVE_FIELD_BODY(E8);
BOARD_MOVE_FIELD_BODY(F1);
BOARD_MOVE_FIELD_BODY(F2);
BOARD_MOVE_FIELD_BODY(F3);
BOARD_MOVE_FIELD_BODY(F4);
BOARD_MOVE_FIELD_BODY(F5);
BOARD_MOVE_FIELD_BODY(F6);
BOARD_MOVE_FIELD_BODY(F7);
BOARD_MOVE_FIELD_BODY(F8);
BOARD_MOVE_FIELD_BODY(G1);
BOARD_MOVE_FIELD_BODY(G2);
BOARD_MOVE_FIELD_BODY(G3);
BOARD_MOVE_FIELD_BODY(G4);
BOARD_MOVE_FIELD_BODY(G5);
BOARD_MOVE_FIELD_BODY(G6);
BOARD_MOVE_FIELD_BODY(G7);
BOARD_MOVE_FIELD_BODY(G8);
BOARD_MOVE_FIELD_BODY(H1);
BOARD_MOVE_FIELD_BODY(H2);
BOARD_MOVE_FIELD_BODY(H3);
BOARD_MOVE_FIELD_BODY(H4);
BOARD_MOVE_FIELD_BODY(H5);
BOARD_MOVE_FIELD_BODY(H6);
BOARD_MOVE_FIELD_BODY(H7);
BOARD_MOVE_FIELD_BODY(H8);

#undef BOARD_MOVE_FIELD_BODY