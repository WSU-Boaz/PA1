/*******************************************************************************************

* Programmer: Boaz Glassberg                                                               *

* Class: CptS 223                                                                          *

* Programming Assignment: PA1                                                              *

* Date:   9/14/2022                                                                        *

* Description:  This program plays a linux command matching game                           *

*******************************************************************************************/

#include "Header.hpp"

void main_menu()
{
	int selection = 0;
	profile player;
	linked_list<string, string> cList; //linked list
	string cmdName, cmdDes;
	fstream infile("commands.csv");
	int size = cList.buildList(infile, cmdName, cmdDes);

	while (selection != 6)
	{
		selection = printMenu();
		switch (selection)
		{
		case 1:	//Game Rules
			gameRules();
			break;
		case 2: //Play Game
			playGame(cList, infile);
			break;
		case 3: //Load Previous Game
			player = loadGame();
			if (player.name != "NULL") { //checking validity
				playGame(player);
			}
			break;
		case 4: //Add Command
			saveCommand(cList);
			break;
		case 5: //Remove Command
			removeCommand(cList);
			break;
		case 6: //Exit
			break;
		}
		system("clear");

	}
	infile.close();
	return;
}


int printMenu()
{
	int selection = 0;

	cout << "Pick one of the following options: \n\n";
	cout << "1.    Game Rules" << endl;
	cout << "2.    Play Game" << endl;
	cout << "3.    Load Previous Game" << endl;
	cout << "4.    Add Command" << endl;
	cout << "5.    Remove Command" << endl;
	cout << "6.    Exit\n\nSelection: ";

	cin >> selection;
	cout << "\n\n";

	int count = 0;

	while (selection < 1 || selection > 6)
	{
		cout << "Please enter a value between 1 and 6." << endl;
		cout << "Selection: " << endl;
		cin >> selection;
		count++;
		if (count > 10) { //infinite preventation system
			system("clear");
			selection = 1;
		}
	}

	return selection;
}

void gameRules() {
	char c = NULL;
	cout << "The objective of this game is to earn points by correctly choosing the definition of linux commands. You will choose one of three options for each question. Correct guesses will earn you one point, while incorrect guesses will lose you one point. There is no maximum nor minimum point limit. Each player's profile will be saved and can be loaded later." << endl << endl;
	cout << "Press any key to continue" << endl;
	cin >> c;
	return;
}

void playGame(linked_list<string, string> cList, fstream& infile) {

	string cmdName, cmdDes, name;
	int index = 0, size = 0, change = 0;

	profile pList[100];
	buildProfiles(pList);

	cout << "Please enter your name" << endl << endl;

	cin >> name;

	while (index < 100 && pList[index].name != name) { //to prevent overflow errors
		if (pList[index].name.empty()) {
			pList[index].name = name;
			pList[index].points = 0;
			savePlayer(pList[index]);
			index = 100;
		}
		index++;
	}

	if (pList[index].name == name) {
		change = playGameLoop(cList.getHead(), pList, pList[index], size); //playGame helper, essentially
	}
	
	pList[index].points = change; //update player's point
	updatePlayer(pList[index]);

	return;
}

void playGame(profile player) { //overloading playGame in case it's given a player (i.e. load game)

	fstream infile("commands.csv");
	linked_list<string, string> cList;
	int size = 0, change = 0, count = 0;
	string cmdName, cmdDes;
	size = cList.buildList(infile, cmdName, cmdDes);

	profile pList[100];
	buildProfiles(pList);

	change = playGameLoop(cList.getHead(), pList, player, size);

	while (count < 100 && pList[count].name != player.name) { //to update pList
		if (pList[count].name == player.name) {
			pList[count].points = change;
		}
		count++;
	}

	updatePlayer(player);
	
	return;
}

//builds a profile array from the profiles.csv file
void buildProfiles(profile pList[]) {
	string temp;
	int count = 0;
	fstream infile("profiles.csv");
	while (!infile.eof() || count >= 100) {
		getline(infile, pList[count].name, ',');
		getline(infile, temp);
		pList[count].points = stoi(temp);
		count++;
	}
	infile.close();
}

//if the user already has points, load their game
profile loadGame() {
	fstream infile("profiles.csv");
	string name, temp;
	profile player;
	cout << "Please enter your name" << endl << endl;
	cin >> name;

	while (!infile.eof()) {
		getline(infile, temp, ',');
		if (temp == name) {
			player.name = temp;
			getline(infile, temp);
			player.points = stoi(temp);
			cout << "You have " << temp << " points. Press any key to continue" << endl << endl;
			cin >> name;
			return player;
		}
		getline(infile, temp);
	}
	cout << "You do not have a profile, press any key to return to the main menu" << endl << endl;
	cin >> name;
	player.name = "NULL";
	infile.close();
	return player;
}

//save a command to commands.csv
void saveCommand(linked_list<string, string> cList) {
	fstream infile("commands.csv");
	string file = "", tempName, tempDes;

	while (!infile.eof()) { //copies the text file in csv format
		getline(infile, tempName);
		if (!file.empty()) {
			file = file + "\n" + tempName;
		}
		else {
			file = file + tempName;
		}
	}
	infile.close();
	ofstream outfile("commands.csv"); //opens it as an outfile to be written to
	system("clear");
	cout << "What is the title of your command?" << endl << endl;
	cin >> tempName;
	cout << "What is the description of your command?" << endl << endl;
	while (tempDes.length() == 0) {
		getline(cin, tempDes);
	}
	tempDes = "\"" + tempDes + "\""; //make sure the description has quotation marks
	file = file + "\n" + tempName + "," + tempDes; //include the new command
	outfile << file; //rewrite the file
	outfile.close();

	cList.add_node(tempName, tempDes); //add node to linked list
}

//delete a command from commands.csv
void removeCommand(linked_list<string, string> cList) {

	fstream infile("commands.csv");
	string file = "", tempName, tempDes, userName;
	system("clear");
	cout << "What is the title of the command? you want to remove" << endl << endl; //ask user for input
	cin >> userName;

	cList.removeNode(userName); //remove the node from the linked list

	while (!infile.eof()) { //copies the text file in csv format
		getline(infile, tempName, ',');
		if (tempName != userName) { //except for the one the user wants to delete, which is skipped

			if (!file.empty()) {
				file = file + "\n" + tempName;
			}
			else {
				file = file + tempName;
			}
			getline(infile, tempDes);
			file = file + "," + tempDes;
		}
		else {
			getline(infile, tempDes);
		}
	}
	infile.close();
	ofstream outfile("commands.csv"); //opens it as an outfile to be written to

	outfile << file; //rewrite the file, pasting everything except for the deleted command
	outfile.close();
}

//Saves a player's profile to profiles.csv
void savePlayer(profile player) {
	fstream infile("profiles.csv");
	string file = "", temp;
	while (!infile.eof()) { //copies the text file in csv format
		getline(infile, temp);
		if (!file.empty()) {
			file = file + "\n" + temp;
		}
		else {
			file = file + temp;
		}
	}
	infile.close();
	ofstream outfile("profiles.csv"); //opens it as an outfile to be written to
	file = file + "\n" + player.name + "," + to_string(player.points); //include the new player at the end
	outfile << file; //rewrite the file
	outfile.close();
}

void updatePlayer(profile player) {
	fstream infile("profiles.csv");
	string file = "", tempName, tempDes;
	while (!infile.eof()) { //copies the text file in csv format
		getline(infile, tempName, ',');
		if (tempName == player.name) { //Find the name we're looking for
			if (!file.empty()) {
				file = file + "\n" + player.name;
			}
			else {
				file = file + player.name;
			}
			getline(infile, tempDes);
			file = file + "," + to_string(player.points); //update player's score
		}
		else {
			if (!file.empty()) {
				file = file + "\n" + tempName;
			}
			else {
				file = file + tempName;
			}
			getline(infile, tempDes);
			file = file + "," + tempDes;
		}
	}
	infile.close();
	ofstream outfile("profiles.csv"); //opens it as an outfile to be written to
	outfile << file; //rewrite the file
	outfile.close();
}

int playGameLoop(node<string,string>* head, profile pList[], profile player, int index) {
	int rand1, rand2, activeCmd, display, answer; //rand 1 and 2 are the random answers, activeCmd is the command and correct answer's index value. Display is the order that they're displayed in
	node<string, string>* tmp1 = head; //tmp1 is correct answer, tmp2 & 3 are incorrect
	node<string, string>* tmp2 = head;
	node<string, string>* tmp3 = head;
	int count = 0, loopsize = 0;
	system("clear");
	cout << "How many questions would you like to answer?" << endl << endl;
	cin >> loopsize;
	while (count < loopsize) {
		system("clear");
		rand1 = rand() % index;
		rand2 = rand() % index;
		do{
			activeCmd = rand() % index;
		} while (activeCmd == rand1 || activeCmd == rand2);
		tmp1 = head; //reset temp to the beginning
		tmp2 = head;
		tmp3 = head;

		for (int i = 0; i < activeCmd; i++) { //find the commands it's looking for
			tmp1 = tmp1->next;
		}
		for (int i = 0; i < rand1; i++) {
			tmp2 = tmp2->next;
		}
		for (int i = 0; i < rand2; i++) {
			tmp3 = tmp3->next;
		}

		display = rand() % 3 + 1; //display correlates to the position of the correct answer

		cout << tmp1->first << endl << endl;

		rand1 = rand() % 2;

		if (display == 1) {
			cout << "1. " << tmp1->second << endl;
			if (rand1) {
				cout << "2. " << tmp2->second << endl;
				cout << "3. " << tmp3->second << endl;
			}
			else {
				cout << "2. " << tmp3->second << endl;
				cout << "3. " << tmp2->second << endl;
			}
		}
		if (display == 2) {
			if (rand1) {
				cout << "1. " << tmp2->second << endl;
				cout << "2. " << tmp1->second << endl;
				cout << "3. " << tmp3->second << endl;
			}
			if (!rand1) {
				cout << "1. " << tmp3->second << endl;
				cout << "2. " << tmp1->second << endl;
				cout << "3. " << tmp2->second << endl;
			}
		}
		if (display == 3) {
			if (rand1) {
				cout << "1. " << tmp2->second << endl;
				cout << "2. " << tmp3->second << endl;
			}
			else {
				cout << "1. " << tmp3->second << endl;
				cout << "2. " << tmp2->second << endl;
			}
			cout << "3. " << tmp1->second << endl;
		}
			
		cout << "Input the correct answer: ";
		cin >> answer;

		if (answer == display) {
			answer = 0;
			player.points++;
			cout << "Correct! You earn one point. Your point total is now " << to_string(player.points) << endl << "Type any number to continue" << endl;
			while (answer == 0) {
				cin >> answer;
			}
		}
		else {
			answer = 0;
			player.points--;
			cout << "Unlucky, that was incorrect so you lose one point. Your point total is now " << to_string(player.points) << endl << "Type any number to continue" << endl;
			while (answer == 0) {
				cin >> answer;
			}
		}

		count++;
		answer = 0;
		display = 0;
	}

	return player.points;
}

ofstream& operator<< (ofstream& lhs, profile& rhs) {
	lhs << rhs.name << ',' << rhs.points;
	return lhs;
}