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
void searchInfo(WordNode* head, char*);//This function takes the head of the array as a parameter and a string and searches for the given string in the linked list.
void freeList(WordNode* head);//This function takes the head as a parameter and frees the list. 
WordNode* searchLinkedList(char * searchName, WordNode* linkedList, int* comparisonCount);
void searchForNameTwice(char* searchName,WordNode* linkedList,WordNode* hashTable, int comparisonCount[2]);

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

	while (1)
	{
		read_line = fopen("names.txt", "r+");
		if (read_line == NULL)
		{
			return 1;
		}
		while (fgets(str1, LENGTH, read_line))
		{
			clearEndOfLine(str1);//clears the end of the line.
			value = myHashFunction(str1);//returns a index value for an array.
			for (i = 0; i <= TABLE_SIZE; i++)
			{
				if (i == value)
				{
					enterNewInfo(&arr[i], str1);//creates a new node and sorts it in a linked list.
				}
			}
		}
		fclose(read_line);
		break;
	}
	while (1)
	{
		printf("\nEnter the String to search for: \n");
		fgets(str2, LENGTH, stdin);
		clearEndOfLine(str2);
		if (checkString(str2) == 1)
		{
			break;
		}
		searchForNameTwice(str2, ptr, arr, count);
		
	}
	
	
	for (i = 0; i < TABLE_SIZE; i++)
	{
		freeList(&arr[i]);//frees all the element of an array and each nodes in a linked list.
	}

	return 0;
}

void searchForNameTwice(char* searchName,WordNode* linkedList,WordNode* hashTable, int comparisonCount[2])
{
	int value = 0;
	int i = 0;
	int count[2] = { 0 };
	char s_name[LENGTH] = "";
	strcpy(s_name, searchName);

	value = myHashFunction(searchName);
	for (i = 0; i <= TABLE_SIZE; i++)
	{
		if (i == value)
		{
			count[1] = i+1;
			linkedList = searchLinkedList(s_name, &hashTable[i], comparisonCount);
			printf("\n Bucket count:%d ", count[1]);
		}
		
	}
	comparisonCount[1] = comparisonCount[1] + count[1];
	printf("\n Total Bucket count:%d && Total Linked List Count: %d", comparisonCount[1], comparisonCount[0]);
}

WordNode* searchLinkedList(char* searchName, WordNode* linkedList, int* comparisonCount)
{
	WordNode* ptr = linkedList;
	int count[2] = { 0 };
	int i = 0;

	ptr = ptr->next;
	
	for (i = 0; ptr != NULL; ptr = ptr->next)
	{
		count[0] = i + 1;
		if (strcmp(ptr->value, searchName) == 0)
		{
			printf("\n%s was found in the linked list in %d comparisons\n", searchName, count[0]);
			break;
		}
		i++;
	}
	if (ptr->value == NULL)
	{
		printf("\n%s was NOT found in the linked list in %d comparisons\n", searchName, count[0]);
	}
	comparisonCount[0] = comparisonCount[0] + count[0];
	return ptr;
}

//This function creates a new node and sorts the node into the linked list.
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

//This function is used to search the given string into a linked list.
void searchInfo(WordNode* head, char* data)
{
	WordNode* ptr = head;
	ptr = ptr->next;
	if (ptr == NULL)
	{
		printf("Not there!\n");
	}
	for (; ptr != NULL; ptr = ptr->next)
	{
		if (strcmp(ptr->value, data) == 0)
		{
			printf("%s\n", ptr->value);
			printf("Success!\n");
			break;
		}
	}

}
//This function is used to calculate the index value of the given hash value by djb2() using division method.
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

//This Function is used to check the string whether it has a '.' in the string.
int checkString(char str[])
{
	if (strchr(str, '.'))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//This function clears the end of the line of a string taken by fgets.
void clearEndOfLine(char* buff)
{
	char* temp = strchr(buff, '\n');
	if (temp != NULL)
	{
		*temp = '\0';
	}
}

//This function frees the dynamically allocated memory.
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