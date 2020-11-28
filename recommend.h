#include <vector>
#include <cstring>
using namespace std;

#ifndef RECOMMEND_H_
#define RECOMMEND_H_
long recommend(vector<vector<vector<unsigned int>>> all_areas_table, vector<string> all_areas_name, string area_name, int user_id, string server);
unsigned int find_highest_degree_user(vector<vector<unsigned int>> one_area_table, unsigned int current_user);
#endif
