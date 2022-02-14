// This file is for CS3520 final project
// Author: Zhijian Wang, Qi Li
// Date: 2021/12/15
#include "Project.h"
#include "Sprint.h"
#include <algorithm>
#include <sstream>
#include <iostream>
Project::Project(string content) {
	size_t start = content.find("Project Name : ") + 15;
	size_t end = content.find('\n');
	name = content.substr(start, end - start);

	start = content.find("Project Description : ", end) + 22;
	end = content.find('\n', start);
	description = content.substr(start, end - start);

	start = content.find("Deadline : ", end) + 11;
	end = content.find('\n', start);
	deadline = stoi(content.substr(start, end - start));

	start = content.find("Current Time : ", end) + 15;
	end = content.find('\n', start);
	current_time = stoi(content.substr(start, end - start));

	start = content.find("Latest Issue ID : ", end) + 18;
	end = content.find('\n', start);
	latest_issue_id = stoi(content.substr(start, end - start));

	start = content.find("Latest Sprint ID : ", end) + 19;
	end = content.find('\n', start);
	latest_sprint_id = stoi(content.substr(start, end - start));

	start = content.find("To Do : ", end) + 8;
	end = content.find("In Progress : ", start);
	to_do.clear();
	string to_do_str = content.substr(start, end - start);
	if (to_do_str.substr(0, 5) != "Empty") {
		size_t to_do_start = to_do_str.find("- Issue", 1);
		size_t to_do_end = to_do_str.find("\n\n", to_do_start + 1);
		while (to_do_end != string::npos) {
			to_do.push_back(new Issue(to_do_str.substr(to_do_start, to_do_end - to_do_start + 1)));
			to_do_start = to_do_str.find_first_not_of("\n\t", to_do_end + 1);
			to_do_end = to_do_str.find("\n\n", to_do_start);
		}
	}

	start = content.find("In Progress : ", end) + 14;
	end = content.find("Work Done : ", start);
	in_progress.clear();
	string in_progress_str = content.substr(start, end - start);
	if (in_progress_str.substr(0, 5) != "Empty") {
		size_t in_progress_start = in_progress_str.find("Sprint", 1);
		size_t in_progress_end = in_progress_str.find("\n\n", in_progress_start + 1);
		while (in_progress_end != string::npos) {
			in_progress.push_back(new Sprint(in_progress_str.substr(in_progress_start, in_progress_end - in_progress_start + 1)));
			in_progress_start = in_progress_str.find_first_not_of("\n\t", in_progress_end + 1);
			in_progress_end = in_progress_str.find("\n\n", in_progress_start);
		}
	}

	start = content.find("Work Done : ", end) + 12;
	end = content.find("Users : ", start);
	work_done.clear();

	string work_done_str = content.substr(start, end - start);
	if (work_done_str.substr(0, 5) != "Empty") {
		size_t work_done_start = work_done_str.find("- Issue", 1);
		size_t work_done_end = work_done_str.find("\n\n", work_done_start + 1);
		while (work_done_end != string::npos) {
			work_done.push_back(new Issue(work_done_str.substr(work_done_start, work_done_end - work_done_start + 1)));
			work_done_start = work_done_str.find_first_not_of("\n\t", work_done_end + 1);
			work_done_end = work_done_str.find("\n\n", work_done_start);
		}
	}

	start = content.find("Users : ", end) + 8;
	end = content.find("All Changes : ", start);
	users.clear();
	string users_str = content.substr(start, end - start);
	if (users_str.substr(0, 5) != "Empty") {
		size_t users_start = users_str.find_first_not_of("\n\t");
		size_t users_end = users_str.find('\n', users_start + 1);
		while (users_end != string::npos) {
			users.push_back(new User(users_str.substr(users_start, users_end - users_start)));
			users_start = users_str.find_first_not_of("\n\t", users_end + 1);
			users_end = users_str.find('\n', users_start);
		}
	}

	start = content.find("All Changes : ", end) + 14;
	end = content.length();
	commits.clear();
	string commits_str = content.substr(start, end - start);
	if (commits_str.substr(0, 5) != "Empty") {
		size_t commits_start = commits_str.find_first_not_of("\n\t");
		size_t commits_end = commits_str.find('\n', commits_start + 1);
		while (commits_end != string::npos) {
			commits.push_back((new Comment())->read(commits_str.substr(commits_start, commits_end - commits_start)));
			commits_start = commits_str.find_first_not_of("\n\t", commits_end + 1);
			commits_end = commits_str.find('\n', commits_start);
		}
	}
}
Project::Project(string project_name, string project_description, int end_time) {
	name = project_name;
	description = project_description;
	deadline = end_time;
	latest_issue_id = 0;
	latest_sprint_id = 0;
	current_time = 0;
}
Project::~Project() {
	for (Issue* issue : to_do) {
		delete issue;
	}
	to_do.clear();
	for (Sprint* sprint : in_progress) {
		delete sprint;
	}
	in_progress.clear();
	for (Issue* issue : work_done) {
		delete issue;
	}
	work_done.clear();
	for (User* user : users) {
		delete user;
	}
	users.clear();
	for (Comment* comment : commits) {
		delete comment->commenter;
		delete comment;
	}
	commits.clear();
}
bool Project::set_name(string project_name, User* user) {
	if (user->get_permission() > 0) {
		name = project_name;
		add_commit_note(user, "The name of the project has been changed to " + project_name + ".");
		return true;
	}
	return false;
}
bool Project::set_deadline(int time, User* user) {
	if (user->get_permission() > 0) {
		deadline = time;
		add_commit_note(user, "The deadline of the project has been changed to " + std::to_string(time) + ".");
		return true;
	} 
	return false;
}
bool Project::set_description(string project_description, User* user) {
	if (user->get_permission() > 0) {
		description = project_description;
		add_commit_note(user, "The description of the project has been changed to " + project_description + ".");
		return true;
	}
	return false;
}

void Project::simulate_time(int time) {
	current_time += time;
	for (Sprint* sp : in_progress) {
		if (current_time > sp->get_end_time() && sp->get_status()) {
			for (Issue* i : *(sp->get_issues())) {
				Issue* issue_copy = new Issue(i->to_string(0));
				if (i->get_status() == "Done") {
					work_done.push_back(issue_copy);
				}
				else {
					issue_copy->set_status("In Progress");
					issue_copy->set_id(++latest_issue_id);
					issue_copy->set_description(i->get_description() + " [Migrated from issue " + std::to_string(i->get_id()) + "]");
					if (get_sprint(sp->get_id() + 1) != NULL && get_sprint(sp->get_id() + 1)->get_status()) {
						get_sprint(sp->get_id() + 1)->add_issue(issue_copy);
					}
					else {
						to_do.push_back(issue_copy);
					}
					i->set_status("Overdue");
				}
			}
			sp->set_status(false);
		}
	}
}

void Project::add_commit_note(User* user, string comment) {
	commits.push_back(new Comment{ new User(user->to_string()) , comment, current_time});
	sort(commits.begin(), commits.end(), [](Comment* comment1, Comment* comment2) {return comment1->time > comment2->time; });
}

// member operations
bool Project::invite(string collaborater_name, string title, int permission_level, User* inviter) {
	if (inviter != NULL && inviter->get_permission() <= 0) {
		return false;
	}
	for (User* user : users) {
		if (user->get_name() == collaborater_name) {
			return false;
		}
	}
	users.push_back(new User(collaborater_name, title, permission_level));
	if (inviter != NULL) {
		add_commit_note(inviter, collaborater_name + " is invited to the project.");
	}
	return true;
}
bool Project::assign_role(string collaborater_name, string title, int permission_level, User* inviter) {
	if (inviter->get_permission() <= 0) {
		return false;
	}
	for (User* user : users) {
		if (user->get_name() == collaborater_name) {
			user->set_title(title);
			user->set_permission(permission_level);
			add_commit_note(inviter, collaborater_name + " is now " + title + " with permission level " + std::to_string(permission_level) + ".");
			return true;
		}
	}
	return false;
}
bool Project::remove_member(string collaborater_name, User* inviter) {
	if (inviter->get_permission() <= 0) {
		return false;
	}
	for (auto it = users.begin(); it < users.end(); it++) {
		if ((*it)->get_name() == collaborater_name) {
			delete (*it);
			users.erase(it);
			add_commit_note(inviter, collaborater_name + " is removed from the project.");
			return true;
		}
	}
	return false;
}

User* Project::get_member(string collaborater_name) {
	for (User* user : users) {
		if (user->get_name() == collaborater_name) {
			return user;
		}
	}
	return NULL;
}

// sprint operations
bool Project::create_sprint(int start, int end, User* user) {
	if (end < current_time || start >= end || end > deadline|| user->get_permission() <= 0) {
		return false;
	}
	in_progress.push_back(new Sprint(++latest_sprint_id, start, end));
	sort(in_progress.begin(), in_progress.end(), [](Sprint* sprint1, Sprint* sprint2) {return *sprint1 > *sprint2; });
	add_commit_note(user, "A new sprint (id: " + std::to_string(latest_sprint_id) 
		+ ", time frame from "+ std::to_string(start) + " to " + std::to_string(end) + ") is created.");
	return true;
}
bool Project::modify_sprint(int sprint_id, int start, int end, User* user) {
	if (start >= end || end > deadline|| user->get_permission() <= 0) {
		return false;
	}
	for (Sprint* sprint : in_progress) {
		if (sprint->get_id() == sprint_id) {
			add_commit_note(user, "The time frame of sprint " + std::to_string(sprint->get_id())
				+ " (from " + std::to_string(sprint->get_start_time()) + " to " + std::to_string(sprint->get_end_time())
				+ ") is changed to (from " + std::to_string(start) + " to " + std::to_string(end) +").");
			sprint->set_start_time(start);
			sprint->set_end_time(end);
			sort(in_progress.begin(), in_progress.end(), [](Sprint* sprint1, Sprint* sprint2) {return *sprint1 > *sprint2; });
			return true;
		}
	}
	return false;
}

Sprint* Project::get_sprint(int sprint_id) {
	for (Sprint* sp : in_progress) {
		if (sp->get_id() == sprint_id) {
			return sp;
		}
	}
	return NULL;
}

// issue operations
bool Project::create_issue(string issue_type, string issue_name, string issue_description, int issue_priority, User* report_user) {
	to_do.push_back(new Issue(++latest_issue_id, current_time, issue_type, issue_name, issue_description, issue_priority, report_user));
	add_commit_note(report_user, "A new issue (" + issue_name + ", id:" + std::to_string(latest_issue_id) + ") is created.");
	sort(to_do.begin(),to_do.end(), [](Issue* issue1, Issue* issue2) {return *issue1 > *issue2; });
	return true;
}
bool Project::delete_issue(int issue_id, User* user) {
	if (user->get_permission() <= 0) {
		return false;
	}
	bool deleted = false;;
	for (int i = 0; i < to_do.size(); i++) {
		if (to_do[i]->get_id() == issue_id) {
			add_commit_note(user, "Issue with id " + std::to_string(issue_id) + " is deleted.");
			delete to_do[i];
			to_do.erase(to_do.begin() + i);
			deleted = true;
			break;
		}
	}
	for (Sprint* sprint : in_progress) {
		if (sprint->remove_issue(issue_id)) {
			add_commit_note(user, "Issue with id " + std::to_string(issue_id) + " is deleted.");
			deleted = true;
			break;
		}
	}
	for (int i = 0; i < work_done.size(); i++) {
		if (work_done[i]->get_id() == issue_id) {
			add_commit_note(user, "Issue with id " + std::to_string(issue_id) + " is deleted.");
			delete work_done[i];
			work_done.erase(work_done.begin() + i);
			deleted = true;
			break;
		}
	}
	return deleted;
}

bool Project::comment_issue(int issue_id, string comment, User* user) {
	Issue* issue = get_issue(issue_id);
	if (issue != NULL) {
		if (user->get_permission() > 0 || is_active_issue(issue_id) && issue->is_assigned_to(user->get_name())) {
			issue->add_comment(user, comment, current_time);
			return true;
		}
	} 
	return false;
}

bool Project::join_issue(int issue_id, User* user) {
	Issue* issue = get_issue(issue_id);
	if (issue != NULL) {
		if (user->get_permission() > 0 || is_active_issue(issue_id)) {
			return issue->assign_to(user);
		}
	}
	return false;
}

bool Project::assign_to_issue(int issue_id, string user_name, User* assigner) {
	if (assigner->get_permission() <= 0) {
		return false;
	}
	Issue* issue = get_issue(issue_id);
	User* user = NULL;
	for (User* u : users) {
		if (u->get_name() == user_name) {
			user = u;
		}
	}
	if (issue != NULL && user != NULL) {
		return issue->assign_to(user);
	}
	return false;
}

bool Project::finish_issue(int issue_id, User* user) {
	Issue* issue = get_issue(issue_id);
	if (issue != NULL) {
		if (user->get_permission() > 0 || is_active_issue(issue_id) && issue->is_assigned_to(user->get_name())) {
			issue->set_status("Done");
			return true;
		}
	}
	return false;
}

Issue* Project::get_issue(int issue_id) {
	for (auto it = to_do.begin(); it < to_do.end(); it++) {
		if ((*it)->get_id() == issue_id) {
			return *it;
		}
	}
	for (Sprint* sprint : in_progress) {
		for (Issue* issue : *(sprint->get_issues())) {
			if (issue->get_id() == issue_id) {
				return issue;
			}
		}
	}
	for (auto it = work_done.begin(); it < work_done.end(); it++) {
		if ((*it)->get_id() == issue_id) {
			return *it;
		}
	}
	return NULL;
}

bool Project::is_active_issue(int issue_id) {
	bool active = false;
	for (auto it = to_do.begin(); it < to_do.end(); it++) {
		if ((*it)->get_id() == issue_id && ((*it)->get_status() == "Open" || (*it)->get_status() == "In Progress")) {
			active = true;
		}
	}
	for (Sprint* sprint : in_progress) {
		for (Issue* issue : *(sprint->get_issues())) {
			if (issue->get_id() == issue_id && sprint->get_status() && (issue->get_status() == "Open" || issue->get_status() == "In Progress")) {
				active = true;
			}
		}
	}
	return active;
}

bool Project::add_to_sprint(int issue_id, int sprint_id, User* user) { // add to the given sprint and mark as in progress
	if (user->get_permission() <= 0) {
		return false;
	}
	Issue* issue = NULL;
	Sprint* sprint = NULL;
	for (Sprint* s : in_progress) {
		if (s->get_id() == sprint_id) {
			sprint = s;
		}
	}
	if (sprint == NULL) {
		return false;
	}
	for (auto it = to_do.begin(); it < to_do.end(); it++) {
		if ((*it)->get_id() == issue_id) {
			issue = *it;
			to_do.erase(it);
			break;
		}
	}
	for (Sprint* sprint : in_progress) {
		for (auto it = sprint->get_issues()->begin(); it < sprint->get_issues()->end(); it++) {
			if ((*it)->get_id() == issue_id) {
				issue = *it;
				sprint->get_issues()->erase(it);
				break;
			}
		}
	}
	for (auto it = work_done.begin(); it < work_done.end(); it++) {
		if ((*it)->get_id() == issue_id) {
			issue = *it;
			work_done.erase(it);
			break;
		}
	}
	if (issue == NULL) {
		return false;
	}
	issue->set_status("In Progress");
	sprint->add_issue(issue);
	return true;
}
// display
string Project::to_string() const {
	stringstream str;
	str << "Project Name : " << name << '\n';
	str << "Project Description : " << description << '\n';
	str << "Deadline : " << deadline << '\n';
	str << "Current Time : " << current_time << '\n';
	str << "Latest Issue ID : " << latest_issue_id << '\n';
	str << "Latest Sprint ID : " << latest_sprint_id << "\n\n";
	str << "To Do : ";
	if (to_do.size() == 0) {
		str << "Empty\n";
	}
	else {
		str << '\n';
	}
	for (Issue* issue : to_do) {
		str << issue->to_string(1) << '\n';
	}
	str << '\n';
	str << "In Progress : ";
	if (in_progress.size() == 0) {
		str << "Empty\n";
	}
	else {
		str << '\n';
	}
	for (Sprint* sprint : in_progress) {
		str << sprint->to_string(1) << '\n';
	}
	str << '\n';
	str << "Work Done : ";
	if (work_done.size() == 0) {
		str << "Empty\n";
	}
	else {
		str << '\n';
	}
	for (Issue* issue : work_done) {
		str << issue->to_string(1) << '\n';
	}
	str << '\n';
	str << "Users : " << view_users() << '\n';
	str << "All Changes : " << view_commits(commits.size()) << '\n';
	return str.str();
}
string Project::view_issue(int issue_id, User* user) const {
	stringstream str;
	for (auto it = to_do.begin(); it < to_do.end(); it++) {
		if ((*it)->get_id() == issue_id && ((*it)->is_assigned_to(user->get_name()) || user->get_permission() > 0)) {
			str << "Issue at 'To Do' : \n" <<(*it)->to_string(0);
		}
	}
	for (Sprint* sprint : in_progress) {
		for (Issue* issue : *(sprint->get_issues())) {
			if (issue->get_id() == issue_id && (issue->is_assigned_to(user->get_name()) || user->get_permission() > 0)) {
				str << "Issue at 'In Progress' : \n";
				if (sprint->get_status()) {
					str << "Deadline of its sprint is " << std::to_string(sprint->get_end_time()) << ".\n";
				}
				else {
					"The sprint it belongs to is closed.\n";
				}
				str << issue->to_string(0);
			}
		}
	}
	for (auto it = work_done.begin(); it < work_done.end(); it++) {
		if ((*it)->get_id() == issue_id && ((*it)->is_assigned_to(user->get_name()) || user->get_permission() > 0)) {
			str << "Issue at 'Work Done' : \n" << (*it)->to_string(0);
		}
	}
	return str.str();
}
string Project::view_area(string area, User* user) const {
	stringstream str;
	if (area == "to-do") {
		if (to_do.size() == 0) {
			return "Empty\n";
		}
		str << "Issues assigned in to do area : \n";
		str << "Issues Count : " << to_do.size() << '\n';
		for (Issue* issue : to_do) {
			if (issue->is_assigned_to(user->get_name()) || user->get_permission() > 0){
				str << issue->to_string(1) << '\n';
			}
		}
	}
	else if (area == "work-done") {
		if (work_done.size() == 0) {
			return "Empty\n";
		}
		str << "Issues finished in work done area : \n";
		str << "Issues Count : " << work_done.size() << '\n';
		for (Issue* issue : work_done) {
			if (issue->is_assigned_to(user->get_name()) || user->get_permission() > 0) {
				str << issue->to_string(1) << '\n';
			}
		}
	}
	else if (area == "in-progress") {
		if (in_progress.size() == 0) {
			return "Empty\n";
		}
		for (Sprint* sprint : in_progress) {
			str << "Sprint " << sprint->get_id() << " : From " << sprint->get_start_time() << " to " << sprint->get_end_time() << '\n';
			str << "Status : " << (sprint->get_status() ? "Open" : "Closed") << '\n';
			str << "Issues Count : " << (*sprint->get_issues()).size() << '\n';
			str << "Issues assigned : ";
			if ((*sprint->get_issues()).size() == 0) {
				str << "Empty";
			}
			else {
				for (Issue* issue : (*sprint->get_issues())) {
					if (issue->is_assigned_to(user->get_name()) || user->get_permission() > 0) {
						str << '\n';
						str << issue->to_string(1);
					}
				}
			}
			str << "\n\n";
		}	
	}
	return str.str();
}
string Project::view_project(User* user) const {
	if (user->get_permission() > 0) {
		return Project::to_string();
	}
	stringstream str;
	str << "Project Name : " << name << '\n';
	str << "Project Description : " << description << '\n';
	str << "Deadline : " << deadline << '\n';
	str << "Issue to do : " << to_do.size() << '\n';
	int count = 0;
	for (Sprint* sp : in_progress) {
		count += sp->get_issues()->size();
	}
	str << "Issue in progress : " << count << '\n';
	str << "Issue done : " << work_done.size() << '\n';
	str << "Collaborators : " << view_users() << '\n';
	return str.str();
}
string Project::view_users() const {
	if (users.size() == 0) {
		return "Empty\n";
	}
	stringstream str;
	str << '\n';
	for (User* user : users) {
		str << '\t' << user->to_string() << '\n';
	}
	return str.str();
}
string Project::view_commits(int amount) const {
	if (commits.size() == 0) {
		return "Empty\n";
	}
	stringstream str;
	str << '\n';
	int n = amount < commits.size() ? amount : commits.size();
	for (int i = 0; i < n; i++) {
		str << '\t' << commits[i]->to_string() << '\n';
	}
	return str.str();
}
string Project::view_current_state(User* user) const {
	stringstream str;
	str << "Current time : " << current_time << '\n';
	str << "Recent project activities : \n";
	str << view_commits(5) << '\n';
	str << "Issue related to you : \n";
	str << "\nTo Do : \n";
	str << view_area("to-do", user);
	str << "\nIn Progress : \n";
	str << view_area("in-progress", user);
	str << "\nWork Done : \n";
	str << view_area("work-done", user);
	return str.str();
}
string Project::get_name() const {
	return name;
}
int Project::get_current_time() const {
	return current_time;
}