#include "modem_func.h"

void mget_imei_error(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

void mget_imei(const char * modem)
{
	FILE * ifile;
	FILE * ofile;
	char buff[256];
	char str[256];

	ifile = fopen(modem, "r");
	ofile = fopen(modem, "a+");
	if(ifile == NULL)
		mget_imei_error("Couldn't open device file for reading.");
	if(ofile == NULL)
	{
		fclose(ifile);
		mget_imei_error("Couldn't open device file for writing.");
	}
	
	fputs("AT+CGSN\r",ofile);
	while(ifile)
	{
		fgets(buff,256,ifile);
		if(!strncmp(buff,"AT+CGSN",7))
		{
			fgets(buff,256,ifile);
			strncpy(str,buff+strlen(buff)-6,4);
			printf("%s\n",str);
			break;
		}
	}
	if(ifile) fclose(ifile);
	if(ofile) fclose(ofile);
}
