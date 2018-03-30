#!/bin/python
# -*- coding: utf-8 -*-

import socket

# 声明套接字对象
s = socket.socket()
host = socket.gethostname()
host = "192.168.2.104"
port = 1234
# 绑定端口
s.bind((host, port))

# 监听端口, 队列容量为5
s.listen(5)
while True:
	# 等待客户端接入
	c, addr = s.accept()
	# print c.recv(1024)
	print 'Got connection from', addr
	print "What can i help you to translate messages?"
	while True:
		try:
			print "input you messages here:"
			msg = raw_input();

			if msg == "exit":
				raise KeyboardInterrupt

			# msg += "\n"
			c.send(msg)
		except KeyboardInterrupt:
			raise KeyboardInterrupt
		except Exception:
			break
	# c.send('\n')
	# c.send('Thank you for connecting')
	# 关闭一个接入
	c.close
	# raise KeyboardInterrupt
