/*
Contains function that creates the index
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "prototypes.h"
#include "struct_def.h"
#include "externs.h"

//Local function prototypes
void insertionSort();
void sortedInsert(pnode_t *head_ref, pnode_t newNode);
void insertionSortFiles();
void sortedInsertFiles(pfile_t *headFile, pfile_t newFile);

//Function to alphabetize the words in all the files, then sort by count per file
//Right now, just prints info to the given fileName
void indexGenerator(char *fileName){

  //Declare a FILE pointer variable
  FILE *fp;

  //If given file cannot be opened, print error message and stop
  if((fp = fopen(fileName, "w"))== NULL){
    fprintf(stderr, "Error: File %s cannot be opened.\n", fileName);
    return;
  }

  //If the list is empty, print so to stderr and stop
  if (h == NULL){
    fprintf(stderr, "The list is empty.\n");
    return;
  }

  //Sort the nodes
  insertionSort();
  insertionSortFiles();

  //Print the entire sorted list to the output file
  pfile_t currFile;

  while (h != NULL){

    fprintf(fp, "<list> %s\n", h->word);

    currFile = h->firstFile;

    while(currFile != NULL){
      fprintf(fp, "%s %d ", currFile->fileName, currFile->count);

      currFile = currFile->nextFile;
    }

    fprintf(fp, "\n</list>\n");
    h = h->next;
  }
}

//Function to sort the linked list with insertion sort
void insertionSort(){

  //Initialize sorted linked list
  pnode_t sorted = NULL;
  //Current pointer
  pnode_t curr = h;

  //Loop through the nodes
  while(curr != NULL){

    pnode_t next = curr->next;

    //Insert current in sorted linked list
    sortedInsert(&sorted, curr);

    curr = next;
  }

  //Update head to point to sorted list
  h = sorted;
}

//Function to insert a new node into the list (similar to push)
void sortedInsert(pnode_t *newHead, pnode_t newNode){

  //If the head is null or the new word is less than the head's
  if(*newHead == NULL || (strcmp((*newHead)->word, newNode->word) > 0)){

    //Insert the node at the beginning
    newNode->next = *newHead;
    *newHead = newNode;
  }
  else{
    //Locate the node before the point of insertion
    pnode_t curr = *newHead;
    while(curr->next != NULL && (strcmp(curr->next->word, newNode->word) < 0))
      curr = curr->next;
    //Insert the node at the right spot
    newNode->next = curr->next;
    curr->next = newNode;
  }
}

//Function to sort the files within each node
void insertionSortFiles(){

  //Current pointer to node and file
  pnode_t curr = h;
  pfile_t currFile;

  //Loop through the nodes
  while(curr != NULL){

    pnode_t next = curr->next;
    currFile = curr->firstFile;

    pfile_t sortedFiles = NULL;

    //Loop through inner nodes, insert each file in count sorted order
    while(currFile!=NULL){
      pfile_t nextFile = currFile->nextFile;
      sortedInsertFiles(&sortedFiles, currFile);
      currFile = nextFile;
    }

    curr->firstFile = sortedFiles;

    curr = next;
  }
}

//Function to insert the file into the correct order in a list
void sortedInsertFiles(pfile_t *headFile, pfile_t newFile){

  //Insert at head
  if(*headFile == NULL || (*headFile)->count <= newFile->count){
    newFile->nextFile = *headFile;
    *headFile = newFile;
 }
  //Insert in correct spot
  else{
    pfile_t currFile = *headFile;
    while (currFile->nextFile != NULL){
      if (currFile->nextFile->count > newFile->count)
        currFile = currFile->nextFile;
      else
        break;
    }

    newFile->nextFile = currFile->nextFile;
    currFile->nextFile = newFile;
  }
}


