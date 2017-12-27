/*
Structure definitions
*/

//Structure for the fileNames and their counts (frequency of word in file)
typedef struct files{
  char *fileName;
  int count;
  struct files *nextFile;
} file_t, *pfile_t;


//Structure for the mapping of the word, filename, count (frequency of word in file)
typedef struct node{
  char *word;
  pfile_t firstFile;
  struct node *next;
} node_t, *pnode_t;


                        
