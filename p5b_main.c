//Name: Meley Kileyesus

//Program 5 part B: program that creates an archive from specified files, extracts files from given archive
// or prints the files in archive which have a specific prefix.
// the program allows three diffrent kinds of command line arguments and flags 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "p5b_constants.h"
#include "p5b_structdef.h"
#include "p5b_prototypes.h" 
 

//globals
nodeI* head;
nodeI* tail;
FILE* arc_ptr; //pointer to arch file used for -c command
FILE* arptr; //poiter to arch file used for -x command

int main(int argc, char* argv[])
{

int i; //temporary loop index
unsigned char length;// to store length of filename
struct stat s; //to optain stat info
int fdesc; //to obtain file descriptor
unsigned int fNum; //to store the number of files
if(argc<ARG_NUM1) //if argument is less than 3
{
fprintf(stderr,"Usage: arguments too few \n"); //writes error message to stderr
		exit(1);
}
if(strcmp(argv[FLAG],"-c")==0)
{
if(argc< ARG_NUM2|| argc> ARG_NUM3) //if flag is -c but arguments are less than 4 or greater than 258
{
fprintf(stderr,"Usage: p5b -c archieve infile1 infile2 ... \n"); //writes error message to stderr
		exit(1); 
}
else
{ //here ther is no error
// loop that starts at the 4th argument index and goes through to get
//file information
  for(i=ARG_NUM1; i<argc; i++)
  {
   length=strlen(argv[i]); //getting the length of the file string
   
   if((fdesc=open(argv[i],O_RDONLY))==-1) //file could not be opened
   {
    fprintf(stderr,"Error: could not open file %s \n",argv[i]); //writes error message to stderr
	  	exit(1); 
   } 
   
   //checking if there is error obtaining stat
	 if (fstat(fdesc,&s) == -1) 
	  {
	  printf( "Could not get stat on file %s\n", argv[i]);
	  }
	  else
	  {
	   //insert the info to the Info list
	   insert_I(length,argv[i],s.st_size);
	  }
	  close(fdesc);//closing the file
  }//end for loop
	//here all the file info has been inserted to the lists
	//so we create the arch usint the lists
	
	fNum= argc-ARG_NUM1; //file number is argc-3 bc the first 3 arguments are not files
	create_arch(fNum,argv[TWO]);
	
 }//end else
}// end if -c

if(strcmp(argv[FLAG],"-x")==0 )
{
 if(argc!= ARG_NUM1) //if flag is -x but arguments are not 3
 {
  fprintf(stderr,"Usage: p5b -x archieve \n"); //writes error message to stderr
		exit(1); 
 }
 else
 {
  extract_files(argv[TWO]);
 }
}//end if -x

if(strcmp(argv[FLAG],"-p")==0 )
{
 if( argc!= ARG_NUM2) //if flag is -p but arguments are not 4
 {
  fprintf(stderr,"Usage: p5b -p prefix archieve \n"); //writes error message to stderr
		exit(1); 
 }
 
 
  prefix_print(argv[TWO],argv[ARG_NUM1]);
 
}//end if -p

//check for invalid flag
if(strcmp(argv[FLAG],"-c")!=0 && strcmp(argv[FLAG],"-x")!=0 && strcmp(argv[FLAG],"-p")!=0)
{
 fprintf(stderr,"Error: undefined flag  %s \n",argv[FLAG]); //writes error message to stderr
		exit(1); 
}

return 0;
}//end main

