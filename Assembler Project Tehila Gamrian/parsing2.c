
#include "project.h"
#include "global.h"
/*This function parse the first time the file, and encode the file according to the commands or directive,
that is writen in the file, the function cautions if an input error exists. */
void parsing2(FILE *fp2)
{
int i;
char arr[MAX_WORD];
char line [MAX_LINE];
char ch=fgetc(fp2);/*read one character in order to recognize if it reach the end of the file*/
g_numLine=0;
g_ic=0;
g_err=0;
g_countExt=0;
g_flagEntry=0;/* a flag to recognize if there is an entry label in the file, in order to create an entry file.*/
 while(ch!=EOF)/* The function run untill it ends*/
 	{
 int flagIsEntry=0;/* ask if the label is an entry label.*/
 fseek(fp2,-1,SEEK_CUR);/*go back one step, in order to read the all word.*/
 g_numLine++;
 fscanf(fp2,"%s",arr);/* read the first word in the line, in order to associate the line to the type. */
 if(arr[0]==';')/* if the line is empty, or with no meaning continue.*/
 		{
    fgets(line,MAX_LINE,fp2);
    ch=fgetc(fp2);
    continue;
 		}

 if(arr[strlen(arr)-1]==':')/* If the last sign is colon, it is a label.*/
 	{
  fscanf(fp2,"%s",arr);
 	}
 if(strcmp(arr,".string")==0||strcmp(arr,".data")==0||strcmp(arr,".extern")==0)/* if the label is the kind of string or data or externcontinue readig*/
 	{
  fgets(line,MAX_LINE,fp2);
  ch=fgetc(fp2);
  continue;
 	}
 if(strcmp(arr,".entry")==0)/* if the label is the kind of entry, continue reading the data afterwords*/
 	{
  g_flagEntry=1;
  fscanf(fp2,"%s",arr);
  i=0;
  while(!flagIsEntry&&i!=g_index)
  	{
   if(strcmp(g_label[i].label,arr)==0)/* check if the label is defined.*/
   	{
    g_label[i].isExternal=2;
   flagIsEntry=1;
   	}
   i++;
  	}
   if(!flagIsEntry)/* if the label is not defined it is an error.*/
   	{
    printf("Error: The label %s is not defined, but it be used as an entry directive. Line: %d\n",arr,g_numLine);
    g_err=yes;
   	}
   ch=fgetc(fp2);
   continue;
 	}
code(fp2,arr);/* go to the function code*/
ch=fgetc(fp2);
	}
}
/* This function gets lines in the file that are not labels and try to check if they are commands,
and encode them as the method of the parameters.*/


void code(FILE* fp2,char arr[])
{
char ch;
int place;
int j=0;
int regist1;
 int regist2;/*if the method of addressing will be register, he creats parameters (the right command)*/
 int rg1;
 int rg2;/* the parameters of the command (kind).*/
char nameParam[MAX_WORD];/* a buffer to put in the name of the parameters.*/
cpuOpcode c;/* build up a struct to put in the data of the command*/

place= whichCode(arr);/*send to the function to define what is the command.*/
if(place==-1) /* if the function didn't recognize the command' is the line an other kind of line.*/
	{
 fgets(line,MAX_LINE,fp2);
 return;
	}
c.u.opcode=cmd[place].code;/* the opcode of the commmand is as the plase thet the function returned*/
c.u.funct=cmd[place].funct;
c.u.era=ABSOLUTE;/* the kind of the command is absolute*/

 
 c.u.regDest=0;
 c.u.regSource=0;
 rg1= checkMethod(fp2);/* check if the method is legal, and which method of addressing it use.*/
 if(rg1==-1)/* if there is no parameters.*/
 	{
 if(cmd[place].oper==0)/* if the command doesn't need parameters.*/
 		{
   c.u.ADest=0;
   c.u.ASource=0;
   c.u.regDest=0;
   c.u.regSource=0;	
   g_code[g_ic]=c.fullReg;/* feel in the struct and send to the arrey of codes.*/
   g_ic++;
 		}
 return;
 	}
fseek(fp2,-2,SEEK_CUR);/* go back 2 steps in order to read the all parameter*/
ch=fgetc(fp2);

 while(ch!=' '&&ch!='\t')/* go back in order to put to recognize the name of the parameter.*/
 	{
  fseek(fp2,-2,SEEK_CUR);
  ch=fgetc(fp2);
 	}
ch=fgetc(fp2);
 while(ch!=' '&&ch!='\t'&&ch!='\n'&&ch!=','&&fp2)/* put the parameter as a string in order to recognize its type.*/
 	{
  nameParam[j]=ch;
  j++;
   ch=fgetc(fp2);
 	}
 nameParam[j]='\0';
  if(ch==' '||ch=='\t')
    ch=checkSpace(fp2);
if(rg1==IMMEDIATE)/* check what the type of the first parameter, and send to the function that handle with it.*/
  checkMethodImmediate(fp2,nameParam,g_ic+1);
 if(rg1==LABEL)/* check what the type of the first parameter, and send to the function that handle with it.*/
  checkMethodLabel(fp2,nameParam,g_ic+1);
 if(rg1==REGISTER)/* check what the type of the first parameter, and send to the function that handle with it.*/
 regist1=checkMethodRegister(fp2,nameParam);
if (rg1==JUMP)
	checkMethodJump(fp2, nameParam,g_ic+1);	
if(ch=='\n'||!fp2)/* go back one step if it reach the end.*/
 fseek(fp2,-1,SEEK_CUR);
rg2= checkMethod(fp2);/* go next and check what the type of the second parameter.*/

if(rg2==-1)/* if there isn't another parameter, feel in the data as acommand with one parameter*/
	{
 if(cmd[place].oper==1)/* just if the function realy need one parameter enter the data.*/
 		{
   c.u.ADest=rg1;
   c.u.ASource=0;
   if(rg1==REGISTER)/* if the kind of the parameter is a register put in the data of the register.*/
   			{
    
    c.u.regDest=regist1;
    c.u.regSource=0;
    c.u.era=ABSOLUTE;
    g_code[g_ic]=c.fullReg;
	g_ic++;
   			}
   else 		{
	   g_code[g_ic] = c.fullReg;/* feel in the data in the arrey of codes. */
	   g_ic += 2;
   			}
 		}
 return;
	}
fseek(fp2,-2,SEEK_CUR);/* go back 2 steps in order to read the all parameter*/
ch=fgetc(fp2);

while(ch!=' '&&ch!='\t'&&ch!=',')/* go back in order to read the whole word at once.*/
	{
 fseek(fp2,-2,SEEK_CUR);
 ch=fgetc(fp2);
	}
 ch=fgetc(fp2);
 j=0;
 while(ch!=' '&&ch!='\t'&&ch!='\n'&&ch!=','&&fp2)/* read the second parameter again in order to define his type.*/
 	{
  nameParam[j]=ch;
  j++;
  ch=fgetc(fp2);
 	}
 nameParam[j]='\0';
 if(cmd[place].oper==2)/* just if the command realy need 2 parameters check the second parameter.*/
 	{
  if(rg2==IMMEDIATE) /* check what the type of the second parameter, and send to the function that handle with it.*/
  		{
  	if(rg1==REGISTER)
  		checkMethodImmediate(fp2,nameParam,g_ic+1);
  		else
  			checkMethodImmediate(fp2,nameParam,g_ic+2);
  		}

   if(rg2==LABEL) /* check what the type of the second parameter, and send to the function that handle with it.*/ 
  		{
  		 if(rg1==REGISTER)
    checkMethodLabel(fp2,nameParam,g_ic+1);
   
    else
        checkMethodLabel(fp2,nameParam,g_ic+2);
   		}
   if(rg2==REGISTER) /* check what the type of the second parameter, and send to the function that handle with it.*/ 
   		{
    regist2=checkMethodRegister(fp2,nameParam);
   		}
   c.u.ADest=rg2; /* feel in the data of the second parameter.*/
   c.u.ASource=rg1;
   g_code[g_ic]=c.fullReg;
   c.u.era=ABSOLUTE;
    if(rg1==REGISTER&&rg2==REGISTER)/* if both parameters are registers feel in both parameters in one struct*/
    		{
     c.u.regDest=regist2;
     c.u.regSource=regist1;
     g_code[g_ic]=c.fullReg;
     g_ic++;
    		}
    else if(rg1==REGISTER)/* if only one of the parameters is a register handle with it*/
     			{ 
      c.u.regSource=regist1;
      g_code[g_ic]=c.fullReg;
	  g_ic+=2;
     			}
	else if (rg2 == REGISTER)/* if only one of the parameters is a register handle with it*/
	{
		c.u.regDest = regist2;
		g_code[g_ic] = c.fullReg;
		g_ic += 2;
	}
	else
	{
      g_code[g_ic]=c.fullReg;
	  g_ic += 3;
	}
    }
  }




