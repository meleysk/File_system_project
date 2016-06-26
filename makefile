#possible suffixes
.SUFFIXES: .c .o

#gcc will be used to compile the source file
CC = gcc

CFLAGS= -g

#rule how the executable should be created from the source file
.c.o:
	$(CC) $(CFLAGS) -c $<
	
#dependency rule for the default target
all:	p5a_main.o	p5b_main.o	p5b_functions.o
	gcc p5a_main.o  -o p5a
	gcc p5b_main.o	p5b_functions.o -o p5b
	
p5a: p5a_main.o 
	gcc p5a_main.o	 -o p5a
	
p5b: p5b_main.o p5b_functions.o	
	gcc p5b_main.o p5b_functions.o	-o p5b
	
#dependency rule for other targets
p5a_main.o:  p5a_constants.h		p5a_structdef.h  p5a_prototypes.h
p5b_main.o:  p5b_constants.h		p5b_structdef.h  p5b_prototypes.h
p5b_functions.o:  p5b_constants.h p5b_structdef.h p5b_prototypes.h



#target for removing unnecessary files
clean:
	rm -f *.o core
