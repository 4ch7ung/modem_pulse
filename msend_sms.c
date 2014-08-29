#include "modem_func.h"

void msend_sms_error(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

void msend_sms(const char* modem, const char* phone, const char* message)
{
	FILE * ifile;
	FILE * ofile;
	char buff[1000];
	char str[1000];
	
	ifile = fopen(modem, "r");
	ofile = fopen(modem, "a+");
	if(ifile == NULL)
		msend_sms_error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		msend_sms_error("Couldn't open device file for writing.");
	}
	
	sprintf(str, "AT+CMGS=\"%s\"\r", phone);
	fputs(str,ofile);
	while(ifile)
	{
		fgets(buff,1000,ifile);
		printf("[INFO] %s: %s", modem, buff);
		if(!strncmp(buff,"AT+CMGS",7))
			break;
	}
	sprintf(str,"%s\032", message);
	fputs(str,ofile);
	while(ifile)
	{
		fgets(buff,1000,ifile);
		if(!strncmp(buff,"+CMGS",5))
		{
			printf("[INFO] %s: +CMGS ... ", modem);
			break;
		}
		if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
		{
			printf("[ERROR] %s: %s", modem, buff);
			return;
		}
	}
	while(ifile)
	{
		fgets(buff,1000,ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("OK!\n");
			break;
		}
		if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
		{
			printf("[ERROR] %s: %s", modem, buff);
			return;
		}
	}
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
}
