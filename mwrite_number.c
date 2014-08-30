#include "modem_func.h"

void mwrite_number_error(const char* message)
{
	printf("%s\n", message);
	exit(1);
}

void mwrite_number(const char * modem, const char * phone)
{
	FILE * ifile;
    FILE * ofile;
    char buff[256];
	char cmd[256];

    ifile = fopen(modem, "r");
    ofile = fopen(modem, "a+");
    if(ifile == NULL)
        minit_session_error("Couldn't open device file for reading.");
    if(ofile == NULL)
    {
        fclose(ifile);
        minit_session_error("Couldn't open device file for writing.");
    }
	
	fputs("AT+CPBS=\"ON\"\r",ofile);
    printf("[INFO] %s: AT+CPBS=\"ON\" ... ", modem);
    while(ifile)
    {
        fgets(buff, 256, ifile);
        if(!strncmp(buff,"OK",2))
        {
            printf("OK!\n");
            break;
        }
        if(!strncmp(buff,"+CMS ERROR",10) || !strncmp(buff,"ERROR",5))
            mwrite_number_error(buff);
    }

	sprintf(cmd, "AT+CPBW=1,\"%s\",129,\"SELF\"\r", phone);
	fputs(cmd, ofile);
    printf("[INFO] %s: %s ... ", modem, cmd);
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
