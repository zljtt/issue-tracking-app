// This file is for CS3520 final project
// Author: Zhijian Wang, Qi Li
// Date: 2021/12/15
#pragma once
#include <string>
#include <vector>
#include "Issue.h"

using namespace std;
class Sprint;
class Project {
private:
	string name;
	string description;
	int current_time;
	int deadline;
	vector<Issue*> to_do;
	vector<Sprint*> in_progress;
	vector<Issue*> work_done;
	vector<User*> users;
	vector<Comment*> commits;
	int latest_issue_id;
	int latest_sprint_id;
public:
	// read from the string / txt file, may also use for copy
	Project(string content);
	// normal init
	Project(string project_name, string project_description, int end_time);
	~Project();
	bool set_name(string project_name, User* user);
	bool set_deadline(int time, User* user);
	bool set_description(string project_description, User* user);
	void simulate_time(int time);
	void add_commit_note(User* user, string comment);// which will be showed in current status
	// member operations
	bool invite(string collaborater_name, string title, int permission_level, User* inviter);
	bool assign_role(string collaborater_name, string title, int permission_level, User* inviter);
	bool remove_member(string collaborater_name, User* inviter);
	User* get_member(string collaborater_name);
	// sprint operations
	bool create_sprint(int start, int end, User* user);
	bool modify_sprint(int sprint_id, int start, int end, User* user);
	Sprint* get_sprint(int sprint_id);
	// issue operations
	bool create_issue(string issue_type, string issue_name, string issue_description, int issue_priority, User* report_user);
	bool delete_issue(int issue_id, User* user);
	bool comment_issue(int issue_id, string comment, User* user);
	bool join_issue(int issue_id, User* user);
	bool assign_to_issue(int issue_id, string user_name, User* assigner);
	bool finish_issue(int issue_id, User* user);
	Issue* get_issue(int issue_id);
	bool is_active_issue(int issue_id);
	bool add_to_sprint(int issue_id, int sprint_id, User* user); // add to the given sprint and mark as in progress
	// display
	string to_string() const;
	string view_issue(int issue_id, User* user) const;
	string view_area(string area, User* user) const;
	string view_project(User* user) const;
	string view_users() const;
	string view_commits(int amount) const;
	string view_current_state(User* user) const;
	string get_name() const;
	int get_current_time() const;
};

