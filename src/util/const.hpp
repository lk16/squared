#pragma once

#include <string>

// subdirectory where images are stored
const std::string IMAGE_PATH = "./images/";

// subdirectory where xml files containing ui are stored
const std::string UI_PATH = "./xml/";

// subdirectory where book data is stored
const std::string BOOK_PATH = "./book/";

// perfect score factor
const int EXACT_SCORE_FACTOR = 1000;

// heighest possible heuristic value
const int MAX_HEURISTIC = 64 * EXACT_SCORE_FACTOR;

// lowest possible heuristic value
const int MIN_HEURISTIC = -1 * MAX_HEURISTIC;

// player colors
const int BLACK = 0;
const int WHITE = 1;