// This file is for CS3520 final project
// Author: Zhijian Wang, Qi Li
// Date: 2021/12/15
#include "Issue.h"
#include <sstream>
#include <iostream>

Issue::Issue(string content) {
	size_t start = content.find("Issue ") + 6;
	size_t end = content.find(" : ");
	issue_id = stoi(content.substr(start, end - start));

	start = content.find(" : ") + 3;
	end = content.find(" (", start);
	name = content.substr(start, end - start);

	start = content.find(" (", end) + 2;
	end = content.find(")\n", start);
	type = content.substr(start, end - start);

	start = content.find("Reported by : ", end) + 14;
	end = content.find(" at ", start);
	reporter = new User(content.substr(start, end - start));

	start = content.find(" at ", end) + 4;
	end = content.find('\n', start);
	created_time = stoi(content.substr(start, end - start));

	start = content.find("Description ", end) + 14;
	end = content.find('\n', start);
	description = content.substr(start, end - start);

	start = content.find("Priority ", end) + 11;
	end = content.find('\n', start);
	priority = stoi(content.substr(start, end - start));

	start = content.find("Status ", end) + 9;
	end = content.find('\n', start);
	status = content.substr(start, end - start);

	// assignees
	start = content.find("Assignees : ", end) + 12;
	end = content.find('\n', start);
	assignees.clear();
	string assignees_str = content.substr(start, end - start);
	if (assignees_str.substr(0, 5) != "Empty") {
		size_t assignees_start = 1;
		size_t assignees_end = assignees_str.find("] ");
		while (assignees_end != string::npos) {
			assignees.push_back(new User(assignees_str.substr(assignees_start, assignees_end - assignees_start)));
			assignees_start = assignees_end + 3;
			assignees_end = assignees_str.find("] ", assignees_start);
		}
	}
	// comments
	start = content.find("Comments : ", end) + 11;
	end = content.length();
	comments.clear();
	string comments_str = content.substr(start, end - start);
	if (comments_str.substr(0, 5) != "Empty") {
		size_t comments_start = comments_str.find_first_not_of('\t', 1);
		size_t comments_end = comments_str.find('\n', comments_start + 1);
		while (comments_end != string::npos) {
			string comment_content = comments_str.substr(comments_start, comments_end - comments_start);
			comments.push_back((new Comment())->read(comment_content));
			comments_start = comments_str.find_first_not_of('\t', comments_end + 1);
			comments_end = comments_str.find('\n', comments_start);
		}
	}
}

Issue::Issue(int id, int start, string issue_type, string issue_name, string issue_description, int issue_priority, User* report_user) {
	issue_id = id;
	created_time = start;
	type = issue_type;
	name = issue_name;
	description = issue_description;
	priority = issue_priority;
	reporter = new User(report_user->to_string());
	status = "Open";
}
Issue::~Issue() {
	for (User* user : assignees) {
		delete user;
	}
	assignees.clear();
	delete reporter;
	
	for (Comment* comment : comments) {
		delete comment->commenter;
		delete comment;
	}
	comments.clear();
}
bool Issue::assign_to(User* user) {
	for (User* u : assignees) {
		if (*u == *user) {
			return false;
		}
	}
	assignees.push_back(new User(user->to_string()));
	return true;
}
bool Issue::remove_assignee(string name) {
	for (auto it = assignees.begin(); it < assignees.end(); it++) {
		if ((*it)->get_name() == name) {
			assignees.erase(it);
			return true;
		}
	}
	return false;
}
void Issue::add_comment(User* user, string comment, int time) {
	comments.push_back( new Comment{ new User(user->to_string()) , comment, time});
}
void Issue::set_type(string issue_type) {
	type = issue_type;
}
void Issue::set_name(string issue_name) {
	name = issue_name;
}
void Issue::set_description(string issue_description) {
	description = issue_description;
}
void Issue::set_priority(int issue_priority) {
	priority = issue_priority;
}
void Issue::set_status(string issue_status) {
	status = issue_status;
}
void Issue::set_id(int id){
	issue_id = id;
}
int Issue::get_id() const {
	return issue_id;
}
int Issue::get_created_time() const{
	return created_time;
}
string Issue::get_type() const {
	return type;
}
string Issue::get_name() const{
	return name;
}
string Issue::get_description() const {
	return description;
}
int Issue::get_priority() const {
	return priority;
}
string Issue::get_status() const {
	return status;
}
User* Issue::get_reporter() const {
	return reporter;
}
vector<Comment*> Issue::get_comments() const {
	return comments;
}
vector<User*> Issue::get_assignees() const {
	return assignees;
}
bool Issue::is_assigned_to(string name) const {
	for (User* user : assignees) {
		if (user->get_name() == name) {
			return true;
		}
	}
	return false;
}
string Issue::to_string(int indentation) const {
	stringstream str;
	str << string(indentation, '\t') << "- Issue " << issue_id << " : " << name << " (" << type << ")\n";
	str << string(indentation, '\t') << "Reported by : " << reporter->to_string() << " at " << created_time << '\n';
	str << string(indentation, '\t') << "Description : " << description << '\n';
	str << string(indentation, '\t') << "Priority : " << priority << '\n';
	str << string(indentation, '\t') << "Status : " << status << '\n';
	str << string(indentation, '\t') << "Assignees : ";
	if (assignees.size() == 0) {
		str << "Empty";
	}
	for (User* u : assignees) {
		str << "[" << u->to_string() << "] ";
	}
	str << "\n" << string(indentation, '\t') << "Comments : ";
	if (comments.size() == 0) {
		str << "Empty";
	}
	for (auto it = comments.begin(); it < comments.end(); it++) {
		str << '\n' << string(indentation + 1, '\t') << (*it)->to_string();
	}
	str << '\n';
	return str.str();
}

string Comment::to_string() const{
	return  commenter->to_string() + " at time " + std::to_string(time) + " : " + content;
}

Comment* Comment::read(string comment) {
	int index_1 = comment.find(" at time ");
	int index_2 = comment.find(" : ");
	commenter = new User(comment.substr(0, index_1));
	time = stoi(comment.substr(index_1 + 9, index_2 - index_1 - 9));
	content = comment.substr(index_2 + 3, comment.length() - index_2 - 3);
	return this;
}

bool Issue::operator==(const Issue& other) const {
	return issue_id == other.issue_id;
}

bool Issue::operator>(const Issue& other) const {
	return priority > other.priority;
}

bool Issue::operator<(const Issue& other) const {
	return priority < other.priority;
}