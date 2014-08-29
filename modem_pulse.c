#include <Python.h>

#include "modem_func.h"

static PyObject *
get_imei(PyObject * self, PyObject * args)
{
	const char * modem;
	char * imei;
	PyObject * result;
	
	if(!PyArg_ParseTuple(args, "s", &modem))
		return NULL;
	imei = mget_imei(modem);
 	result = Py_BuildValue("s#",imei,4);
 	free(imei);
 	return result;
}

static PyObject *
init_session(PyObject * self, PyObject * args)
{
	const char * modem;
	const char * Russian;
	if(!PyArg_ParseTuple(args, "ss", &modem, &Russian))
		return NULL;
	minit_session(modem,Russian);
	Py_RETURN_NONE;
}

static PyObject *
server(PyObject * self, PyObject * args)
{
	const char * modem;
	if(!PyArg_ParseTuple(args, "s", &modem))
		return NULL;
	mserver(modem);
	Py_RETURN_NONE;
}

static PyObject *
read_message(PyObject * self, PyObject * args)
{
	const char * modem;
	if(!PyArg_ParseTuple(args, "s", &modem))
		return NULL;
	mread_message(modem);
	Py_RETURN_NONE;
}

static PyObject *
send_sms(PyObject * self, PyObject * args)
{
	const char * modem;
	const char * phone;
	const char * message;
	if(!PyArg_ParseTuple(args, "sss", &modem, &phone, &message))
		return NULL;
	msend_sms(modem,phone,message);
	Py_RETURN_NONE;
}

static PyObject *
send_recv(PyObject * self, PyObject * args)
{
	const char * modem;
	const char * phone;
	const char * message;
	const char * save_dir;
	
	char * response;
	PyObject * result;
	
	if(!PyArg_ParseTuple(args, "ssss", &modem, &phone, &message, &save_dir))
		return NULL;
	response = msend_recv(modem,phone,message,save_dir);
	result = Py_BuildValue("s",response);
	free(response);
	return result;
}

static PyObject *
send_ussd(PyObject * self, PyObject * args)
{
	const char * modem;
	const char * ussd;
	char * response;
	PyObject * result;
	
	if(!PyArg_ParseTuple(args, "ss", &modem, &ussd))
		return NULL;
	response = msend_ussd(modem,ussd);
	result = Py_BuildValue("s",response);
	free(response);
	return result;
}

static PyObject *
recv_message(PyObject * self, PyObject * args)
{
	const char * modem;

	char * response;
	PyObject * result;
	
	if(!PyArg_ParseTuple(args, "s", &modem))
		return NULL;
	response = mrecv_message(modem);
	
	result = Py_BuildValue("ss",response, response+13);
	free(response);
	return result;
}


static PyMethodDef modem_methods[] = {
	{"get_imei", (PyCFunction)get_imei, METH_VARARGS, "Prints the IMEI number of the given <modem>"},
	{"init_session", (PyCFunction)init_session, METH_VARARGS, "Inits session for <modem>"},
	{"server", (PyCFunction)server, METH_VARARGS, "Launches server on <modem>"},
	{"read_message", (PyCFunction)read_message, METH_VARARGS, "Reads messages from <modem>"},
	{"send_sms", (PyCFunction)send_sms, METH_VARARGS, "Sends sms from <modem> to <phone> with text <message>"},
	{"send_recv", (PyCFunction)send_recv, METH_VARARGS, "Sends sms from <modem> to <phone> with text <message> and waits for reply to save in <save_dir>"},
	{"send_ussd", (PyCFunction)send_ussd, METH_VARARGS, "Sends from <modem> ussd request <ussd> and prints the response"},
	{"recv_message", (PyCFunction)recv_message, METH_VARARGS, "Recieves incoming message on <modem> and returns its contents"},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initmodem_pulse() {
	Py_InitModule3("modem_pulse", modem_methods, "Module for using ZTE modems");
}
