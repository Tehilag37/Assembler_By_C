/* all global variables are declated in main.c file and start with g_ to distinguish them.
They are explained in main.c here they are defined so we can use them in other files */
extern symbols* g_label; /* an arrey of structures that contains all the labels that was written in the input.*/
extern unsigned int g_code[];/* an arrey that containes all the codes of the commands.*/
extern struct cmdStruct cmd[];
extern dataMatrix g_data[];/* an arrey that contains the data of the project.*/
extern int g_ic, g_dc, g_index, g_err;/* counters of commands, data, and labels.*/
extern char line [MAX_LINE];
extern int g_numLine;/* a counter of lines in the code.*/
extern int g_flagEntry;/* a flag to mark if there is any entry variables in the file, to know if to build up a file.*/
extern extLabelStruct* g_ext_label;/* a pointer to build up the struct of the labels.*/
extern int g_countExt;/* a counter of the external labels.*/
extern char * regs[];
