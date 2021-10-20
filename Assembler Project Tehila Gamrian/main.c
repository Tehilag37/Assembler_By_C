#include "project.h"
/*presenter by Tehila Gamrian 211975867 and sarah Gulia Sudri 918258021*/
/*
project goal is to translate assembly files to obinary files which 
are used by the linker program for assembly.
Input files are given in command arguments  
the program will append .as to the file names for input.
if tranlation encounters error it will send them to stderr and stop the translation at end of file.
if no errors it will produce 3 files for each input file:
1) file.ob - object code
2) file.ent - entries label codes produce only if their is a entries label 
3) file.ext - external label codes produce only if their is a external label */

/* global variables are defined here they start with g_ to distinguish them */
symbols* g_label=NULL;/*this is array of sturcts that contain all the labels- the name of them the address, if them entry or external and if them defined in command line*/
int g_flagEntry=0;/* this variablre show how many entries label there is*/
char line [MAX_LINE];/* string that can contain a line*/
int g_numLine;/*show which line are in now*/
unsigned int g_code[MACHINE_RAM]; /* array to store machine code */
 dataMatrix g_data[MACHINE_RAM]; /* array to store machine data (numbers and strings) */
int g_ic=START_ADD, g_dc=0; /* Instruction Counter and Data counter the Instruction Counter stert with 100 because the code start from 100*/
int g_err; /* have we reached an error in file? */
 int g_index=0;/* counter of labels*/
extLabelStruct* g_ext_label=NULL;/*array to store  the extarnal label,it contain the name of the label and in which line it used*/
int g_countExt=0;/* this variablre show how many time the extearnal label used*/
char *regs[]={"r0","r1","r2","r3","r4","r5","r6","r7"};/* a array of all the names of register*/
struct cmdStruct cmd[] = {
/* table of all commands which holds: string, code, number of arguments,funct, index, permittable params in source destination.
	the 4 permittable params are: immediate, label, jump with parameters, register. 1 means permitted 0 means not permitted. */
	{"mov", 0, 2,0,0, {{1,1,0,1}, {0,1,0,1}}},/*first argument and second */
	{"cmp", 1, 2,0,1, {{1,1,0,1}, {1,1,0,1}}}, /*s,d*/
	{"add", 2, 2,1,2, {{1,1,0,1}, {0,1,0,1}}}, 
	{"sub", 2, 2,2,3, {{1,1,0,1}, {0,1,0,1}}},
	{"lea", 4, 2,0,4, {{0,1,0,0}, {0,1,0,1}}},
	{"clr", 5, 1,1,5, {{0,0,0,0}, {0,1,0,1}}}, 
	{"not", 5, 1,2,6, {{0,0,0,0}, {0,1,0,1}}}, 
	{"inc", 5, 1,3,7, {{0,0,0,0}, {0,1,0,1}}}, 
	{"dec", 5, 1,4,8, {{0,0,0,0}, {0,1,0,1}}}, 
	{"jmp", 9, 1,1,9, {{0,0,0,0}, {0,1,1,0}}}, 
	{"bne", 9, 1,2,10, {{0,0,0,0}, {0,1,1,0}}},
	{"jsr", 9, 1,3,11, {{0,0,0,0}, {0,1,1,0}}},  
	{"red", 12, 1,0,12, {{0,0,0,0}, {0,1,0,1}}}, 
	{"prn", 13, 1,0, 13,{{0,0,0,0}, {1,1,0,1}}}, 
	{"rts", 14, 0,0,14, {{0,0,0,0}, {0,0,0,0}}}, 
	{"stop", 15, 0,0,15, {{0,0,0,0}, {0,0,0,0}}}
};

int main (int argc, char *argv[]) {
	FILE *fp1, *fp2; /* fp1 for read fp2 for write */
	int i;
	char fileName[4][50]; /* array to store up to 4 file names */
	if (argc == 1) {
		fprintf (stderr, "No input file.\n");
		return 1;
	}
	for (i=1;i<argc;i++) { /* for all files in command line */
		strcpy (&fileName[0][0],*(argv+i)); 
		strcat (&fileName[0][0], EXT_READ); /* append .as to file name */
		strcpy (&fileName[1][0],*(argv+i));
		strcat (&fileName[1][0], EXT_OBJECT); /* append .ob to file name */
		strcpy (&fileName[2][0],*(argv+i));
		strcat (&fileName[2][0], EXT_EXTERN); /* append .ext to file name */
		strcpy (&fileName[3][0],*(argv+i));
		strcat (&fileName[3][0], EXT_ENTRY); /* append .ent to file name */
		if (!(fp1 = fopen(&fileName[0][0], "r"))) { /* open .as file for read */
			fprintf (stderr, "can't open %s\n", &fileName[0][0]);
			return 1; 
		} 
		parsing (fp1); /* parse the file */
                fclose(fp1);
                if (!(fp1 = fopen(&fileName[0][0], "r"))) { /* open .as file for read */
			fprintf (stderr, "can't open %s\n", &fileName[0][0]);
			return 1; 
		} 
		if(g_err)/* if we reached an error exit */
			continue;
                parsing2(fp1);
                if(g_err)/*if we reached an error exit */
			continue;
		if (!(fp2 = fopen(&fileName[1][0], "w"))) { /* open .ob file to write */
			fprintf (stderr, "can't open %s\n", &fileName[1][0]);
			return 1; 
		} 
		writeObject(fp2); /* write .ob file content */
                if(g_countExt!=0)/*if there is a  command that used with external label*/
                {
		if(!(fp2 = fopen(&fileName[2][0], "w"))) { /* open .ext file to write */
			fprintf (stderr, "can't open %s\n", &fileName[1][0]);
			return 1; 
		} 
		writeExternal(fp2); /* write .ext file content */
                }
                if(g_flagEntry!=0)/*if there is a entries label*/
                {
		if ((fp2 = fopen(&fileName[3][0], "w")) == NULL) { /* open .ent file to write */
			fprintf (stderr, "can't open %s\n", &fileName[1][0]);
			return 1; 
		} 
		writeEntry(fp2); /* write .ent file content */
                }
		if(g_countExt)/* release the external chart*/
          	  free(g_ext_label);
	        if(g_label)/*release the label chart*/
                  free(g_label);
	}
	return 0;
}

