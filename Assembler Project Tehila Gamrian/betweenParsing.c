#include "project.h"
#include "global.h"

void between()
{
 int i=0;
 for(;i<g_index;i++)
  if(g_label[i].isExternal==0&& g_label[i].isCode==0)
   g_label[i].address+=g_ic;
}


