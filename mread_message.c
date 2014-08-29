#include "modem_func.h"

void mread_message_error(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

void mread_message(const char * modem)
{
	FILE * ifile;
	FILE * ofile;
	char buff[1000];
	char cmd[256];
	char msg[1000];
	char *pch;
	char imei[5];
	
	strncpy(imei,modem+strlen(modem)-4,4);
	ifile = fopen(modem, "r");
	ofile = fopen(modem, "a+");
	if(ifile == NULL)
		mread_message_error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		mread_message_error("Couldn't open device file for writing.");
	}
// Start of cmd send	
	sprintf(cmd,"AT+CMGL=\"ALL\"\r");
	fputs(cmd,ofile);
	while(ifile)
	{
		fgets(buff,1000,ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("Done\n");
			break;
		}
		if(!strncmp(buff,"+CMGL:",6))
		{
			pch = strtok(buff,"\",");
			printf("[INFO] %s:", imei);
			while(pch != NULL)
			{
				if(strcmp(pch,"") && strncmp("REC",pch,3))
					printf(" %s", pch);
				pch = strtok(NULL, "\",");
			}
			fgets(msg,1000,ifile);
			printf("{SIZE} %lu\n", strlen(msg));
			msg[strlen(msg)/4 * 4] = 0;
			printf("{NEW SIZE} %lu\n", strlen(msg));
			printf("{MSG} %s\n",msg);
		}
		if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
		{
			printf("[ERROR] %s: %s", imei, buff);
			exit(1);
		}
	}
// End of cmd send
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
}
