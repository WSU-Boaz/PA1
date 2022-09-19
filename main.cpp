// code source: https://www.codesdope.com/blog/article/c-linked-lists-in-c-singly-linked-list/

/*******************************************************************************************

* Programmer: Boaz Glassberg                                                               *

* Class: CptS 223                                                                          *

* Programming Assignment: PA1                                                              *

* Date:   9/14/2022                                                                        *

* Description:  This program plays a linux command matching game                           *

*******************************************************************************************/

#include "Header.hpp"

/* 

ADVANTAGES/DISADVANTAGES LINKED LIST:

1 advantage: The list was extremely easy to manipulate. Accessing and especially updating nodes was much easier than tracking an array through many different functions 
1 disadvantage: The list was difficult to traverse, you had to create node pointers and manually scout through it which is pretty inefficient

ADVANTAGES/DISADVANTAGES ARRAY:

1 advantage: The array by contrast was really easy to search through. Finding a specific profile was quick and easy, especially if you already had the index
2 advantages: The array was hard to track throughout multiple functions. Changing a players score required that I jumped through some awkward hoops I wouldn't have to deal with if I was just using node pointers

*/

int main()
{
	srand(time(NULL));
	main_menu();
    return 0;
}


/*
To do list:

CMake/Github

*/