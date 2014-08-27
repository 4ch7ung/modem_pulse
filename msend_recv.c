#include "modem_func.h"

void msend_recv_error(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

void msend_recv(const char* modem, const char* phone, const char* message, const char* save_dir)
{
	FILE * ifile;
	FILE * ofile;
	FILE * savefile;
	char buff[256];
	char cmd[256];
	char msg[256];
	char sphone[256];
	char rphone[256];
	char imei[5];
	char savedir[256];
		
	strncpy(imei,modem+strlen(modem)-4,4);
	strcpy(sphone,phone);
	strcpy(savedir,save_dir);
	ifile = fopen(modem, "r");
	ofile = fopen(modem, "a+");
	if(ifile == NULL)
		msend_recv_error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		msend_recv_error("Couldn't open device file for writing.");
	}
// Start of SMS send	
	sprintf(cmd,"AT+CMGS=\"%s\"\r",phone);
	fputs(cmd,ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		printf("[INFO] %s: %s", imei, buff);
		if(!strncmp(buff,"AT+CMGS",7))
			break;
	}
	sprintf(msg,"%s\032",message);
	fputs(msg,ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"+CMGS",5))
		{
			printf("[INFO] %s: +CMGS ... ", imei);
			break;
		}
		if(!strncmp(buff,"+CMS ERROR",10))
		{
			printf("[ERROR] %s: %s", imei, buff);
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
			printf("[ERROR] %s: %s", imei, buff);
			exit(1);
		}
	}
// End of SMS send
// Start of SMS wait
	printf("[INFO] %s: Waiting for SMS reaponse ... ", imei);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"+CMT",4))
		{
			strncpy(rphone,buff+7,12);
			rphone[12] = 0;
			if(strcmp(rphone,phone))
			{
				printf("NO\n[INFO] %s: Recieved message from %s\n", imei, rphone);
				fgets(buff,256,ifile);
				printf("\t%s",buff);
			}
			else
			{
				printf("YES\n[INFO] %s: Received message from %s\n", imei, rphone);
				fgets(buff,256,ifile);
				printf("\t%s",buff);
				//buff[strlen(buff)-2] = 0;
				strcat(savedir,"/");
				strcat(savedir,imei);
				savefile = fopen(savedir, "w+");
				if(savefile)
				{
					fputs(buff,savefile);
					fclose(savefile);
					break;
				}
				else
				{
					printf("[ERROR] %s: Fail to open save file for writing\n",imei);
					break;
				}
			}
		}
	}
// End of SMS wait
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
}
