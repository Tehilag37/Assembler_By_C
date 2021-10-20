#include "project.h"
#include "global.h"

int isLabelLegal(char arr[],int length,FILE *fp1)/*this function check if the label legal, if it legal return 0 else return 1*/
{
 int i;

 for(i=0;i<length;i++)/*passing all the labels that has and check if the lebal already exist*/
 {
  if(strcmp(arr,g_label[i].label)==0)/*if the label already exist*/
  {
   printf("Error:  the label %s is set twice.	Line: %d\n",arr,g_numLine);
   g_err=yes;
   fgets(line,MAX_LINE,fp1);
   return 1;
  }
 }
 for(i=0;i<16;i++)/*passing all the commands and check if the lebal a command*/
 {
  if(strcmp(arr,cmd[i].name)==0)/*if the lebal command*/
  {
   printf("Error:  the label %s cant be label because it a saved word.	Line: %d\n",arr,g_numLine);
   g_err=yes;
   fgets(line,MAX_LINE,fp1);
   return 1;
  }
 }
for(i=0;i<8;i++)/*passing all the register and check if the label name of register*/
 {
  if(strcmp(arr,regs[i])==0)/*if the label name of register*/
  {
   printf("Error:  the label %s cant be label because it a saved word.	Line: %d\n",arr,g_numLine);
   g_err=yes;
   fgets(line,MAX_LINE,fp1);
   return 1;
  }
}
 for(i=1;i<strlen(arr);i++)/*check if the label include only letters and numbers*/
 {
  if(isalnum(arr[i])==0)
  {
   printf("Error:  the label %s not only letters and numbers.	Line: %d\n",arr,g_numLine);
   g_err=yes;
   fgets(line,MAX_LINE,fp1);
   return 1;
  }
 }
 return 0;
}
/*int isJumpLableGood (char arr[],int length,FILE *fp1)
{
 int i;
if(arr[0]=='&')
	{
	int j=0;
	while (arr[j]!='\0')
		{
		arr[j]=arr[j+1];
		j++;
		}
		arr[j]='\0';
	}
 for(i=0;i<length;i++)/*Goes through all the existing labels and checks if it already exists*/
 /*{
  if(strcmp(arr,g_label[i].label)==0)/*if the label is exist*/
  
   /*return 1;
  
 }
 
   printf("Error:  the label %s isn't exist	Line: %d\n",arr,g_numLine);
   g_err=yes;
   fgets(line,MAX_LINE,fp1);
   return 0;

 
	
}*/
void checkString(FILE* fp1)/*this function passing the line of .string and insert the data to the place in array data, if the line doesn't worte good, write an error massage read all that's left in the line and going out from the function*/
{
 char ch;
 ch=checkSpace(fp1);
 if(ch!='\"')/*if the data don't start with " */
 {
  printf("Error:  the string in the directive .string need to start with \"	Line: %d\n",g_numLine);
  fgets(line,MAX_LINE,fp1);
  g_err=yes;
  return;
 }
 ch=fgetc(fp1);/*read the next character*/
 while(ch!='\"')/*as long as he didn't reach the end of the string he read the data*/
 {
  if(ch=='\n'||!fp1)/*if he finish all the data without put " in the end*/
  {
  printf("Error:  the string in the directive .string need to finish with \"	Line: %d\n",g_numLine);
   g_err=yes;
   return ;
  }
  g_data[g_dc].variable=ch;/*insert the character to the place in the chart*/
  g_dc++;
  ch=fgetc(fp1);/*read the next character*/
 }
 ch=checkSpace(fp1);
 g_data[g_dc++].variable='\0';/*insert 0 to the place in the chart after all the string that insert*/
 if(ch!='\n'&&ch!=EOF)/*if there is more character after the " */
 {
   printf("Error:  the string in the directive .string need to finish with \"	Line: %d\n",g_numLine);
   g_err=yes;
   fgets(line,MAX_LINE,fp1);
   return;
 }
}


void checkData(FILE *fp1)/*this function passing the line of .data and insert the numbers to the place in array data, If the line is not written correctly, write an error massage read all that's left in the line and going out from the function*/
{
 char ch=checkSpace(fp1);/*read the first character*/
 while(ch!='\n'&&ch!=EOF)/*as long as he didn't reach to the end of the line*/
 {
  char str[10]="\0";
  int i=0,num=0;
   if(ch==',')/*if there are two consecutive commas*/
    {
     printf("Error:  the legal order in .data need to be integer and than comma and not two comma in a row.	Line: %d    \n",g_numLine);    
     g_err=yes;
     while(ch!='\n'&&ch!=EOF)
      ch= fgetc(fp1);
     return;
    }

  while(ch!='\n'&&ch!=EOF&&ch!=','&&ch!=' '&&ch!='\t')/*read a number*/
  {

   if(ch!='-'&&ch!='+'&&isdigit(ch)==0)/*if this is not a number*/
   {
   
    
     printf("Error:  it is not legal to enter things that not integers in .data.	Line:  %d\n",g_numLine);
    g_err=yes;
    while(ch!='\n'&&ch!=EOF)
     ch=fgetc(fp1);
    return;
   }
   if((ch=='-'||ch=='+')&&i!=0)/*if this is not a number*/
   {
    printf("Error:  it is not legal to enter things that not integers in .data.	Line:  %d\n",g_numLine);
    g_err=yes;
    while(ch!='\n'&&ch!=EOF)
     ch=fgetc(fp1);
    return;
   }
   str[i]=ch;
   i++;
   ch=fgetc(fp1);/*read the next data*/
  }
  num=atoi(str);/*move the data from string to number*/
  g_data[g_dc].variable=num;/*insert the number to the place in the chart*/
  g_dc++;
  if(ch==' '||ch=='\t')
   ch=checkSpace(fp1);
  if(ch!=','&&ch!='\n'&&ch!=EOF)/*if their is not comma between the number*/
  {
   printf("Error:  the legal order in .data need to be integer and than comma.	Line:  %d\n",g_numLine);
     g_err=yes;
    while(ch!='\n'&&fp1)
     ch=fgetc(fp1);
    return;
  }
  if(ch==',')
  {
   ch=checkSpace(fp1);
   if(ch=='\n'||!fp1)/*if the data finish with a comma in the end*/
   {
     printf("error:   in .data not need to finish with ,	Line:  %d\n",g_numLine);
    g_err=yes;
   }
  }
 }
}




int whichCode(char arr[])/*this function passing all the command and check  which command the word if it is not a connand return -1 else return the number of the command*/
{
 int i;
 for(i=0;i<16;i++)
 {
  if(strcmp(cmd[i].name,arr)==0)
   return i;
 }
 return -1;
}




char checkSpace(FILE* fp1)/*this function read spaces and tabs until he come to character that not and return it*/
{
 char ch;
 ch=fgetc(fp1);
 while(ch==' '||ch=='\t')
  ch=fgetc(fp1);
 return ch;
}



int checkMethod(FILE* fp1)/*this function check what a method written and return 0 if immediate 1 if label, 2 if its a jump  and 3 if register if he come to the end of the line and their is not character return -1*/
{
 char ch;
 ch= checkSpace(fp1);
 if(ch=='\n'||ch==EOF)/*if it comes to the end of the line*/
   return -1;
 if(ch=='#')/*that mean that this immediate method*/
 {
  while(ch!=' '&&ch!=','&&ch!='\t'&& ch!='\n'&&fp1)
  {
   ch=fgetc(fp1);
  }
  return IMMEDIATE;
 }
if(ch=='&')/*that mean that this an address */
 {
  while(ch!=' '&&ch!=','&&ch!='\t'&& ch!='\n'&&fp1)
  {
   ch=fgetc(fp1);
  }
  return JUMP;
 }
 if(ch=='r')/*if it start with r check if it register*/
 {
   ch=fgetc(fp1);
   if(isdigit(ch))/*if the after character is number*/
   {
   int num;
    num=ch-'0';
    if(num>=0&&num<=7)/*check if it one of the registers*/
    {
     ch=fgetc(fp1); 
     if(ch!=' '&&ch!=','&&ch!='\t'&& ch!='\n'&&fp1)/*if it not register*/
     {
      while(ch!=' '&&ch!=','&&ch!='\t'&& ch!='\n'&&fp1)
       ch=fgetc(fp1);
      return LABEL;
     }
     return REGISTER;/*if it register*/
    }
   }
 }
 while(ch!=' '&&ch!=','&&ch!='\t'&& ch!='\n'&&fp1)/*if not register or immidiate or jump its a lable*/
 {
  ch=fgetc(fp1);
 }
 return LABEL;
}

 
 
 
    
void checkMethodImmediate(FILE* fp2,char nameParam[],int location)/*this function getting a string that include number and place to insert and insert the number to the place in array command */
{
 int num;
 immidiate a;
 num=atoi(nameParam+1);/*move the data from string to number*/
 a.u.kind=ABSOLUTE;
 a.u.destination=num;
 g_code[location]=a.fullReg;
}

void checkMethodLabel(FILE* fp2,char nameParam[],int location)/*this function getting a string that include label and place to insert and insert the label to the place in array command */
{
 int i,flag=no;/*flag show if the label found in the table of the labels*/

 lable a;
 for(i=0;i<g_index&&flag==no;i++)/*passing all the label table as long as the lebal not found*/
{	    	

if(strcmp(nameParam,g_label[i].label)==0)/*if the label found*/
 {
  flag=yes;
  if(g_label[i].isExternal==1)/*if the label is external adding to the extearnal chart*/
   	{
    	a.u.kind=EXTERNAL;
    	a.u.destination=0;
	if(g_countExt==0)/*if until now there is not any label external build a new extearnal chart*/
    		{
      		g_ext_label=(extLabelStruct*)malloc(sizeof(extLabelStruct));
      		if(!g_ext_label)
      			{
      			printf("Error: memory allocation failed.	Line: %d\n", g_numLine);
      			exit(0);
      			}
    		}
    		else/*adding to the extearnal chart another data*/
    		{
      		g_ext_label=(extLabelStruct*)realloc(g_ext_label,sizeof(extLabelStruct)*(g_countExt+1));
      		if(!g_ext_label)
      			{
      			printf("Error: memory allocation failed.	Line: %d\n", g_numLine);
      			exit(0);
      			}
  		}
  		strcpy(g_ext_label[g_countExt].label,nameParam);/*add the name of lebal to the external data*/
   		g_ext_label[g_countExt].address=location;/*add the location of lebal to the external data*/
   		g_countExt++;
   	}
   	else/*if it not external updating the address of the label and the is relocatable*/
   	{
    	a.u.destination=g_label[i].address;
   	a.u.kind=RELOCATABLE;
   	}
   	g_code[location]=a.fullReg;
  }
}
 if (flag==no)/*if the label don't found in the label chart*/
 		{
  printf("Error:  the variable %s wasn't defined as a label.	Line: %d\n",nameParam,g_numLine);
  g_err=yes;
  checkMethod(fp2);
		 }
	
}
void checkMethodJump(FILE* fp2,char nameParam[],int location)/*this function getting a string that include label and place to insert and insert the label to the place in array command */
{
 int i,flag=no;/*flag show if the label found in the table of the labels*/
 jump a;

	int j=0;
	while (nameParam[j]!='\0')/*delete the & from the begining of the label name*/
		{
		nameParam[j]=nameParam[j+1];
		j++;
		}
		nameParam[j]='\0';
	
 for(i=0;i<g_index&&flag==no;i++)/*passing all the label table as long as the lebal not found*/
 	{
  if(strcmp(nameParam,g_label[i].label)==0)/*if the label found*/
  		{
   flag=yes;
   if(g_label[i].isExternal!=1)/*if the label is external*/
   {
  
    a.u.kind=ABSOLUTE;
    a.u.destination=(g_label[i].address)-(g_ic)-100;
   }
   
    else
    	{
      printf("Error: cannt use an external lable in this command	Line: %d\n", g_numLine);
      exit(0);
      			}
   		}
   	}
   
   g_code[location]=a.fullReg;
  
 
 if (flag==no)/*if the label don't found in the label chart*/
 {
  printf("Error:  the variable %s wasn't defined as a label.	Line: %d\n",nameParam,g_numLine);
  g_err=yes;
  checkMethod(fp2);
 }
}
int checkMethodRegister(FILE* fp2,char nameParam[])/*this function getting a string that include register and place to insert and return what a register number*/
{
 int i;
 for(i=48;i<56;i++)
 {
  if(nameParam[1]==i)
  {
   return i-'0';
  }
 }
 return -1;
}

