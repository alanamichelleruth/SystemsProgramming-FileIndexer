/*
Contains functions to browse directories or files, also helper functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include "constants.h"
#include "prototypes.h"
#include "struct_def.h"
#include "externs.h"

//Prototype for local functions
void push(char *wd, char *fName);
void changeFileName(char *fName);

//Function to browse through directories one at a time, checking for subdirectories with recursion
void browseDirectories(char* directoryName){
  //Declare a DIR pointer variable
  DIR *dir;

  //If given directory cannot be opened, print error message and stop
  if ((dir = opendir(directoryName)) == NULL){
    fprintf(stderr, "Error: Directory %s cannot be opened.\n", directoryName);
    return;
  }

  //Declare a directory entry pointer variable
  struct dirent *dentry;

  //Loop through all files and subdirectories in the directory
  while((dentry = readdir(dir)) != NULL){

    //If it is a directory (not named . or ..)
    if (strchr(dentry->d_name, '.') == NULL && strcmp(dentry->d_name, ".") != 0  && strcmp(dentry->d_name, "..") != 0){
      browseDirectories(dentry->d_name);
    }
    //Otherwise, if it is not names . or .., call browseFile
    else if (strcmp(dentry->d_name, ".") != 0 && strcmp(dentry->d_name, "..") != 0){
      browseFile(dentry->d_name);
    }
  }

  //Close the directory
  closedir(dir);
}

//Function to browse through a file by separators (any non numerical/alphabetical char)
void browseFile(char* fileName){
  //Declare a FILE pointer variable
  FILE *fp;

  //If given file cannot be opened, print error message and stop
  if((fp = fopen(fileName, "a+"))== NULL){
    fprintf(stderr, "Error: File %s cannot be opened.\n", fileName);
    return;
  }

  //Change the file name (only does if needed)
  changeFileName(fileName);

  //Add a space to the end of the file, then rewind
  /*This is done so that in the case that the last char in the file
    is alphanumeric, the final word will still be counted*/
  fputs(" ", fp);
  rewind(fp);

  //Declare int variables to represent each char in file
  int c;
  //Declare char pointer to represent each word
  char *w = NULL;

  //Declare a conditional int, TRUE if within word of alphanumeric, FALSE otherwise
  int inWord = FALSE;

  //Loop through all the characters in the file, one by one
  while((c = fgetc(fp)) != EOF){

    //If the character is an alphabet or a digit, append it to the word in lowercase
    if (!(isalpha(c) == 0) || !(isdigit(c) == 0)){
      inWord = TRUE;

      //Adding the char to the word
      if(w == NULL) {
        w = malloc(sizeof(char)*2);
        w[0] = tolower(c);
        w[1] = '\0';
      }else{
        int len = strlen(w);
        w = realloc(w, sizeof(char)*(len+1));
        w[len] = tolower(c);
        w[len+1] = '\0';
      }

    }
    //Otherwise, reset the char pointer to be empty
    else{
      //If word just ended
      if (inWord){
        //Reset inWord to be false
        inWord = FALSE;

        //Push the word to the linked list with the fileName
        push(w, fileName);
        free(w);
        w = NULL;
      }
    }
  }

  //Close the file
  fclose(fp);
}

//Function to push the word to the linked list with the given fileName
void push(char *wd, char *fName){

  //If the list is empty
  if (h == NULL){
    //Declare newNode pointer and newFileNode pointer and check malloc
    pnode_t newNode;
    pfile_t newFileNode;
    if((newNode = checkMalloc(sizeof(node_t))) == NULL)
      return;
    if((newFileNode = checkMalloc(sizeof(file_t))) == NULL)
      return;

    //Initialize the newNode to have the word
    newNode->word = malloc(sizeof(char)*(strlen(wd)+1));
    strcpy(newNode->word, wd);
    newNode->word[strlen(wd)] = '\0';
    newNode->next = NULL;

    //Initialize the newFileNode to have fName and count = 1
    newFileNode->fileName = malloc(sizeof(char)*(strlen(fName)+1));
    strcpy(newFileNode->fileName, fName);
    newFileNode->fileName[strlen(fName)] = '\0';
    newFileNode->count = 1;
    newFileNode->nextFile = NULL;
    newNode->firstFile = newFileNode;

    //Set h = newNode
    h = newNode;

    //Stop
    return;
  }

  //If the list is not empty

  //Set current node to be head
  pnode_t curr = h;

  //Declare current file node
  pfile_t currFile;

  //Iterate through the nodes
  while (curr != NULL){

    //If the word exists
  if(strcmp(curr->word, wd) == 0){

      //Iterate through the fileNames in the nodes
      currFile = curr->firstFile;
      while (currFile != NULL){

        //If the file exists, increment its count and stop
        if(strcmp(currFile->fileName, fName) == 0){

          currFile->count++;
          return;
        }

        //Go to the next file
        currFile = currFile->nextFile;
      }

      //If the file does not exist, add it to the end, set its count = 1, and stop
      //Declare newFileNode pointer and check malloc
      pfile_t newFileNode;
      if((newFileNode = checkMalloc(sizeof(file_t))) == NULL)
       return;

      //Initialize the newFileNode to have fName and count = 1
      newFileNode->fileName = malloc(sizeof(char)*(strlen(fName)+1));
      strcpy(newFileNode->fileName, fName);
      newFileNode->fileName[strlen(fName)] = '\0';
      newFileNode->count = 1;
      newFileNode->nextFile = NULL;


      //trial file to run through the linked list again
      pfile_t trialFile = curr->firstFile;
      while (trialFile->nextFile != NULL){
        trialFile = trialFile->nextFile;
      }

      //Set the end file to be the newFileNode
      trialFile->nextFile = newFileNode;

      //Stop
      return;
    //Go to the next node
    curr = curr->next;
  }

  //If the word doesn't exist through the entire list

  //Declare newNode pointer and newFileNode pointer and check malloc
  pnode_t newNode;
  pfile_t newFileNode;
  if((newNode = checkMalloc(sizeof(node_t))) == NULL)
    return;
  if((newFileNode = checkMalloc(sizeof(file_t))) == NULL)
    return;

  //Initialize the newNode to have the word
  newNode->word = malloc(sizeof(char)*(strlen(wd)+1));
  strcpy(newNode->word, wd);
  newNode->word[strlen(wd)] = '\0';
  newNode->next = NULL;

  //Initialize the newFileNode to have fName and count = 1
  newFileNode->fileName = malloc(sizeof(char)*(strlen(fName)+1));
  strcpy(newFileNode->fileName, fName);
  newFileNode->fileName[strlen(fName)] = '\0';
  newFileNode->count = 1;
  newFileNode->nextFile = NULL;
  newNode->firstFile = newFileNode;

  // trial node to run through the linked list again
  pnode_t trial = h;
  while (trial->next != NULL){
    trial = trial->next;
  }

  //Set the end node to the newNode
  trial->next = newNode;

  //Stop
  return;
}

//If the given file name already exists in the list, append an appropriate number to the end of the file name
void changeFileName(char *fName){

  //Set current node to be head
  pnode_t curr = h;
  //Declare current file node
  pfile_t currFile;

  //Initialize numFile to be 0
  int numFiles = 1;

  //Iterate through the nodes
  while(curr != NULL){

    //Iterate through the fileNames in the nodes
    currFile = curr->firstFile;
    while (currFile != NULL){
      //If the file name exists, increment numFile counter
      if (strcmp(currFile->fileName, fName) == 0)
        numFiles++;

      //Go to the next file
      currFile = currFile->nextFile;
    }
    //Go to the next node
    curr = curr->next;
  }

  //If the file already exists, append its number to the name
  if(numFiles > 1){
    //Adding the char to the file name
    int len = strlen(fName);
    fName = realloc(fName, sizeof(char)*(len+1));
    fName[len] = (numFiles + '0');
    fName[len+1] = '\0';
  }
}


//Wrapper function for malloc checking
void *checkMalloc(size_t numBytes){

  //Allocate memory for the buffer
  void *buffer = malloc(numBytes);

  //If malloc fails, print error
  if (buffer == NULL)
    fprintf(stderr, "Error: Memory allocation error.\n");

  return buffer;
}

//Wrapper function for free checking
void freeMem(void *p){

  //Free the pointer and set it to null
  free(p);
  p = NULL;
}

