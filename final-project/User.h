// This file is for CS3520 final project
// Author: Zhijian Wang, Qi Li
// Date: 2021/12/15
#pragma once
#include <string>
using namespace std;
class User {
private:
	string name;
	string title;
	int permission_level;
public:
	User();
	User(string read_from);
	User(string user_name, string user_title, int permission);
	string get_name() const;
	string get_title() const;
	int get_permission() const;
	string to_string() const;
	void set_title(string user_title);
	void set_permission(int level);
	bool operator==(const User& other) const;
};

