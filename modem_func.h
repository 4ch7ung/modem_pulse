#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mget_imei(const char * modem);
void mget_imei_error(const char * message);

void minit_session(const char * modem, const char * Russian);
void minit_session_error(const char* message);

void msend_recv(const char* modem, const char* phone, const char* message, const char* save_dir);
void msend_recv_error(const char* message);

void msend_sms(const char* modem, const char* phone, const char* message);
void msend_sms_error(const char* message);

void mserver(const char* modem);
void mserver_error(const char* message);

void msend_ussd(const char * modem, const char * ussd);
void msend_ussd_error(const char * message);

void mread_message(const char * modem);
void mread_message_error(const char * message);