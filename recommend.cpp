#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <climits>
#include "readfile.h"
#include "recommend.h"
#define USER_NOT_FOUND -1
#define NO_RECOMMENDATION -2

using namespace std;
long recommend(vector<vector<vector<unsigned int>>> all_areas_table, vector<string> all_areas_name, string area_name, int user_id, string server);
unsigned int find_highest_degree_user(vector<vector<unsigned int>> one_area_table, unsigned int current_user);

long recommend(vector<vector<vector<unsigned int>>> all_areas_table, vector<string> all_areas_name, string area_name, int user_id, string server) {
	vector<vector<unsigned int>> this_area_table;
	for (int i = 0; i < all_areas_name.size(); i++) {
		if (all_areas_name[i].compare(area_name) == 0) {
			this_area_table = all_areas_table[i];
			i = all_areas_name.size();
		}
		// find the graph for this country
	}

	vector<unsigned int> friend_list_one_user;
	bool user_found = false;
	for (int i = 0; i < this_area_table.size(); i++) {
		if (this_area_table[i][0] == user_id) {
			for (int j = 1; j < this_area_table[i].size(); j++) {
				friend_list_one_user.push_back(this_area_table[i][j]);
			}
			user_found = true;
			i = this_area_table.size();
		}
		// go through everyone in the graph for this country. if find user then save his friends to a list.
	}

	if (!user_found) { 
		// user id is not found in this country
		cout << "User " << user_id << " does not show up in " << area_name << endl;
		cout << "The server " << server << " has sent \"User " << user_id << " not found\" to Main Server" << endl;
		return USER_NOT_FOUND;
	}
	else {
		// user id is found in this country
		cout << "The server " << server << " is searching possible friends for User " << user_id << "..." << endl;
		vector<unsigned int> all_users;
		for (int i = 0; i < this_area_table.size(); i++) {
			all_users.push_back(this_area_table[i][0]);
		}
		// all_users is a list of all users in this country

		if (friend_list_one_user.size() + 1 == all_users.size()) {
			// If the user has already connect to everyone in the country
			cout << "Here are the results: [no more recommendation]" << endl;
			return NO_RECOMMENDATION;
		}

		// the user is not connected to everyone in the country
		map<unsigned int, int> candidate_list;
		for (int i = 0; i < friend_list_one_user.size(); i++) {
			unsigned int current_friend = friend_list_one_user[i];
			int current_friend_index;
			for (int j = 0; j < all_users.size(); j++) {
				if (all_users[j] == current_friend) {
					current_friend_index = j;
					j = all_users.size();
				}
			}
			// find which row this friend of user is located, then we can look at his/her friend list
			vector<unsigned int> candidate_list_from_current_friend = this_area_table[current_friend_index];
			for (int j = 1; j < candidate_list_from_current_friend.size(); j++) {
				unsigned int candidate_from_current_friend = candidate_list_from_current_friend[j];
				if ((find(friend_list_one_user.begin(), friend_list_one_user.end(), candidate_from_current_friend) == friend_list_one_user.end()) 
					&& (candidate_from_current_friend != user_id)) {
						if (candidate_list.find(candidate_from_current_friend) == candidate_list.end()) {
							candidate_list.insert({candidate_from_current_friend, 1}); 
						}
						else { candidate_list[candidate_from_current_friend] ++; }

				}
			}


			// loop through all of his/her friend, and add people who: 1. are not friends of user 2. are not user himself 3. are not yet one of candidates  to the candidate list
		}

		int max_degree = -1;
		unsigned int recommended_user = UINT_MAX;
		if (candidate_list.size() != 0) {
			// user is not yet connected to every friend of his friends.
			for (map<unsigned int, int>::iterator iter2 = candidate_list.begin(); iter2 != candidate_list.end(); iter2++) {
				max_degree = max(max_degree, iter2->second);
			}
			// find the maximum degree first
			for (map<unsigned int, int>::iterator iter2 = candidate_list.begin(); iter2 != candidate_list.end(); iter2++) {
				if (iter2->second == max_degree) {
					recommended_user = min(recommended_user, iter2->first);
				}
			}
			// find the id in the candidate list that have the maximum degree and the lowest id number
		}
		else {
			// user has connected to every friend of his friends.
			recommended_user = find_highest_degree_user(this_area_table, user_id);
		}
		
		cout << "Here are the results: User " << recommended_user << endl;
		return recommended_user; 
	}


}

unsigned int find_highest_degree_user(vector<vector<unsigned int>> one_area_table, unsigned int current_user) {
	vector<unsigned int> candidates;
	vector<unsigned int> friend_list_one_user;
	vector<int> degrees;

	for(int i = 0; i < one_area_table.size(); i++) {
		friend_list_one_user.clear();
		for (int j = 0; j < one_area_table[i].size(); j++) {
			if (find(friend_list_one_user.begin(), friend_list_one_user.end(), one_area_table[i][j]) == friend_list_one_user.end()){
				// not a duplicate
				friend_list_one_user.push_back(one_area_table[i][j]);
			}
		}
		degrees.push_back(friend_list_one_user.size() - 1);
		// a list of number of connections for each user
	}

	int max_degree = -1;
	for (int i = 0; i < degrees.size(); i++) {
		if (degrees[i] > max_degree && one_area_table[i][0] != current_user) {max_degree = degrees[i];}
		// find the maximum degree
	}
	for (int i = 0; i < degrees.size(); i++) {
		if ((degrees[i] == max_degree) && (one_area_table[i][0] != current_user)) {candidates.push_back(one_area_table[i][0]);}
		// find the users with maximum degree, excluding user himself
	}

	unsigned int min_id = UINT_MAX;
	for (int i = 0; i < candidates.size(); i++) {
		min_id = min(min_id, candidates[i]);
		// find the user with maximum degree and minimum id number
	}
	return min_id;
}