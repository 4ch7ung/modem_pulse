#include "modem_func.h"

void msend_single_command_error(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

void msend_single_command(const char * modem, const char * command)
{
	FILE * ifile;
	FILE * ofile;
	char imei[4];
	char cmd[256];
	
	strncpy(imei,modem+strlen(modem)-4,4);
	ifile = fopen(modem, "r");
	ofile = fopen(modem, "a+");
	if(ifile == NULL)
		msend_single_command_error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		msend_single_command_error("Couldn't open device file for writing.");
	}
	sprintf(cmd,"%s\r",command);
	fputs(cmd,ofile);
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
}
