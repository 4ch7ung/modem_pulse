#include "modem_func.h"

void mget_selfnumber_error(const char* message)
{
	printf("%s\n", message);
	exit(1);
}

char * mget_selfnumber(const char * modem)
{
	FILE * ifile;
    FILE * ofile;
    char buff[256];
	char * tmp;
	char * selfnumber = NULL;

    ifile = fopen(modem, "r");
    ofile = fopen(modem, "a+");
    if(ifile == NULL)
        minit_session_error("Couldn't open device file for reading.");
    if(ofile == NULL)
    {
        fclose(ifile);
        minit_session_error("Couldn't open device file for writing.");
    }
	
	fputs("AT+CNUM\r", ofile);
    printf("[INFO] %s: AT+CNUM\n", modem);
    while(ifile)
    {
        fgets(buff,256,ifile);
        if(!strncmp(buff,"+CNUM",5))
        {
            strtok(buff,"\"");
            strtok(NULL, "\"");
        	strtok(NULL, "\"");
            tmp = strtok(NULL, "\"");
            printf("[INFO] %s: %s\n", modem, tmp);
            selfnumber = strdup(tmp);
            break;
        }
        if(!strncmp(buff,"ERROR",5))
            mget_selfnumber_error(buff);
    }
    return selfnumber;
}
