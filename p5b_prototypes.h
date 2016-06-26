//prototypes
void insert_I(int l, char* fString, unsigned int size);
void insert_B( );
void create_arch(unsigned char n, char* archStr); //nodeI* head, nodeB* head);
	void write_list(nodeI** hd);
	void write_bytes(nodeI** hd);
	void write_info(char* fName,int size);
	void extract_files(char* arcName);
	void print_list(nodeI** h); //FOR TEST, REMOVE
	void copy_bytes(nodeI** h,char* arcName,int dis);
	void write_file(char* fName, int size, int fds_ar);
	void prefix_print(char* prefix, char* arcName);
