// This file is for CS3520 final project
// Author: Zhijian Wang, Qi Li
// Date: 2021/12/15
#pragma once
#include <string>
#include <vector>
#include "Issue.h"
using namespace std;
class Project;
class Sprint {
private:
	int sprint_id;
	int start_time;
	int end_time;
	bool status;
	vector<Issue*> issues;
public:
	Sprint(string content);
	Sprint(int id, int start, int end);
	~Sprint();
	bool add_issue(Issue* issue);
	bool remove_issue(int issue_id);
	void set_start_time(int time);
	void set_end_time(int time);
	void set_status(bool status);
	vector<Issue*>* get_issues();
	int get_start_time() const;
	int get_end_time() const;
	bool get_status() const;
	int get_id() const;
	string to_string(int indentation) const;
	bool operator==(const Sprint& other) const;
	bool operator<(const Sprint& other) const;
	bool operator>(const Sprint& other) const;
};

