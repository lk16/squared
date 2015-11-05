#pragma once

#include <string>

// subdirectory where images are stored
const std::string IMAGE_PATH = "./images/";

// subdirectory where xml files containing ui are stored
const std::string UI_PATH = "./xml/";

// multiplication factor for heuristics when finding an exact score 
const int EXACT_SCORE_FACTOR = 1000;

// highest possible heuristic value
const int MAX_HEURISTIC = 64 * EXACT_SCORE_FACTOR;

// lowest possible heuristic value
const int MIN_HEURISTIC = -MAX_HEURISTIC;

// highest possible perfect heuristic (capturing all opponents discs)
const int MAX_PERFECT_HEURISTIC = 64;

// lowest possible perfect heuristic (losing every disc)
const int MIN_PERFECT_HEURISTIC = -MAX_PERFECT_HEURISTIC;



// player colors
const int BLACK = 0;
const int WHITE = 1;