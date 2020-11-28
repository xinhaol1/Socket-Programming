#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctype.h>
#include <vector>
using namespace std;

#ifndef READFILE_H_
#define READFILE_H_
bool line_is_alpha(string line);
vector<unsigned int> read_numeric_line(string line);
vector<vector<vector<unsigned int>>> read_all_users(string filename);
vector<string> read_all_areas(string filename);
#endif