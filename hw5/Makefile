AS = nasm #NOTE : yasm тоже подойдёт

		    all : asm
			  - prog

			  asm
			  - prog : main.o $(CC)
			  - no
			  - pie $
     ^ -o $ @

	 main.o : main.asm $(AS)
	 - felf64 $
     ^

     clean : rm - f asm
	     - prog
		 *.o core

		    .PHONY : clean

			       docker :
#docker build-- platform linux / amd64 - t asm - compiler.
    docker run
	     - it
	     - v $(pwd) : / app-- rm asm
	     - compiler
