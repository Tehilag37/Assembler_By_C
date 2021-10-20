#include "project.h"
#include "global.h"
int flag_label;/* check if the variable is realy a label.*/
/* this project get the file that the user enter and read it, while reading the function count how many place the file need for the data and for the commands.*/
void parsing(FILE *fp1)
{
 char arr [MAX_WORD];
 int isDirective;/* a flag that mark if the line is adirective.*/
 char ch=fgetc(fp1);
 g_numLine=0;/* a counter of lines.*/
 flag_label=0;
 g_ic=START_ADD;/* counter of code.*/
 g_dc=0,/* counter of data.*/
 g_index=0;
 g_err=0;
  while(ch!=EOF)/* if the file didn't reach end of file' read the next line.*/
  	{
    fseek(fp1,-1,SEEK_CUR);/* go back one line in order to read the all word.*/
    fscanf(fp1,"%s",arr);/* read the first word.*/
	
    flag_label=0;
    g_numLine++;
    if(arr[0]==';')/* if the line is an empty line read untill the end' and go back.*/
    		{ 
      fgets(line,MAX_LINE,fp1);
      ch=fgetc(fp1);
      continue;
    		}
 
   if(arr[strlen(arr)-1]==':')/* if the first word is defined as a label.*/
   		{ 
      char str[MAX_WORD];
      strncpy(str,arr,strlen(arr)-1);/* put in the array in a special string in order to check.*/
      
      str[strlen(arr)-1]='\0';

      flag_label=yes;/* the flag of labels got on.*/
      if(!(isalpha(arr[0])))/* if the first letter is not an alphabet letter it is an error.*/
      			{
       printf("Error:  the label has to start with a letter ,and %s doesn't start with a letter.	Line: %d\n",arr,g_numLine);
       g_err=yes;
       fgets(line,MAX_LINE,fp1);
       ch=fgetc(fp1);
       continue;
      			}
      
      if(isLabelLegal(str,g_index,fp1))/* send to a function that checks if the label is defined legaly.*/
      			{
       ch=fgetc(fp1);
       continue;
      			}
      if(g_index==0)/* if there isn't any code before,  build up a place of memory.*/
      			{
       g_label=(symbols*)malloc(sizeof(symbols));
       if(!g_label)
       				{
        printf("Error: memory allocation failed.	Line: %d\n", g_numLine);
        exit(0);
       				}
      			}
      else
      			{
       g_label=(symbols*)realloc(g_label,(g_index+1)*sizeof(symbols));/* continue building up place for the next labels.*/
       if(!g_label)
       				{
        printf("Error: memory allocation failed.	Line: %d\n", g_numLine);
        exit(0);
       				}
      			}
      strncpy(g_label[g_index].label,arr,strlen(arr)-1);
      g_label[g_index].label[strlen(arr)-1]='\0';
      fscanf(fp1,"%s",arr);/* read the next word.*/
		}
     
   isDirective= checkDirective(arr, fp1);/* send to a function that checks if the line is directive or not.*/
   if(isDirective!=1)/*it doesnt directive */
     checkCode(arr,fp1);/* send to a function that checks if it is code.*/
   if(flag_label==1 ) /* if there is a label precede the counter.*/ 
    g_index++;
   ch=fgetc(fp1);
  }
between();/* send to a function before going over the second time.*/
}
/*  these function gets the file to read and the first word in the line, and checks if it's a directive line.*/
int checkDirective(char arr[],FILE *fp1)
{ 
  if(strcmp(arr,".string")==0||strcmp(arr,".data")==0)/* if the word is data or string, put in the right data in the label structure.*/
  	{
   if(flag_label)
   		{
    g_label[g_index].address=g_dc;
    g_label[g_index].isCode=no;
    g_label[g_index].isExternal=no;
   		}
   if(strcmp(arr,".data")==0)/* if is data, send to a function that read data.*/
     checkData(fp1);
     
   if(strcmp(arr,".string")==0)/* if it is a string, send to the function that read strings.*/
     checkString(fp1);
   return 1;
  	}
  if(strcmp(arr,".extern")==0||strcmp(arr,".entry")==0)/* if the word is entry, or extern check if the data after it is legal.*/
  	{
   char buffer [MAX_WORD];
   char ch= checkSpace(fp1);
   if(ch=='\n'||ch==EOF)/* if after entry or extern the file or line ends, it is an error.*/
   		{
     printf("Error: after %s has to be one parameter	Line: %d\n",arr,g_numLine);
     g_err=yes;
     buffer[0]='\0';
     return 1;
   		}
   fseek(fp1,-1,SEEK_CUR);
   fscanf(fp1,"%s",buffer);/* read the next word.*/
   ch= checkSpace(fp1);
   if(ch!='\n'&&(fp1))/* if after the second word the line didn't finish it's an error.*/
   		{  
     fgets(line,MAX_LINE,fp1);
     printf("Error: after %s has to be just one parameter	Line: %d\n",arr,g_numLine);
     g_err=yes;
     return 1;
   		}     
   if(strcmp(arr,".extern")==0)   /* if the type is extern' check if it's legal.*/
   		{
    if(!(isalpha(buffer[0])))/* if the first letter is not an alphabet letter it's an error.*/ 
    			{
       printf("Error:  the label has to start with a letter ,and %s doesn't start with a letter.	Line: %d\n",buffer,g_numLine);
       g_err=yes;
       fgets(line,MAX_LINE,fp1);
       return 1;
    			}
    if(isLabelLegal(buffer,g_index,fp1))/* check if the label is legal.*/
     return 1;
    if(g_index==0)
    			{
     g_label=(symbols*)malloc(sizeof(symbols));/* build up a place for the labels.*/
     if(!g_label)
     				{
      printf("Error: memory allocation failed.	Line: %d\n", g_numLine);
      exit(0);
     				}
    			}
    else/* if there is place for the labeles, continue it.*/
    			{
     g_label=(symbols*)realloc(g_label,sizeof(symbols)*(g_index+1));
     if(!g_label)
     				{
      printf("Error: memory allocation failed.	Line: %d\n", g_numLine);
      exit(0);
     				}
    			}
    strcpy(g_label[g_index].label,buffer);/* put in the data in the arrey.*/
    g_label[g_index].label[strlen(buffer)]='\0';
    g_label[g_index].address=0;
    g_label[g_index].isExternal=yes;
    g_label[g_index].isCode=0;
    flag_label=yes;
   	}
   	
   if(flag_label==yes&&strcmp(arr,".entry")==0)/* if his type is entry there isn't a label now.*/
   {
    strcpy(g_label[g_index].label,"\0");
    flag_label=no;
   }
   return 1;
  }
  return 0;
}
/* these function gets the first word of the line, and the file to read, and check if the code is legal. */
void checkCode(char arr[],FILE *fp1)  
{
 int place;
char ch;
   int count=0;
   int param1,param2;
 place= whichCode(arr);/* check the place of the command.*/
 if(place==-1)
 	{ 
  printf("Error: the command has no meaning .	Line: %d \n",g_numLine);
  g_err=yes;
  fgets(line,MAX_LINE,fp1);
  return;
 	}
 if(flag_label)/* if the code is a label.*/
 	{
  g_label[g_index].address=g_ic;
  g_label[g_index].isCode=yes;
  g_label[g_index].isExternal=no; 
 	}
 g_ic++;
 
   param1= checkMethod(fp1);/* read the first parameter and check its type.*/
   fseek(fp1,-1,SEEK_CUR);
   ch=checkSpace(fp1);
   if(param1!=-1)/* if there is one parameter and then empty space. the number of parameters is one.*/
   		{
    if(ch=='\n'||!fp1)
     count=1;
    else
			{ 
     if(ch!=',')/* if there isn't a comma(,) between the parameters it is an error.*/
				{
      printf("Error: there is not comma (,) between the paramters of the command %s.	Line: %d\n",arr,g_numLine);
      g_err=yes; 
      fgets(line,MAX_LINE,fp1);
      return;
				}
     else
     				{
      param2= checkMethod(fp1);/* send the second parameter to the checking function.*/
      if(param2==-1)/* if there isn't any parameter after the comma it's an error.*/
      					{
       printf("Error: there is not any paramater after the comma %s.	Line: %d\n",arr,g_numLine);
       g_err=yes; 
       return;
      					}
      count=2;
      fseek(fp1,-1,SEEK_CUR);
      ch=checkSpace(fp1);
      if(ch!='\n'&&ch!=EOF)/* if there is more parameters count them.*/
      					{
       checkMethod(fp1);
       count++;
      					}
     				}
    			}
  		}
       
  if(count>cmd[place].oper)/* if the parameters are more than it has to be its an error.*/
  		{
    printf("Error: too much paremetrs for the command %s.	Line: %d\n",cmd[place].name,g_numLine);
    g_err=yes;
    return;
  		}
  if(count<cmd[place].oper)/* if there are less parameters from what it has to be' it is an error.*/
  		{
    printf("Error: too little paremetrs for the command %s.	Line: %d \n",cmd[place].name,g_numLine);
    g_err=yes;
    return;
   		}
   if(count==1)
   		{
    if(cmd[place].permit[1][param1]==0)/* if the command cann't use the type of a spesific parameter it is an error.*/
    			{
     printf("Error: the method is not available for the command %s.	Line: %d\n",cmd[place].name,g_numLine);
     g_err=yes;
     return;
    			}
   		}
   if(count==2)
   		{
    if(cmd[place].permit[0][param1]==0||cmd[place].permit[1][param2]==0)/* if the command cann't use the type of a spesific parameter it is an error.*/
    			{
      printf("Error: the method is not available for the command %s.	Line: %d\n",cmd[place].name,g_numLine);
      g_err=yes;
      return;
    			}
   		}
   if(count==1&&param1!=REGISTER)/* procede the counter of code according to the parameters.*/
     g_ic++; 
   if(count==2)
   		{
    if((param1!=REGISTER&&param2==REGISTER)||(param1==REGISTER&&param2!=REGISTER))/*only one of the operands is a register*/
     g_ic++;
    if(param1!=REGISTER&&param2!=REGISTER)/*both  operands are not registers*/
     g_ic+=2;
   		}
  }


