project: parsing.o parsing2.o write.o betweenParsing.o functions.o main.o
	gcc -g -Wall -ansi -pedantic parsing.o parsing2.o write.o betweenParsing.o functions.o main.o -o assembler -lm
 
parsing.o: parsing.c global.h project.h
	gcc -g -Wall -ansi -pedantic parsing.c -c

parsing2.o: parsing2.c global.h project.h
	gcc -g -Wall -ansi -pedantic parsing2.c -c 

write.o: write.c global.h project.h
	gcc -g -Wall -ansi -pedantic write.c -o write.o -c

betweenParsing.o: betweenParsing.c global.h project.h
	gcc -g -Wall -ansi -pedantic betweenParsing.c -c

functions.o: functions.c global.h project.h
	gcc -g -Wall -ansi -pedantic functions.c -c 

main.o: main.c global.h project.h
	gcc -g -Wall -ansi -pedantic main.c -c 

