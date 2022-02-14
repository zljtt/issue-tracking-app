// This file is for CS3520 final project
// Author: Zhijian Wang, Qi Li
// Date: 2021/12/15
#include "User.h"
#include "Sprint.h"
#include "Issue.h"
#include "Project.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
using namespace std;

int get_output_integer();
bool is_digit(string value);

int main() {
	// create project and log in
	ifstream in_file("project.txt");
	Project* project = NULL;
	User* current_user = NULL;
	if (in_file.is_open()) {
		stringstream str;
		str << in_file.rdbuf();
		project = new Project(str.str());
		cout << "... Loading " << project->get_name() << "..." << endl;
		// log in
		cout << "Please enter your user name:\t";
		string user_name;
		getline(cin, user_name);
		current_user = project->get_member(user_name);
		while (current_user == NULL) {
			cout << "User not found in the project, please enter again:\t";
			getline(cin, user_name);
			current_user = project->get_member(user_name);
		}
		in_file.close();
	}
	else {
		cout << "There is no project found, please create a new one." << endl;
		string name;
		string description;
		int deadline;
		string project_owner_name;
		cout << "Please enter the new project name:\t";
		getline(cin, name);
		cout << "Please enter the project description:\t";
		getline(cin, description);
		cout << "Please enter the project deadline (an int, unit is 'day'):\t";
		deadline = get_output_integer();
		project = new Project(name, description, deadline);
		// auto login as owner
		cout << "Please enter your name as the project owner:\t";
		getline(cin, project_owner_name);
		project->invite(project_owner_name, "Project Owner", 2, NULL);
		current_user = project->get_member(project_owner_name);
	}
	// display first log in information
	cout << "______________________________________________________\n\n";
	cout << project->view_current_state(current_user);
	cout << "______________________________________________________\n\n";
	// menu
	string command = "";
	cout << "Please enter commands: " << endl;
	cout << "Enter 'help' to see all supported commands." << endl;
	while (command != "quit ") {
		// obtain command tokens
		getline(cin, command);
		command += ' ';
		vector<string> tokens;
		size_t start = 0;
		size_t end = command.length();
		while (start < command.length() && start != std::string::npos) {
			if (command.at(start) == '"') {
				end = command.find('"', start + 1);
				if (end != std::string::npos) {
					tokens.push_back(command.substr(start + 1, end - start - 1));
					start = end + 2;
				}
			}
			else {
				end = command.find(' ', start);
				if (end != std::string::npos && command.substr(start, end - start) != "") {
					tokens.push_back(command.substr(start, end - start));
				}
				start = end + 1;
			}
		}
		int add_time = 1 + rand() % 9;
		if (tokens.size() > 0) {
			// view
			if (tokens[0] == "view") {
				if (tokens.size() > 1) {
					if (tokens[1] == "status") {
						cout << "______________________________________________________\n\n";
						cout << project->view_current_state(current_user);
						cout << "______________________________________________________\n";
					}
					else if (tokens[1] == "project") {
						cout << "______________________________________________________\n\n";
						cout << project->view_project(current_user);
						cout << "______________________________________________________\n";
					}
					else if (tokens[1] == "to-do") {
						cout << "______________________________________________________\n\n";
						cout << project->view_area("to-do", current_user);
						cout << "______________________________________________________\n";
					}
					else if (tokens[1] == "in-progress") {
						cout << "______________________________________________________\n\n";
						cout << project->view_area("in-progress", current_user);
						cout << "______________________________________________________\n";
					}
					else if (tokens[1] == "work-done") {
						cout << "______________________________________________________\n\n";
						cout << project->view_area("work-done", current_user);
						cout << "______________________________________________________\n";
					}
					else if (tokens[1] == "issue" && tokens.size() > 2) {
						// View issue
						if (is_digit(tokens[2])) {
							string str = project->view_issue(stoi(tokens[2]), current_user);
							if (str == "") {
								cout << "Issue not found, or permission denied.\n";
							}
							else {
								cout << "______________________________________________________\n\n";
								cout << str;
								cout << "______________________________________________________\n";
							}						
						}
					}
					else {
						cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
					}
				}
				else {
					cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
				}
			}
			// issue 
			else if (tokens[0] == "issue") {
				if (tokens.size() > 1) {
					if (tokens[1] == "create" && tokens.size() > 5 && is_digit(tokens[5])) {
						project->create_issue(tokens[3], tokens[2], tokens[4], stoi(tokens[5]), current_user);
						cout << "Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
					}
					else if (tokens[1] == "join" && tokens.size() > 2 && is_digit(tokens[2])) {
						cout << (project->join_issue(stoi(tokens[2]), current_user) ? 
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n" 
							: "Issue does not exist, or you are already in the issue, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else if (tokens[1] == "comment" && tokens.size() > 3 && is_digit(tokens[2])) {
						cout << (project->comment_issue(stoi(tokens[2]), tokens[3], current_user) ? 
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n" 
							: "Issue does not exist, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else if (tokens[1] == "finish" && tokens.size() > 2 && is_digit(tokens[2])) {
						cout << (project->finish_issue(stoi(tokens[2]), current_user) ?
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n" 
							: "Issue does not exist, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else if (tokens[1] == "move" && tokens.size() > 3 && is_digit(tokens[2]) && is_digit(tokens[3])) {
						cout << (project->add_to_sprint(stoi(tokens[2]), stoi(tokens[3]), current_user) ?
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n"
							: "Issue/sprint does not exist, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else if (tokens[1] == "modify" && tokens.size() > 4 && is_digit(tokens[2])) {
						Issue* modify_issue = project->get_issue(stoi(tokens[2]));
						if (modify_issue == NULL) {
							cout << "Issue " << tokens[2] << " does not exist (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else if (current_user->get_permission() <= 0) {
							cout << "Permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else if (tokens[3] == "name") {
							modify_issue->set_name(tokens[4]);
							cout << "Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else if (tokens[3] == "description") {
							modify_issue->set_description(tokens[4]);
							cout << "Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else if (tokens[3] == "priority" && is_digit(tokens[4])) {
							modify_issue->set_priority(stoi(tokens[4]));
							cout << "Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else if (tokens[3] == "status") {
							modify_issue->set_status(tokens[4]);
							cout << "Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else {
							cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
					}
					else if (tokens[1] == "assign" && tokens.size() > 3 && is_digit(tokens[2])) {
						cout << (project->assign_to_issue(stoi(tokens[2]), tokens[3], current_user) ?
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n" 
							: "Issue/user does not exist, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else if (tokens[1] == "delete" && tokens.size() > 2 && is_digit(tokens[2])) {
						cout << (project->delete_issue(stoi(tokens[2]), current_user) ?
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n"
							: "Issue does not exist, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else {
						cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
					}
				}
				else {
					cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
				}
			}
			// project 
			else if (tokens[0] == "project") {
				if (tokens.size() > 1) {
					if (tokens[1] == "modify" && tokens.size() > 3) {
						if (tokens[2] == "name") {
							cout << (project->set_name(tokens[3], current_user) ? 
								"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n"
								: "Permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
						}
						else if (tokens[2] == "description") {
							cout << (project->set_description(tokens[3], current_user) ?
								"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n"
								: "Permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
						}
						else if (tokens[2] == "deadline" && is_digit(tokens[3])) {
							cout << (project->set_deadline(stoi(tokens[3]), current_user) ?
								"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n"
								: "Permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
						}
						else {
							cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
					}
					else if (tokens[1] == "delete") {
						if (current_user->get_permission() >= 2) {
							project = NULL;
							cout << "Process succeeds.\n";
							break;
						}
						else {
							cout << "Permission denied.\n";
						}
					}
					else {
						cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
					}
				}
				else {
					cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
				}
			}
			// user 
			else if (tokens[0] == "user") {
				if (tokens.size() > 1) {
					if (tokens[1] == "invite" && tokens.size() > 4 && is_digit(tokens[4])) {
						cout << (project->invite(tokens[2],tokens[3], stoi(tokens[4]), current_user) ?
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n"
							: "The user already exists, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else if (tokens[1] == "assign" && tokens.size() > 4 && is_digit(tokens[4])) {
						cout << (project->assign_role(tokens[2], tokens[3], stoi(tokens[4]), current_user) ?
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n"
							: "User not found, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else if (tokens[1] == "remove" && tokens.size() > 2) {
						cout << (project->remove_member(tokens[2], current_user) ?
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n"
							: "User not found, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else {
						cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
					}
				}
				else {
					cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
				}
			}
			// sprint 
			else if (tokens[0] == "sprint") {
				if (tokens.size() > 1) {
					if (tokens[1] == "create" && tokens.size() > 3 && is_digit(tokens[2]) && is_digit(tokens[3])) {
						cout << (project->create_sprint(stoi(tokens[2]), stoi(tokens[3]), current_user) ?
							"Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n"
							: "Wrong timeframe, or permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n");
					}
					else if (tokens[1] == "modify" && tokens.size() > 4 && is_digit(tokens[2])) {
						Sprint* modify_sprint = project->get_sprint(stoi(tokens[2]));
						if (modify_sprint == NULL) {
							cout << "Sprint " << tokens[2] << " does not exist (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else if (current_user->get_permission() <= 0) {
							cout << "permission denied (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else if (tokens[3] == "start-time" && is_digit(tokens[4])) {
							modify_sprint->set_start_time(stoi(tokens[4]));
							cout << "Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else if (tokens[3] == "end-time" && is_digit(tokens[4])) {
							modify_sprint->set_end_time(stoi(tokens[4]));
							cout << "Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else if (tokens[3] == "status") {
							modify_sprint->set_status(tokens[4] == "Open");
							cout << "Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
						}
						else {
							cout << "Unsupported command format.\n";
						}
					}
					else {
						cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
					}
				}
				else {
					cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
				}
			}
			// help
			else if (tokens.size() > 0 && tokens[0] == "help") {
				cout << "If you want to enter a text with space inside, for example for a description, you must surround it with quotation marks (\").\n";
				cout << "All '[' and ']' in the following commands should be ignored, as they simply indicate that the surrounded content is a variable.\n";
				cout << "- View Commands:\n";
				cout << "\tTo pull the current status of the project, use 'view status'.\n";
				cout << "\tTo view properties of the project, use 'view project'.\n";
				cout << "\tTo view issues in the selected area, use 'view [area]'.\n";
				cout << "\t\tFor [area], you may choose from 'to-do','in-progress' and 'work-done'.\n";
				cout << "\tTo view an individual issue, use 'view issue [issue-id]'.\n";
				cout << "- Issue Commands:\n";
				cout << "\tTo create a new issue, use 'issue create [issue-name] [issue-type] [issue-description] [issue-priority]'.\n";
				cout << "\tTo join an issue, use 'issue join [issue-id]'.\n";
				cout << "\tTo mark an issue assigned as done, use 'issue finish [issue-id]'.\n";
				cout << "\tTo comment on an issue assigned, use 'issue comment [issue-id] [comment]'.\n";
				cout << "\t(leader/co-leader only) To assign an issue to specific sprint, use 'issue move [issue-id] [sprint-id]'.\n";
				cout << "\t(leader/co-leader only) To modify properties of an issue, use 'issue modify [issue-id] [property-name] [value]'.\n";
				cout << "\t\tFor [property-name], you may choose from 'name','description','priority' and 'status'(Open/In Progress/Done/Overdue).\n";
				cout << "\t(leader/co-leader only) To assign a user to a specific issue, use 'issue assign [issue-id] [user-name]'.\n";
				cout << "\t(leader/co-leader only) To delete an issue, use 'issue delete [issue-id]'.\n";
				cout << "- Project Commands:\n";
				cout << "\t(leader/co-leader only) To modify properties of the project, use 'project modify [property-name] [value]'.\n";
				cout << "\t\tFor [property-name], you may choose from 'name','description' and 'deadline'.\n";
				cout << "\t(project owner only) To delete this project, use 'project delete'.\n";
				cout << "- User Commands:\n";
				cout << "\t(leader/co-leader only) To invite a user to collaborate, use 'user invite [user-name] [user-title] [permission-level]'.\n";
				cout << "\t\tFor [permission-level], it determine the user's operation permission, 0 means normal user, 1 means project leader/co-leader, and 2 means project owner.\n";
				cout << "\t\t[user-title] does not influence the user's permission level.\n";
				cout << "\t(leader/co-leader only) To assign a different title/permission to an existing user, use 'user assign [user-name] [user-title] [permission-level]'.\n";
				cout << "\t(leader/co-leader only) To remove an existed user, use 'user remove [user-name]'.\n";
				cout << "- Sprint Commands:\n";
				cout << "\t(leader/co-leader only) To create a new sprint, use 'sprint create [start-time] [end-time]'.\n";
				cout << "\t(leader/co-leader only) To modify properties of a sprint, use 'sprint modify [sprint-id] [property-name] [value]'.\n";
				cout << "\t\tFor [property-name], you may choose from 'start-time','end-time' and 'status'(Closed/Open).\n";
				cout << "- Other Commands:\n";
				cout << "\tTo quit, use 'quit'.\n";
				cout << "\tTo switch user, use 'switch [user-name]'.\n";
			}
			else if (tokens.size() > 1 && tokens[0] == "switch") {
				User* temp_user = project->get_member(tokens[1]);
				if (temp_user == NULL) {
					cout << "User not found in the project. (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
				}
				else {
					current_user = temp_user;
					cout << "Process succeeds (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
				}
			}
			else if (command != "quit ") {
				cout << "Unsupported command format (current time : " + std::to_string(project->get_current_time() + add_time) + ").\n";
			}
			// simulate time
			project->simulate_time(add_time);
		}
		// save project into file
		ofstream out_file("project.txt");
		out_file << project->to_string();
		out_file.close();
	}
	// save project into file
	if (project == NULL) {
		remove("project.txt");
	}
	else {
		ofstream out_file("project.txt");
		out_file << project->to_string();
		out_file.close();
	}
	delete project;
	return 0;
}
// helper functions
int get_output_integer() {
	string value;
	getline(cin, value);
	if (!is_digit(value)) {
		return get_output_integer();
	}
	return stoi(value);
}
bool is_digit(string value) {
	for (char const& c : value) {
		if (std::isdigit(c) == 0) {
			return false;
		}
	}
	return true;
}
int testIO() {
	Project* project = new Project("Final Project", "The last cs3520 hw.", 10000);
	project->invite("Alex", "Manager", 1, NULL);
	User* alex = project->get_member("Alex");
	project->assign_role("Alex", "Manager", 2, alex);
	project->simulate_time(3);
	project->invite("Steve", "Designer", 0, alex);
	User* steve = project->get_member("Steve");
	project->create_sprint(3, 100, alex);
	cout << "Test Project IO 1 : " << (project->to_string() == (new Project(project->to_string()))->to_string() ? "Pass" : "Fail") << endl;
	project->create_issue("documentation", "Start", "start all tasks", 100, alex);
	project->join_issue(1, alex);
	project->join_issue(1, steve);
	project->comment_issue(1, "Let's go", alex);
	project->add_to_sprint(1, 1, alex);
	project->simulate_time(20);
	cout << "Test Project IO 2 : " << (project->to_string() == (new Project(project->to_string()))->to_string() ? "Pass" : "Fail") << endl;
	project->comment_issue(1, "Bad idea", steve);
	project->create_issue("documentation", "Restart", "remove all tasks", 20, steve);
	project->join_issue(2, steve);
	project->simulate_time(10);
	project->add_to_sprint(2, 1, alex);
	project->simulate_time(50);
	project->finish_issue(2, alex);
	cout << "Test Project IO 3 : " << (project->to_string() == (new Project(project->to_string()))->to_string() ? "Pass" : "Fail") << endl;
	project->create_sprint(400, 800, steve);
	project->create_issue("build", "Build system", "work around the basic system", 200, alex);
	project->join_issue(2, alex);
	project->comment_issue(2, "may finish next week", alex);
	project->add_to_sprint(3, 2, alex);
	project->delete_issue(2, alex);
	project->remove_member("Steve", alex);
	cout << "Test Project IO 4 : " << (project->to_string() == (new Project(project->to_string()))->to_string() ? "Pass" : "Fail") << endl;
	cout << "_____________________________________________\n\n";
	cout << project->to_string();
	delete project;
	return 0;
}