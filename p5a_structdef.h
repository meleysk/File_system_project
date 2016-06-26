//tree stucture definition
struct tree_node{
		char string[STR_LEN]; 
	 long int  size;
		char date[DAT_LEN]; 
		struct tree_node* left;
		struct tree_node* right;
	};
typedef struct tree_node tNode;
