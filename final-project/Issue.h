// This file is for CS3520 final project
// Author: Zhijian Wang, Qi Li
// Date: 2021/12/15
#pragma once
#include <string>
#include <vector>
#include <map>
#include "User.h"
using namespace std;
struct Comment {
	User* commenter;
	string content;
	int time;
	string to_string() const;
	Comment* read(string comment);
};

class Issue {
private:
	int issue_id;
	int created_time;
	string type;
	string name;
	string description;
	vector<User*> assignees;
	User* reporter;
	int priority;
	string status;
	vector<Comment*> comments;
public:
	Issue(string content);
	Issue(int id, int start, string issue_type, string issue_name, string issue_description, int issue_priority, User* report_user);
	~Issue();
	bool assign_to(User* user);
	bool remove_assignee(string name);
	void add_comment(User* user, string comment, int time);
	void set_type(string issue_type);
	void set_name(string issue_name);
	void set_description(string issue_description);
	void set_priority(int issue_priority);
	void set_status(string issue_status);
	void set_id(int id);
	int get_id() const;
	int get_created_time() const;
	string get_type() const;
	string get_name() const;
	string get_description() const;
	int get_priority() const;
	string get_status() const;
	User* get_reporter() const;
	vector<Comment*> get_comments() const;
	vector<User*> get_assignees() const;
	bool is_assigned_to(string name) const;
	string to_string(int intendation) const;
	bool operator==(const Issue& other) const;
	bool operator<(const Issue& other) const;
	bool operator>(const Issue& other) const;
};

