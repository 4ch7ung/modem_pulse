#include "modem_func.h"

void mserver_error(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

void mserver(const char* modem)
{
	FILE * ifile;
	FILE * ofile;
	char buff[256];
	char cmd[256];
	char phone[256];
	
	ifile = fopen(modem, "r");
	ofile = fopen(modem, "a+");
	if(ifile == NULL)
		mserver_error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		mserver_error("Couldn't open device file for writing.");
	}
	
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"+CMT",4))
		{
			strncpy(phone,buff+7,12);
			printf("[INFO] %s: Message from %s\n", modem, phone);
			fgets(buff,256,ifile);
			buff[strlen(buff)-2] = 0;
			printf("[INFO] %s: %s\n", modem, buff); 
			sprintf(cmd,"AT+CMGS=\"%s\"\r",phone);
			fputs(cmd,ofile);
			while(ifile)
			{
				fgets(buff,256,ifile);
				if(!strncmp(buff,"AT+CMGS",7))
					break;
			}
			sprintf(cmd,"%s\032","Some tea");
			fputs(cmd,ofile);
			while(ifile)
			{
				fgets(buff,256,ifile);
				if(!strncmp(buff,"+CMGS",5))
				{
					printf("[INFO] %s: +CMGS %s ", modem, phone);
					break;
				}
				if(!strncmp(buff,"+CMS ERROR",10))
				{
					printf("[ERROR] %s: %s", modem, buff);
					exit(1);
				}
			}
			while(ifile)
			{
				fgets(buff,256,ifile);
				if(!strncmp(buff,"OK",2))
				{
					printf("OK!\n");
					break;
				}
				if(!strncmp(buff,"+CMS ERROR",10))
				{
					printf("[ERROR] %s: %s", modem, buff);
					exit(1);
				}
			}				
		}
	}
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
}
