#pragma once
/*******************************************************************************************

* Programmer: Boaz Glassberg                                                               *

* Class: CptS 223                                                                          *

* Programming Assignment: PA1                                                              *

* Date:   9/14/2022                                                                        *

* Description:  This program plays a linux command matching game                           *

*******************************************************************************************/

//guard code
#define HEADER_H

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <stdlib.h> 
#include <fstream>
#include <time.h>

using namespace std;
using std::string;
using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::getline;

struct profile {
	string name;
	int points;
};

template <class T1,class T2>
struct node
{
	T1 first;
	T2 second;
	node* next;
};

template <class T1,class T2>
class linked_list
{
private:
	node<T1, T2>* head, * tail;

	void destroyListHelper(node<T1, T2>* tmp)
	{
		if (tmp != nullptr)
		{
			destroyListHelper(tmp->next);
			delete tmp;
		}
	}

	void removeNodeHelper(node<T1, T2>* tmp) {
		node<T1, T2>* ptr = head;
		node<T1, T2>* pLast = head;

		while (ptr != nullptr && ptr != tmp) {
			pLast = ptr;
			ptr = ptr->next;
		}
		pLast->next = ptr->next;
		delete ptr;
		return;
	}

public:
	linked_list()
	{
		head = NULL;
		tail = NULL;
	}

	void add_node(T1 first, T2 second)
	{
		node<T1, T2>* tmp = new node<T1, T2>;
		tmp->first = first;
		tmp->second = second;
		tmp->next = NULL;

		if (head == NULL)
		{
			insertAtFront(tmp);
		}
		else
		{
			tail->next = tmp;
			tail = tail->next;
		}
	}

	void insertAtFront(node<T1, T2>* tmp) {
		if (tmp != nullptr) {
			tmp->next = head;
			head = tmp->next;
		}
	}

	void removeNode(T1 first) {
		node<T1, T2>* tmp = head;
		while (tmp != nullptr) {
			if (tmp->first != first) {
				tmp = tmp->next;
			}
			else {
				removeNodeHelper(tmp);
			}
		}
	}

	node<T1,T2>* getHead() {
		return head;
	}
	node<T1, T2>* getTail() {
		return tail;
	}

	~linked_list() {
		this->destroyList();
	}

	void destroyList()
	{
		destroyListHelper(this->head);
	}

	//the command name is first, and the command description is second in this case
	int buildList(fstream& infile, T1 first, T2 second) {
		int size = 0;
		while (!infile.eof()) {

			getline(infile, first, ',');
			getline(infile, second);

			this->add_node(first, second);
			size++;
		}
		return size;

	}
};

void main_menu();
int printMenu();
void gameRules();
void playGame(linked_list<string, string> cList, fstream& infile);
void playGame(profile player);
void buildProfiles(profile pList[]);
profile loadGame();
void saveCommand(linked_list<string, string> cList);
void removeCommand(linked_list<string, string> cList);
void savePlayer(profile player);
void updatePlayer(profile player);
int playGameLoop(node<string, string>* head, profile pList[], profile player, int index);

ofstream& operator<< (ofstream& lhs, profile& rhs);