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

//externs
extern nodeI* head;
extern nodeI* tail;
extern FILE* arc_ptr; //pointer to arch file used for -c command
extern FILE* arptr;

//funcion that takes the number of files and the name of the file archive
//as parameters
void create_arch(unsigned char n, char* arcS)
{
//opending the arcive file for writing
if((arc_ptr =fopen(arcS,"wb"))==NULL)// if error in opening output file
	{ 
	 fprintf(stderr, "Error: Could not open file %s for writing.\n",arcS); //write error message to stderr
		exit(1);
	}
			//write N to the file
			fwrite(&n,sizeof(unsigned char),1,arc_ptr);
 //writie the list with all the info
   write_list(&head);
   write_bytes(&head);

}//end create_arch

//function that takes the length,name and size of the file
//as parameters and inserts them to the linked list node
void insert_I(int l, char* fString, unsigned int size)
{
 nodeI* temp;
 temp = (nodeI *)malloc(sizeof(nodeI));
	
	(temp->len)=l;
	strcpy((temp->string),fString);
	(temp->size)= size;
	
	temp->next = NULL;
	
	if(head == NULL)
	{
		//list is currently empty
		head = tail = temp;
		
	}	
	else
	{//the list is not empty. 			
		//add the node to the end of the list, 
		(tail)->next = temp;
		tail = (tail)->next;
		
	}
	}//end insert I
	
//function that takes a pointer to the head node as a parameter
//and writes the infomration in each node to the archive file
void write_list(nodeI** h){
	nodeI* temp;
	temp= *h;
	if(temp== NULL){
		printf("No match found\n");
		return;
	}
	else{
		while ((temp) != NULL)
		{
			fwrite(&((temp)->len),sizeof(unsigned char),1,arc_ptr); //writing the 1 bytelength of the file to archiv
			fwrite((temp)->string,sizeof(char),(int)((temp)->len),arc_ptr);//wring the len byte filename
			fwrite(&((temp)->size),sizeof(int),1,arc_ptr);// writing the 4byte size of the file
		 (temp) = (temp)->next; //increment node
		 
		}
	}
}//end print

//function that takes pointer to the head node as a parameter
//goes through each node and uses the file name and size to call 
//another function that opens the file and writes the bytes of file to archive
void write_bytes(nodeI** h){
	
	if(*h== NULL){
		printf("No match found\n");
		return;
	}
	else{
		//printf("Prin);
		while ((*h) != NULL)
		{
		//write info uses the size to write the bytes to arch
			write_info((*h)->string,(int)((*h)->size)); 
		 (*h) = (*h)->next; 
		}
	} 
}//end print

//takes the file name and size as parameters
//reads the amount of bytes specified by size
//and writes those bytes to the archive
void write_info(char* fName, int size)
{
 char buf[size];//buffer to read bytes
 int fdsc; //file descriptor
 ssize_t rd; //to use for read
 
 // open file, read from file and write to arch
 if((fdsc=open(fName,O_RDONLY))==-1) //file could not be opened
   {
    fprintf(stderr,"Error: could not open file %s \n",fName); //writes error message to stderr
	  	exit(1); 
   } 
  //here it is opened sucessfully so read
   rd=read(fdsc,buf,size);
   //write to arch
   fwrite(buf,sizeof(char),size,arc_ptr);
 //close file after writing
 close(fdsc);
}//end write info

//takes the archive file name as a parameter
//reads the appropriate info from file and inserts
//the info into a list
void extract_files(char* arcName)
{
unsigned char n; // for the number of files
unsigned char len;
char str[FILE_LEN]; //for file name CHANGE FILELEN
unsigned int size; //size of each file
int tot_len=0; //to keep track fo total of each string length
int i;
int dis; //to be used to specify the distance from beginning of file

if ((arptr=fopen(arcName,"rb"))==NULL)
{
  fprintf(stderr, "Error: Could not open file %s for reading.\n",arcName); //write error message to stderr
		exit(1);
}
fread(&n,sizeof(unsigned char),1,arptr);//reading the number of files
//reading the next n file s annd their infos
for(i=0;i<n;i++)
{
 fread(&len,sizeof(unsigned char),1,arptr);//reading the 1byte length of file
 //str= (char*)malloc(sizeof((int)len)); //dynamically allocating space for string
 
 fread(str,sizeof(char),(int)len,arptr); //readint hte len byte file name
 fread(&size, sizeof(unsigned int),1,arptr);//reading size
 // now insert the info in a string
 str[len]='\0'; //manually inserting the end of string char
 insert_I(len,str,size);
 tot_len+=len; //keeping track of length
}//end for loop
 fclose(arptr);//closing archive file
 dis=(LZ*n)+tot_len; //5*n+ total lengths of file names, to get the starting point of bytes to read
 copy_bytes(&head,arcName,dis);
}//end extract files

//takes a pointer to the head node, the archive name
//and the distance from beginning to the point where the 
//actual bytes start
void copy_bytes(nodeI** h,char* arcName,int dis){

int fds_ar; //file descriptor for archive

//open archive for reading
 if((fds_ar=open(arcName,O_RDONLY))==-1) //file could not be opened
 {
  fprintf(stderr,"Error: could not open file %s \n",arcName); //writes error message to stderr
	 exit(1); 
 }
 //move to apropriate position of file,wher the bytes start
 lseek(fds_ar,(off_t)(dis+1), SEEK_SET);
 
	if(*h== NULL){
		printf("No match found\n");
		return;
	}
	else{
		
		while ((*h) != NULL)
		{
		
	 write_file((*h)->string, (*h)->size, fds_ar);
		 (*h) = (*h)->next; 
		 
		}
	}
}//read bytes

//takes teh filename,size and file descriptor of the archive file
//as parameters, reads the amount specified by size from archive
//and writes it to the file
void write_file(char* fName, int size, int fds_ar)
{
 char buf[size];//buffer to read bytes
 int fds_f; //file descriptor for file
 
 if((fds_f=open(fName,O_WRONLY|O_CREAT,PER))==-1) //file could not be opened
   {
    fprintf(stderr,"Error: could not open file %s \n",fName); //writes error message to stderr
	  	exit(1); 
   } 
  read(fds_ar,buf,size); //read from arch the bytes specified by size
  write(fds_f,buf,size);//write to file the bytes specified by size
  close(fds_f);//closing file
}//end write_byes

//takes the prefix and archive name as parameters
//if a file with the specified prefix is found,
//it prints the file and its info to stout
void prefix_print(char* prefix, char* arcName)
{
unsigned char n; // for the number of files
unsigned char len;
char str[FILE_LEN]; //for file name CHANGE FILELEN
unsigned int size; //size of each file
int p_len=strlen(prefix); //storing the length of the prefix in p_len
char temp[p_len]; //temporary string used for comparison
int i;


if ((arptr=fopen(arcName,"rb"))==NULL)
{
  fprintf(stderr, "Error: Could not open file %s for reading.\n",arcName); //write error message to stderr
		exit(1);
}
fread(&n,sizeof(unsigned char),1,arptr);//reading the number of files
//reading the next n file s annd their infos
for(i=0;i<n;i++)
{
 fread(&len,sizeof(unsigned char),1,arptr);//reading the 1byte length of file
 //str= (char*)malloc(sizeof((int)len)); //dynamically allocating space for string
 
 fread(str,sizeof(char),(int)len,arptr); //readint hte len byte file name
 fread(&size, sizeof(unsigned int),1,arptr);//reading size
 // now insert the info in a string
 str[len]='\0'; //manually inserting the end of string char
 strncpy(temp,str,p_len); //copying the portion of str with same legnth as prefix to temp
 temp[p_len]='\0';
  if(strcmp(temp,prefix)==NULL) //if prefix is found
 {
 insert_I(len,str,size);//insert info to the list
 }
}//end for loop//
//print the list
print_list(&head);
}//end prefix_print

//takes a pointer to the head node as a parameter
//prints out the name and size of each node
void print_list(nodeI** h){
	
	if(*h== NULL){//if list is empty
		printf("No matching files found\n");
		return;
	}
	else{
		
		while ((*h) != NULL)
		{
		//print name and size
			printf(" %s  \t %u  \n",(*h)->string, (*h)->size);
	
		 (*h) = (*h)->next; 
		}
	}
}//end print




