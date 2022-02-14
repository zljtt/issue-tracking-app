Updated 2021/12/15

---------------------------
HOW TO USE/TEST THE PROGRAM
---------------------------
If the user start the program with no file in default directory named "project.txt", it will prompt user to finish the creation of a new project, and allow the user to be the project owner.
If the user start the program with a "project.txt" in the directory (most likely the product of runing this program before), it will prompt the user to log in with a user name, which is either the project owner's name entered in the project creation, or the user that is invited by the project owner.

After creating project / log in, the project will display the Current Status of the project, and prompt the user to enter commands.
The operation of the program is based on COMMANDS. By entering commands, user are able to view/modify the project.

Here are some example commands:
- view project
	- means displaying the basic information about the project.
- issue create Bug12 debug "a new bug to fix" 10
	- means creating a new debug issue named Bug12 with description "a new bug to fix" and priority 10, and put it into to-do area.
- sprint create 100 200
	- means creating a new sprint start from day 100 to day 200.
- user invite Ben designer 0
	- means inviting Ben to the project with a title of designer, and with permission level 0;
	- after that, Ben is able to log in with the username "Ben".
PS:
Different users have different permission level, which resulting in different results of different command operations. 
It is very obvious with 'view project' command, project leader can see all information of the project, while normal users can only see the minor information that are related to them.
If you want to enter a text with space inside, for example for a description, you must surround it with quotation marks (")

Here is the more specific explanation, the user may also access it in the program by entering 'help':
All '[' and ']' in the following commands should be ignored, as they simply indicate that the surrounded content is a variable.
- View Commands:
	To pull the current status of the project, use 'view status'.
	To view properties of the project, use 'view project'.
	To view issues in the selected area, use 'view [area]'.
		For [area], you may choose from 'to-do','in-progress' and 'work-done'.
	To view an individual issue, use 'view issue [issue-id]'.
- Issue Commands:
	To create a new issue, use 'issue create [issue-name] [issue-type] [issue-description] [issue-priority]'.
	To join an issue, use 'issue join [issue-id]'.
	To mark an issue assigned as done, use 'issue finish [issue-id]'.
	To comment on an issue assigned, use 'issue comment [issue-id] [comment]'.
	(leader/co-leader only) To assign an issue to specific sprint, use 'issue move [issue-id] [sprint-id]'.
	(leader/co-leader only) To modify properties of an issue, use 'issue modify [issue-id] [property-name] [value]'.
		For [property-name], you may choose from 'name','description','priority' and 'status'(Open/In Progress/Done/Overdue).
	(leader/co-leader only) To assign a user to a specific issue, use 'issue assign [issue-id] [user-name]'.
	(leader/co-leader only) To delete an issue, use 'issue delete [issue-id]'.
- Project Commands:
	(leader/co-leader only) To modify properties of the project, use 'project modify [property-name] [value]'.
		For [property-name], you may choose from 'name','description' and 'deadline'.
	(project owner only) To delete this project, use 'project delete'.
- User Commands:
	(leader/co-leader only) To invite a user to collaborate, use 'user invite [user-name] [user-title] [permission-level]'.
		For [permission-level], it determine the user's operation permission, 0 means normal user, 1 means project leader/co-leader, and 2 means project owner.
		[user-title] does not influence the user's permission level.
	(leader/co-leader only) To assign a different title/permission to an existing user, use 'user assign [user-name] [user-title] [permission-level]'.
	(leader/co-leader only) To remove an existed user, use 'user remove [user-name]'.
- Sprint Commands:
	(leader/co-leader only) To create a new sprint, use 'sprint create [start-time] [end-time]'.
	(leader/co-leader only) To modify properties of a sprint, use 'sprint modify [sprint-id] [property-name] [value]'.
		For [property-name], you may choose from 'start-time','end-time' and 'status'(Closed/Open).
- Other Commands:
	To quit, use 'quit'.
	To switch user, use 'switch [user-name]'.