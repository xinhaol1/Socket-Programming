#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include "readfile.h"
using namespace std;

bool line_is_alpha(string line) {
	for (char& c : line) {
		if (isalpha(c)) {return true;}
		// any character in the line containing a non digit makes this line a country name
	}
	return false;
}

vector<unsigned int> read_numeric_line(string line) {
	line += " ";
	vector<unsigned int> current_line;
	string current_int_str;
	for (char const &c : line) {
		if (c >= 48 && c <= 57) {
			current_int_str += c;
			// current character is a digit, append it to c
		}
		else {
			// current character is not a digit, transform c to an int and reset c
			if (current_int_str.size() != 0) {
				int current_int = atoi(current_int_str.c_str());
				if (find(current_line.begin(), current_line.end(), current_int) == current_line.end()) {
					// this integer has not been added to current line, add it
					current_line.push_back(current_int);
				}
				current_int_str.clear();
			}
		}
	}
	return current_line;
	// return a list of integers read from this line
}

vector<vector<vector<unsigned int>>> read_all_users(string filename) {
	string line;
	vector<vector<vector<unsigned int>>> all_areas_table;

	ifstream datafile;
	datafile.open(filename);
	if (datafile.is_open()) {
	getline(datafile, line);
		while (!datafile.eof()) {
			vector<vector<unsigned int>> one_area_table;
			while (line_is_alpha(line) && !datafile.eof()) {
				// this line is country name, skip it
				getline(datafile, line);
			}
			while(!line_is_alpha(line) && !datafile.eof()) {
				// this line is numeric, read it with read_numeric_line()
				vector<unsigned int> current_line_table = read_numeric_line(line);
				one_area_table.push_back(current_line_table);
				getline(datafile, line);
			}
			vector<unsigned int> current_line_table = read_numeric_line(line);
			one_area_table.push_back(current_line_table);
			all_areas_table.push_back(one_area_table);
			// user ids for one country is done
		}
	}
	datafile.close();
	return all_areas_table;
}

vector<string> read_all_areas(string filename) {
	string line;
	vector<string> all_area_names;
	ifstream datafile;
	datafile.open(filename);
	if (datafile.is_open()) {
		getline(datafile, line);
		while (!datafile.eof()) {
			while (line_is_alpha(line) && !datafile.eof()) {
				// this line is country name, save it
				all_area_names.push_back(line);
				getline(datafile, line);
			}
			while(!line_is_alpha(line) && !datafile.eof()) {
				// this line is numeric, skip it
				getline(datafile, line);
			}
		}
	}
	datafile.close();
	return all_area_names;
}