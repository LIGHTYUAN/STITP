#!/bin/python
# -*- coding: utf-8 -*-

import socket

s = socket.socket()
host = socket.gethostname()
host = "192.168.2.104"
port = 1234
s.bind((host, port))

s.listen(5)
while True:
	c, addr = s.accept()
	# print c.recv(1024)
	print 'Got connection from', addr
	print "What can i help you to translate messages?"
	print "input you messages here:"
	msg = raw_input();
	while True:
		try:
			print "input you messages here:"
			msg = raw_input();
			c.send(msg)
		except KeyboardInterrupt:
			raise KeyboardInterrupt
		except Exception:
			break
	# c.send('\n')
	# c.send('Thank you for connecting')
	c.close
	# raise KeyboardInterrupt
