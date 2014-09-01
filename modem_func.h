#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * mget_imei(const char * modem);
void mget_imei_error(const char * message);

void minit_session(const char * modem, const char * Russian);
void minit_session_error(const char* message);

char * msend_recv(const char* modem, const char* phone, const char* message, const char* save_dir);
void msend_recv_error(const char* message);

void msend_sms(const char* modem, const char* phone, const char* message);
void msend_sms_error(const char* message);

void mserver(const char* modem);
char * mrecv_message(const char * modem);
void mserver_error(const char* message);

char * msend_ussd(const char * modem, const char * ussd);
void msend_ussd_error(const char * message);

void mread_message(const char * modem);
void mread_message_error(const char * message);

void mwrite_number(const char * modem, const char * phone);
void mwrite_number_error(const char * message);

char * mget_selfnumber(const char * modem);
void mget_selfnumber_error(const char * message);