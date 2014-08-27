#include "modem_func.h"

void msend_ussd_error(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

void convertUTF16BE_to_UTF8(const char *instr, char *outstr)
{
	char buff[4];
	unsigned long i;
	int j;
	unsigned long k;
	unsigned long instrlen;
	int outchar;
	
	instrlen = strlen(instr);
	if(instrlen % 4)
	{
		printf("Seems like a mistake, string has Odd number of symbols\n");
		outstr[0] = 0;
		return;
	}
	k = 0;
	for(i = 0; i < instrlen; i+=4)
	{
		for(j = 0; j < 4; ++j)
			buff[j] = instr[i+j];
		outchar = 0;
		if(buff[2] >= '0' && buff[2] <= '9') outchar += (buff[2]-'0')*16;
		if(buff[2] >= 'A' && buff[2] <= 'F') outchar += (buff[2]-'A' + 10)*16;
		if(buff[3] >= '0' && buff[3] <= '9') outchar += (buff[3]-'0');
		if(buff[3] >= 'A' && buff[3] <= 'F') outchar += (buff[3]-'A' + 10);
		outstr[k++] = (char)outchar;
	}
	outstr[k] = 0;
}

void msend_ussd(const char * modem, const char * ussd)
{
	FILE * ifile;
	FILE * ofile;
	char buff[1000];
	char cmd[256];
	char * msg;
	char imei[5];
	
	strncpy(imei,modem+strlen(modem)-4,4);
	ifile = fopen(modem, "r");
	ofile = fopen(modem, "a+");
	if(ifile == NULL)
		msend_ussd_error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		msend_ussd_error("Couldn't open device file for writing.");
	}
// Start of USSD send	
	sprintf(cmd,"AT+CUSD=1,%s,15\r",ussd);
	fputs(cmd,ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"AT+CUSD",7))
			break;
		if(!strncmp(buff,"ERROR",10))
		{
			printf("[ERROR] %s: %s", imei, buff);
			exit(1);
		}
	}
// End of USSD send
// Start of USSD wait
	printf("[INFO] %s: Waiting for USSD response ... \n", imei);
	while(ifile)
	{
		fgets(buff,1000,ifile);
		if(!strncmp(buff,"+CUSD",5))
		{
			strtok(buff,"\"");
			msg = strtok(NULL, "\"");
			//convertUTF16BE_to_UTF8(msg, cmd);
			printf("[INFO] %s: %s\n", imei, msg);
			break;
		}
	}
// End of USSD wait
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
}
