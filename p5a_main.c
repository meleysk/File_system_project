//Name: Meley Kileyesus

//Program 5 part A: given a directory name, it prints out the name , size and date modified of hidden files
// if a flag -s is specified, it prints the information in a sorted size order

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "p5a_constants.h"
#include "p5a_structdef.h"
#include "p5a_prototypes.h"
 
//global tree root
tNode *root; 
int main(int argc, char* argv[])
{

	if (argc!=ARG_NUM1 && argc!=ARG_NUM2)//checking if number of arguments are not two or three
	{
 	fprintf(stderr,"usage: p5a pathName OptionalFlag  \n"); //writes error message to stderr if not correct
		exit(1); 
	}
	
	if (argc==ARG_NUM1)//when flag is not specified
	{
	//print info in any order
 	print_info(argv[1]); 
	
	}//end if 2 arguments

	if(argc==ARG_NUM2) //when flag is specified
	{
 	if (strcmp(argv[FLAG],"-s")!=0) //check flag for error
 	{
 	fprintf(stderr,"Error: undefined flag  %s \n",argv[FLAG]); //writes error message to stderr if flag is not -s
		exit(1); 
 	}
  //insert info in a tree
  insert_info(argv[1]);
 //PRINT tree 
  print_tree(root);
}//end if flag specified

return 0;
}//end main
 

//function that takes the directory name as a parameter
//goes through each line and prints each file name, size and date modified
void print_info(char *dirName)
{
 DIR* dirptr;
	struct dirent *dp;
	struct stat s; 
 const char* fName;
 char temp[TIME_LEN];
 char *d;
 char *month;
 char *day;
 char *time;
 char *year;
 char date[DAT_LEN];
 
 if ((dirptr = opendir(dirName)) == NULL) //open directory
	 {
	 fprintf(stderr, "Could not open directory %s\n", dirName);
	 exit(1);
  }
 chdir(dirName) ;
	while ((dp = readdir(dirptr)) != NULL) //reading one directory entry at a time
	{
	//checking that the file name is not a '.' or '..'
	if((dp->d_name)[0]=='.' && strcmp((dp->d_name),".")!=0 && strcmp((dp->d_name),"..")!=0 )
	 {
  fName=(dp->d_name);
       
	 //checking if there is error obtaining stat
	 if (stat(fName,&s) == -1) 
	 {
	 printf( "Could not get stat on file %s\n", fName);
	 }
	 else
	 {
	 strcpy(temp,ctime(&(s.st_mtime)));
	 //using strtok to separate the strings
	 d=strtok(temp," \t");
	 month=strtok(NULL," \t");
	 day=strtok(NULL, " \t");
	 time=strtok(NULL, " \t");
	 year=strtok(NULL, " \t\n");
	 
	 //concatenating only the month day and year back together
	 strcpy(date,month);
	 strcat(date," ");
	 strcat(date,day);
	 strcat(date," ");
	 strcat(date,year);
	 //printing info to stdout
  printf("%s  \t %ld  \t%s\n", fName,s.st_size,date);  
  }//end else
 }//end if
} //end while
//cose the directory
closedir(dirptr);
 
}//end print_info


//functons that takes directory name as a parameter
//this is used when the -s flag is specified
//it reads the information and inserts each file name 
//along with size and date modified in a sorted size order
//in to a tree
void insert_info(char* dirName)
{
  DIR* dirptr;
  struct dirent *dp;
	struct stat s; 
 const char *fName;
 char temp[TIME_LEN];
 char *d;
 char *month;
 char *day;
 char *time;
 char *year;
 char date[DAT_LEN];
 
 if ((dirptr = opendir(dirName)) == NULL) //open directory
	 {
	 fprintf(stderr, "Could not open directory %s\n", dirName);
	 exit(1);
  }
  chdir(dirName); //changing current directory to the input directory
	while ((dp = readdir(dirptr)) != NULL) //reading one directory entry at a time
	{
	//checking that the file name is not a '.' or '..'
	if((dp->d_name)[0]=='.' && strcmp((dp->d_name),".")!=0 && strcmp((dp->d_name),"..")!=0 )
	 {
	 //checking if there is error obtaining stat
	 if (stat(dp->d_name,&s) == -1) 
	 {
	 printf( "Could not get stat on file %s\n", fName);
	 }
	 else
	 {
	  strcpy(temp,ctime(&(s.st_mtime)));
	 //use strtok to separate the strings
	 d=strtok(temp," \t");
	 month=strtok(NULL," \t");
	 day=strtok(NULL, " \t");
	 time=strtok(NULL, " \t");
	 year=strtok(NULL, " \t\n");
	 
	 //concatenating only the month day and year back together
	 strcpy(date,month);
	 strcat(date," ");
	 strcat(date,day);
	 strcat(date," ");
	 strcat(date,year);
	 
	 //inserting the name size and date modified to a binary tree
  insert_node(&root,(dp->d_name),s.st_size,date);
  }//end else
 }
} 
//close directory
 closedir(dirptr);
}//end insert_info

//function takes pointer to the tree root, the file name string,
// the size of the file and the date string as parameters
//inserts each data to the tree in sorted order
 void insert_node(tNode **rt, char *str,long int s, char* d) 
{
 
 if((*rt)==NULL)   //if tree is empty
  {    
   *rt = (tNode*)malloc(sizeof(tNode)); //allocate space for a node
   (*rt)->left = (*rt)->right = NULL; //set right and left nodes to null bc it is the only node
    strcpy(((*rt)->string),str);         //copy the input string to the node elements
   ((*rt)->size)=s;																//copy the size 
    strcpy((*rt)->date,d);             //copy the time
    
   return; 
  }

 if(s<=((*rt)->size)){  //if the size is less than or equal to the size of the root node
			insert_node(&(*rt)->left, str,s,d);  // recursively inset on the left of the root     
  } 
  
 else if(s>((*rt)->size)) { //if the input size is greater than the size in the node
     insert_node(&(*rt)->right, str,s,d);     //recursively insert on the right of the node
   }
 }
 
// takes the root as a parameter and prints the binar tree
 void print_tree(tNode* rt)
{
 if(rt==NULL)   //if nothing in the tree
	{
   return; 
	}
	else{
   print_tree((rt->left)); // recursively print the left side of tree
   printf("%s\t %ld \t %s\n", (rt->string),(rt->size) ,(rt->date));
   print_tree(rt->right); //recursively prints right side
 }
} //end print tree


