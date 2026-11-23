compiler = cc

mypreprocessor:main.o file_inclusions.o macro_handler.o comment_removal.o 
	$(compiler) $^ -o $@

main.o:main.c header.h
	$(compiler) -c $< 

file_inclusions.o:file_inclusions.c
	$(compiler) -c $^ 

macro_handler.o:macro_handler.c
	$(compiler) -c $^ 

comment_removal.o:comment_removal.c
	$(compiler) -c $^

clear:
	@echo "cleaningup..."
	@rm -vr *.o