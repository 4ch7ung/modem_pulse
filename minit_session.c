#include "modem_func.h"

void minit_session_error(const char* message)
{
	printf("%s\n", message);
	exit(1);
}

void minit_session(const char * modem, const char * russian)
{
	FILE * ifile;
	FILE * ofile;
	char buff[256];

	ifile = fopen(modem, "r");
	ofile = fopen(modem, "a+");
	if(ifile == NULL)
		minit_session_error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		minit_session_error("Couldn't open device file for writing.");
	}

	fputs("AT+CMGF=1\r",ofile);
	printf("[INFO] %s: AT+CMGF=1 ... ", modem);
	while(ifile)
	{
		fgets(buff, 256, ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("OK!\n");
			break;
		}
		if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
			minit_session_error(buff);
	}
	
	fputs("AT+CPMS=\"SM\",\"SM\",\"SM\"\r", ofile);
	printf("[INFO] %s: AT+CPMS=\"SM\",\"SM\",\"SM\" ... ", modem);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("OK!\n");
			break;
		}
		if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
			minit_session_error(buff);
	}

	fputs("AT+CNMI=1,2,0,0,0\r",ofile);
	printf("[INFO] %s: AT+CNMI=1,2,0,0,0 ... ", modem);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("OK!\n");
			break;
		}
		if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
			minit_session_error(buff);
	}
	
	fputs("AT+CMGD=1,1\r",ofile);
	printf("[INFO] %s: AT+CMGD=1,4 ... ", modem);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"OK",2))
		{
			printf("OK!\n");
			break;
		}
		if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
			minit_session_error(buff);
	}
	if(!strcmp(russian,"YES"))
	{
		printf("[INFO] %s: Russian language support included.\n", modem);
		fputs("AT+CSMP=17,167,0,25\r",ofile);
		printf("[INFO] %s: AT+CSMP=17,167,0,25 ... ", modem);
		while(ifile)
		{
			fgets(buff,256,ifile);
			if(!strncmp(buff,"OK",2))
			{
				printf("OK!\n");
				break;
			}
			if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
				minit_session_error(buff);
		}
		fputs("AT+CSCS=\"UCS2\"\r",ofile);
		printf("[INFO] %s: AT+CSCS=\"UCS2\" ... ", modem);
		while(ifile)
		{
			fgets(buff,256,ifile);
			if(!strncmp(buff,"OK",2))
			{
				printf("OK!\n");
				break;
			}
			if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
				minit_session_error(buff);
		}
	}
	else
	{
		fputs("AT+CSMP=,,0,0\r",ofile);
		printf("[INFO] %s: AT+CSMP=,,0,0 ... ", modem);
		while(ifile)
		{
			fgets(buff,256,ifile);
			if(!strncmp(buff,"OK",2))
			{
				printf("OK!\n");
				break;
			}
			if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
				minit_session_error(buff);
		}
	}
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
}
