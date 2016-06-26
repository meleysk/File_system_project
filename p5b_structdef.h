//struct def for info node 
 struct info_node
 {
 unsigned char len;
	char string[FILE_LEN];
	unsigned int size;
	struct info_node *next;
};typedef struct info_node nodeI; 
