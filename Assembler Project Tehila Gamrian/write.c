#include "project.h"
#include "global.h"

/* these function gets lines of code and write down in a new file the lines of code in the special language of the project.*/  
void writeObject(FILE* fp2)
{
 int i;
 long int j; 
 unsigned long int sum; 
 unsigned int mask;
 
 
 fprintf(fp2,"%7d %d\n",g_ic,g_dc);/* print the multiple lines of the data and code.*/
 for(i=0;i<g_ic;i++)
	{
	
	fprintf (fp2, "%07d", i+100);/*printing the address +100*/
	fputc('\t',fp2); 
	j=24;/*Function for converting the line of code to decimal with masks*/
	sum=0;
	mask=(1<<23);
	while (j>0) 
		{ 
	if(!((g_code[i]) & mask))
	j--; 
	else 
			{
		sum += pow(2,j-1); 
		j--; 
			} 
	mask>>=1; 
		}
	fprintf(fp2,"%.6lx", sum);
	fputc('\n',fp2); 
	}

 for(i=0;i<g_dc;i++)
  	{
  	
	fprintf (fp2, "%07d", i+100+g_ic);/*printing the address +100*/
	fputc('\t',fp2);/*Function for converting the line of code to decimal with masks*/
  	j=24;
	sum=0;
	mask=(1<<23);
	while (j>0) 
		{ 
		if(!((g_data[i].variable)&mask))
		j--; 
		else 
			{ 
		
		sum += pow(2,j-1); 
		j--; 
			}
		  mask>>=1;
  		}
  fprintf(fp2,"%.6lx", sum);
		fputc('\n',fp2);
	}
}

/* this function gets a pointer to a new created file, and write down the all entry labels that are written by the input file.*/
void writeEntry(FILE* fp2)
{
  int i;
  
  for(i=0;i<g_index;i++)
  {
   if(g_label[i].isExternal==2)/* if the label is marked as an entry label write it down in the new file.*/
   {
    fprintf(fp2,"%s",g_label[i].label);
    fputc('\t',fp2);
    fprintf(fp2,"%07d",g_label[i].address);/* print the address in the memory of the entry label.*/
    fputc('\n',fp2);
   }
  }
}

/* this function gets a pointer to a new created file, and write down the all extern labels that are written by the input file.*/
void writeExternal(FILE *fp2)
{
 int i=0;
 for(i=0;i<g_countExt;i++)
 {
  fprintf(fp2,"%s",g_ext_label[i].label);/* if the label is marked as an entry label write it down in the new file.*/
  fputc('\t',fp2);
  fprintf(fp2,"%07d",g_ext_label[i].address+100);/* print the address in the memory of the extern label.*/
  fputc('\n',fp2);
  
  
  
 }
}
   

