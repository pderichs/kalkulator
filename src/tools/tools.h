#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <deque>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "rectangle.h"

#define RUN_TEST(testproc) { int ___test_res = testproc(); if (___test_res) { std::cerr << "TEST " << #testproc << " failed." << std::endl; return ___test_res; } }
#define TEST_ASSERT(cond) { if (!(cond)) { std::cerr << __FILE__ << ":" << __LINE__ << ": error: *** TEST FAILURE: " << #cond << std::endl; return 1; } }

typedef std::vector<std::string> StringVector;
typedef std::set<std::string> StringSet;
typedef std::deque<std::string> StringDeque;

typedef std::vector<int> IntVector;
typedef std::vector<unsigned char> UnsignedCharVector;

namespace pdtools {
    std::string trim(const std::string &s);
    std::vector<std::string> split(const std::string &s, char delim);
    std::string stringVectorToString(const StringVector& v);
    std::string intVectorToString(const IntVector& v);
    std::string locationToString(const Location& l);
    std::string rectangleToString(const Rectangle& rect);
}; // namespace pdtools

// Thanks to https://github.com/osalbahr/adventOfCode/blob/main/problems/day17/day17_2.cpp
#define REPORT( X ) std::cout << #X << " = " << ( X ) << std::endl

#define MARK(msg) std::cout << "*** MARK (" << msg << ")" << std::endl

#endif
