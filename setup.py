from distutils.core import setup, Extension
setup(name='modem_pulse', version='1.0', \
	  ext_modules=[Extension('modem_pulse', ['modem_pulse.c', \
	  										 'mget_imei.c', \
	  										 'minit_session.c', \
	  										 'msend_recv.c', \
	  										 'msend_sms.c', \
	  										 'mserver.c', \
	  										 'msend_ussd.c', \
	  										 'mread_message.c', \
											 'mwrite_number.c'])])
