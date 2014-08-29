#include "modem_func.h"

void mserver_error(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

char * mrecv_message(const char* modem)
{
	FILE * ifile;
	FILE * ofile;
	char buff[1000];
	char phone[256];
	char * response = NULL;
	
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
		fgets(buff,1000,ifile);
		if(!strncmp(buff,"+CMT",4))
		{
			strncpy(phone,buff+7,12);
			printf("[INFO] %s: Message from %s\n", modem, phone);
			fgets(buff,1000,ifile);
			buff[strlen(buff)-2] = 0;
			printf("[INFO] %s: %s\n", modem, buff); 
			response = malloc((strlen(phone)+strlen(buff))*sizeof(char));
			strcpy(response,phone);
			response[12] = 0;
			strcpy(response+13,buff);
			break;
		}
	}
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
	return response;
}

void mserver(const char* modem)
{
	FILE * ifile;
	FILE * ofile;
	char buff[1000];
	char cmd[256];
	char phone[256];
	char * message = "HUI";
	
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
		fgets(buff,1000,ifile);
		if(!strncmp(buff,"+CMT",4))
		{
			strncpy(phone,buff+7,12);
			printf("[INFO] %s: Message from %s\n", modem, phone);
			fgets(buff,1000,ifile);
			buff[strlen(buff)-2] = 0;
			printf("[INFO] %s: %s\n", modem, buff); 
			
			sprintf(cmd, "AT+CMGS=\"%s\"\r", phone);
			fputs(cmd,ofile);
			while(ifile)
			{
				fgets(buff,1000,ifile);
				printf("[INFO] %s: %s", modem, buff);
				if(!strncmp(buff,"AT+CMGS",7))
					break;
			}
			
			sprintf(cmd,"%s\032",message);
			fputs(cmd,ofile);
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
					exit(1);
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
