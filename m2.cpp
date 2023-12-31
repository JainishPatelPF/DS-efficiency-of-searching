/*
* FILE : m2.cpp
* PROJECT : SENG1050 - Major 2
* PROGRAMMER : Jainish Prakash Patel
* FIRST VERSION : 08-07-2022
* DESCRIPTION :
* Compare the efficiency of searching a sorted linked list and a hash table.
*/
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#pragma warning(disable:4996)
#pragma warning(disable:6001)

#define LENGTH 21
#define TABLE_SIZE 127


struct WordNode
{
	char value[LENGTH];
	struct WordNode* next;
};

int checkString(char str[]); //This function takes string as a parameter and returns 1 or 0 as a result.
void clearEndOfLine(char* buff); //This function takes the string as a parameter and clears the end of the line.
unsigned long djb2(char* str); //This function returns the hash value and a string is taken as a parameter.
int myHashFunction(char*); //This function takes a string as a parameter and gets a index value for the hash table to store the string.
WordNode* enterNewInfo(WordNode* newHead, char* str);//This function takes the head pointer as a array element and a string to save it to a new node and sort it.
void freeList(WordNode* head);//This function takes the head as a parameter and frees the list. 
WordNode* searchLinkedList(char * searchName, WordNode* linkedList, int* comparisonCount);//This function searches through the linked list for a string.
void searchForNameTwice(char* searchName,WordNode* linkedList,WordNode* hashTable, int comparisonCount[2]);//this function is used to search the string in the hash table as well as the sorted linked list.

int main()
{
	char str1[LENGTH] = "";
	char str2[LENGTH] = "";
	unsigned int value = 0;
	struct WordNode arr[TABLE_SIZE] = { NULL };
	struct WordNode* ptr = NULL;
	int i = 0;
	FILE* read_line = NULL;
	int count[2] = { 0 };
	int search_count = 0;

	while (1)
	{
		read_line = fopen("names.txt", "r+"); //opens the file
		if (read_line == NULL)
		{
			return 1;
		}
		while (fgets(str1, LENGTH, read_line)) //gets the string line by line
		{
			clearEndOfLine(str1);//clears the end of the line.
			ptr = enterNewInfo(ptr, str1);
			value = myHashFunction(str1);//returns a index value for an array.
			for (i = 0; i <= TABLE_SIZE; i++)
			{
				if (i == value)
				{
					enterNewInfo(&arr[i], str1);//creates a new node and sorts it in a linked list.
				}
			}
		}
		if (fclose(read_line) != 0) //closes the file
		{
			printf("\nError reading file.\n");
		}
		break;
	}
	i = 0;
	printf("Search for Names. Press '.' to exit.\n");
	while (1)
	{
		search_count = i;
		
		fgets(str2, LENGTH, stdin); //gets the value from the user.
		clearEndOfLine(str2); //clears end of the line.
		if (checkString(str2) == 1) // checks the string if there is a '.'.
		{
			break;
		}
		searchForNameTwice(str2, ptr, arr, count); //this function call is used to search for the hash buckets and number of linked lists searched to the desired string.
		i++;
	}
	
	printf("\tTotal Linked Lists Count: %d\n", count[0]);
	printf("\tTotal Hash Table Buckets Count:%d\n", count[1]);
	printf("\tTotal Searches Count:%d\n", search_count);
	
	for (i = 0; i < TABLE_SIZE; i++)
	{
		freeList(&arr[i]);//frees all the element of an array and each nodes in a linked list.
	}

	return 0;
}

/* FUNCTION		: searchForNameTwice()
* DESCRIPTION   : this function takes 4 parameters and it is used to search the hash table as well as the very long sorted linked list.
* PARAMETERS    : char* searchName, WordNode* linkedList, WordNode* hashTable, int comparisonCount[2]
* RETURNS		: nothing
*/
void searchForNameTwice(char* searchName,WordNode* linkedList,WordNode* hashTable, int comparisonCount[2])
{
	int value = 0;
	int i = 0;
	int count[2] = { 0 };
	char s_name[LENGTH] = "";
	strcpy(s_name, searchName);
	WordNode* Return_node = NULL;
	WordNode* Return_Bucket_node = NULL;

	value = myHashFunction(searchName); //calculates the index value of the hash table.
	for (i = 0; i <= TABLE_SIZE; i++)
	{
		if (i == value)
		{	
			Return_Bucket_node = searchLinkedList(s_name, &hashTable[i], &count[1]); //search for hash table bucket.
			count[1] = i + 1;
			if (Return_Bucket_node != NULL)
			{
				printf("\t%s was found in the hash table bucket in %d comparisons.\n", searchName, count[1]);
			}
			else
			{
				printf("\t%s was NOT found in the hash table bucket in %d comparisons.\n", searchName, count[1]);
			}
		}
	}
	comparisonCount[1] = comparisonCount[1] + count[1];
	Return_node = searchLinkedList(s_name, linkedList, &count[0]); //search for linked list count.
	if (Return_node != NULL)
	{
		printf("\t%s was found in the linked list in %d comparisons.\n", searchName, count[0]);
	}
	else
	{
		printf("\t%s was NOT found in the linked list in %d comparisons.\n", searchName, count[0]);
	}
	comparisonCount[0] = comparisonCount[0] + count[0];
}

/* FUNCTION		: searchLinkedList()
* DESCRIPTION   : this function takes 3 parameters and it is used to search the sorted linked list and sets the node to null if the match is not found.
* PARAMETERS    : char* searchName, WordNode* linkedList, int* comparisonCount
* RETURNS		: WordNode pointer
*/
WordNode* searchLinkedList(char* searchName, WordNode* linkedList, int* comparisonCount)
{
	WordNode* ptr = linkedList;
	int i = 0;

	ptr = ptr->next;
	
	for (i = 0; ptr != NULL; ptr = ptr->next)
	{
		comparisonCount[0] = i + 1;
		if (strcmp(ptr->value, searchName) == 0) //compares the string with the search variable
		{
			break;
		}
		i++;
	}
	if (ptr->value == NULL)
	{
		ptr = NULL;
	}

	return ptr;
}

/* FUNCTION		: enterNewInfo()
* DESCRIPTION   : This function creates a new node and sorts the node into the linked list.
* PARAMETERS    : WordNode* newHead, char* str
* RETURNS		: WordNode pointer
*/

WordNode* enterNewInfo(WordNode* newHead, char* str)
{
	WordNode* newBlock = NULL;
	WordNode* ptr = NULL;
	WordNode* prev = NULL;
	newBlock = (WordNode*)malloc(sizeof(WordNode));
	if (newBlock == NULL)
	{
		printf("Error! No more memory\n");
		return newHead;
	}

	strcpy(newBlock->value, str);
	newBlock->next = NULL;

	if (newHead == NULL)
	{
		newHead = newBlock;
	}
	else if (strcmp(newHead->value, newBlock->value) >= 0)
	{
		newBlock->next = newHead;
		newHead = newBlock;
	}
	else
	{
		prev = newHead;
		ptr = newHead->next;

		while (ptr != NULL)
		{
			if (strcmp(ptr->value, newBlock->value) >= 0)
			{
				break;
			}
			prev = ptr;
			ptr = ptr->next;
		}
		newBlock->next = ptr;
		prev->next = newBlock;
	}
	return newHead;
}

/* FUNCTION		: myHashFunction()
* DESCRIPTION   : This function is used to calculate the index value of the given hash value by djb2() using division method.
* PARAMETERS    : char* element
* RETURNS		: int
*/
int myHashFunction(char* element)
{
	unsigned int hash_Value = 0;
	unsigned int result = 0;
	hash_Value = djb2(element);
	result = (hash_Value % TABLE_SIZE);
	return result;
}

/*
* Function Name: djb2
* Created by: Dan Bernstein
* Found at: week 5 slides in Data Structures module in Software Engineering Technology Course at Conestoga College.
*/
unsigned long djb2(char* str)
{
	unsigned long hash = 5381;
	int c = 0;
	while ((c = *str++) != '\0')
	{
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

/* FUNCTION		: checkString()
* DESCRIPTION   : This Function is used to check the string whether it has a '.' in the string.
* PARAMETERS    : char str[]
* RETURNS		: int
*/

int checkString(char str[])
{
	if (strcmp(str, ".") == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* FUNCTION		: clearEndOfLine()
* DESCRIPTION   : This function clears the end of the line of a string taken by fgets.
* PARAMETERS    : char* buff
* RETURNS		: nothing
*/
void clearEndOfLine(char* buff)
{
	char* temp = strchr(buff, '\n');
	if (temp != NULL)
	{
		*temp = '\0';
	}
}

/* FUNCTION		: clearEndOfLine()
* DESCRIPTION   : This function frees the dynamically allocated memory.
* PARAMETERS    : WordNode* head
* RETURNS		: nothing
*/

void freeList(WordNode* head)
{
	WordNode* temp = NULL;
	WordNode* ptr = head->next;
	while (ptr != NULL)
	{
		temp = ptr;
		ptr = ptr->next;
		free(temp);
		break;
	}
}