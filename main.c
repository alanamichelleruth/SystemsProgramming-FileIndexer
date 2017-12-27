/*
Main worker method, calls browse functions and index functions directly
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //used for getcwd command
#include "prototypes.h"
#include "struct_def.h"
#include "globals.h"

int main(int argc, char *argv[]){

  //Delcare char array to hold directory name
  char directoryName[1024];
  //Making sure to have a '\0' at the end of the directory name
  memset(directoryName, '\0', 1024);

  //Initialize the head of the linked list to be null
  h = NULL;

  //If there are an invalid number of commands, print error and stop
  if (argc > 3){
    fprintf(stderr, "Error: Invalid number of arguments.\n" );
    return 0;
  }

  //If there are no arguments, work on current directory and generate the file "invind.txt"
  if (argc == 1){

    //If current working directory cannot be received, print error and stop
    if(getcwd(directoryName, sizeof(directoryName))==NULL){
      fprintf(stderr, "Error: Current working directory cannot be accessed.\n");
      return 0;
    }
    browseDirectories(directoryName);
    //Generate index file
    indexGenerator("invind.txt");
  }

  //If there is one argument, work on given directory and generate the file "invind.txt"
  else if(argc == 2){

    //If there is no file extension, it is a directory
    if(strchr(argv[1], '.') == NULL){
      browseDirectories(argv[1]);
      //Generate index file
      indexGenerator("invind.txt");
    }

    //Otherwise, print error and stop
    else{
      fprintf(stderr, "Error: Argument is not a directory.\n");
      return 0;
    }
  }

  //If there are two arguments, work on given directory/file and generate given fileName
  else if(argc == 3){

    //If the first argument is not a file, print error and stop
    if(strchr(argv[1], '.') == NULL){
      fprintf(stderr, "Error: First argument is not a file type.\n");
      return 0;
    }

    //If the second argument is a directory, browse it
    if(strchr(argv[2], '.') == NULL)
      browseDirectories(argv[2]);

    //Otherwise, browse the file
    else
      browseFile(argv[2]);

    //Generate index file
    indexGenerator(argv[1]);
  }

  //Free the memory of h (all the nodes and inner files)
  pnode_t curr = h;
  pfile_t currFile;

  while(curr != NULL) {
    free(h->word);

    currFile = curr->firstFile;
    while(currFile != NULL){
      free(curr->firstFile->fileName);

      currFile = curr->firstFile->nextFile;
      free(curr->firstFile);
    }
    curr = h->next;
    free(h);
  }

  return 0;
}
