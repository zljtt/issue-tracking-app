// This file is for CS3520 final project
// Author: Zhijian Wang, Qi Li
// Date: 2021/12/15
#include "Sprint.h"
#include "Project.h"
#include <algorithm>
#include <sstream>
#include <iostream>

Sprint::Sprint(string content) {
	size_t start = content.find("Sprint ") + 7;
	size_t end = content.find(" : ");
	sprint_id = stoi(content.substr(start, end - start));

	start = content.find("From ", end) + 5;
	end = content.find(" to ", start);
	start_time = stoi(content.substr(start, end - start));

	start = content.find(" to ", end) + 4;
	end = content.find('\n', start);
	end_time = stoi(content.substr(start, end - start));

	start = content.find("Status : ", end) + 9;
	end = content.find('\n', start);
	status = content.substr(start, end - start) == "Open" ? true : false;

	start = content.find("Issues : ", end) + 9;
	end = content.length();
	issues.clear();
	string issues_str = content.substr(start, end - start);
	if (issues_str.substr(0, 5) != "Empty") {
		size_t issues_start = issues_str.find("- Issue", 1);
		size_t issues_end = issues_str.find("- Issue", issues_start + 1);
		while (issues_start != string::npos) {
			if (issues_end == string::npos) {
				issues_end = issues_str.length();
				issues.push_back(new Issue(issues_str.substr(issues_start, issues_end - issues_start)));
				break;
			}
			issues.push_back(new Issue(issues_str.substr(issues_start, issues_end - issues_start)));
			issues_start = issues_str.find_first_not_of('\t', issues_end + 1);
			issues_end = issues_str.find("- Issue", issues_start + 1);
		}
	}
}
Sprint::Sprint(int id, int start, int end) {
	sprint_id = id;
	start_time = start;
	end_time = end;
	status = true;
}
Sprint::~Sprint() {
	for (Issue* issue : issues) {
		delete issue;
	}
	issues.clear();
}
bool Sprint::add_issue(Issue* issue) {
	for (Issue* i : issues) {
		if (*i == *issue) {
			return false;
		}
	}
	issues.push_back(issue);
	sort(issues.begin(), issues.end(), [](Issue* issue1, Issue* issue2) {return *issue1 > *issue2; });
	return true;
}
bool Sprint::remove_issue(int issue_id) {
	for (int i = 0; i < issues.size(); i++) {
		if (issues[i]->get_id() == issue_id) {
			delete issues[i];
			issues.erase(issues.begin() + i);
			return true;
		}
	}
	return false;
}
void Sprint::set_start_time(int time) {
	start_time = time;
}
void Sprint::set_end_time(int time) {
	end_time = time;
}
void Sprint::set_status(bool state){
	status = state;
}
vector<Issue*>* Sprint::get_issues() {
	return &issues;
}
int Sprint::get_start_time() const {
	return start_time;
}
int Sprint::get_end_time() const {
	return end_time;
}
bool Sprint::get_status() const {
	return status;
}
int Sprint::get_id() const{
	return sprint_id;
}
string Sprint::to_string(int indentation) const {
	stringstream str;
	str << string(indentation, '\t') << "Sprint " << sprint_id << " : From " << start_time << " to " << end_time << '\n';
	str << string(indentation, '\t') << "Status : " << (status ? "Open" : "Closed") << '\n';
	str << string(indentation, '\t') << "Issues : ";
	if (issues.size() == 0) {
		str << "Empty";
	}
	else {
		str << '\n';
		for (Issue* issue : issues) {
			str << issue->to_string(indentation + 1);
		}
	}
	str << '\n';
	return str.str();
}

bool Sprint::operator==(const Sprint& other) const {
	return sprint_id == other.sprint_id;
}

bool Sprint::operator>(const Sprint& other) const {
	return end_time > other.end_time;
}

bool Sprint::operator<(const Sprint& other) const {
	return end_time < other.end_time;
}