#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define MAX_WORD 31 
#define MAX_LINE 80
#define MAX_AMOUNT 50
#define START_ADD 100  /* offset for CODE start address */
#define MACHINE_RAM 2000 
#define MAX_LABELS 100 /* maximum number of labels */
#define EXT_READ ".as" /* extention for read assembly file */
#define EXT_OBJECT ".ob" /* extention of write object file */
#define EXT_EXTERN ".ext" /* extention of write external file */
#define EXT_ENTRY ".ent" /* extention of write entry file */
typedef enum{IMMEDIATE=0,LABEL,JUMP,REGISTER}address;
typedef enum {EXTERNAL=1,RELOCATABLE=2,ABSOLUTE=4} codeEnum;
enum {no=0,yes};
typedef enum CPU_REGISTERS{r0,r1,r2,r3,r4,r5,r6,r7} CPU_REGISTERS;
typedef struct{
        char label [MAX_WORD];
        int address;
        int isExternal;/* put a 0 if it's not external, 1 if it is external, 2 if it is internal.*/
        int isCode;
           }symbols;
typedef struct {
         char label[MAX_WORD];
         short address;
          }extLabelStruct  ;
typedef struct data{
   int variable : 24;
           }dataMatrix;

struct cmdStruct { /* stores commands such as mov, cmp, inc etc. */
	char name[5]; /* 4 chars + '0' */
	int code; /* code between 0 and 15 */
	int oper; /* how many operands for this command (0-2) */
	int funct;/*The kind of the function*/
	int index;/*The index of the command*/
	int permit[2][4]; /* table of 2*4 permittable source/destinations operands: immediate, direct, indirect, register */
};

typedef union 
 {
   struct
   {
  unsigned int kind : 3;/*just one of the operands is a register*/
  int destination : 21;
 
    }u;
   unsigned int fullReg;
  }immidiate;

typedef union 
 { 
struct
   {
  unsigned int kind : 3;/*just one of the operands is a register*/
  unsigned int destination : 21;
 
    }u;
   unsigned int fullReg;
  }lable;
 
typedef union 
 {
struct
   {
  unsigned int kind : 3;/*just one of the operands is a register*/
  int destination : 21;
 
    }u;
   unsigned int fullReg;
  }jump;
 
 

typedef union
 {
	struct
	{
		
                unsigned int era : 3;
		unsigned int funct : 5;
		unsigned int regDest : 3;/*instead of regparm1*/
		unsigned int ADest : 2;
		unsigned int regSource : 3;
		unsigned int ASource : 2;/*instead of regparm2*/
		unsigned int opcode : 6;
                unsigned int unused : 8;/*the size of register is  32 and that number is the empty room that left*/
	} u;
	unsigned int fullReg;
 } cpuOpcode;





void parsing(FILE *fp1);
int checkDirective(char arr[],FILE *fp1);
void checkCode(char arr[],FILE *fp1);
int checkMethod(FILE* fp1);
int checkMethod1(FILE* fp1);
char checkSpace(FILE* fp1);
int whichCode(char arr[]);
int isLabelLegal(char arr[],int length,FILE *fp1);
void checkString(FILE* fp1);
void checkData(FILE *fp1);
void parsing2(FILE *fp2);
void code(FILE* fp2,char arr[]);
void checkMethodImmediate(FILE* fp2,char nameParam[],int location);
void checkMethodLabel(FILE* fp2,char nameParam[],int location);
int checkMethodRegister(FILE* fp2,char nameParam[]);
void between();
void writeExternal(FILE *fp2);
void writeEntry(FILE* fp2);
void writeObject(FILE* fp2);
void printCode (FILE *fp, unsigned int i);              
void checkMethodJump(FILE* fp2,char nameParam[],int location);



