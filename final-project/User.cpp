// This file is for CS3520 final project
// Author: Zhijian Wang, Qi Li
// Date: 2021/12/15
#include "User.h"

User::User() {
	name = "";
	title = "";
	permission_level = 0;
}
User::User(string read_from) {
	name = read_from.substr(0, read_from.find('('));
	title = read_from.substr(read_from.find('(') + 1, read_from.find(",Permission:") - read_from.find('(') - 1);
	permission_level = stoi(read_from.substr(read_from.find(":") + 1, read_from.find(")") - read_from.find(":") - 1));
}
User::User(string user_name, string user_title, int permission) {
	name = user_name;
	title = user_title;
	permission_level = permission;
}
string User::get_name() const {
	return name;
}
string User::get_title() const {
	return title;
}
int User::get_permission() const {
	return permission_level;
}
string User::to_string() const {
	return name + '(' + title + ",Permission:" + std::to_string(permission_level) + ')';
}
void User::set_title(string user_title) {
	title = user_title;
}
void User::set_permission(int level) {
	permission_level = level;
}
bool User::operator==(const User& other) const {
	return name == other.name;
}