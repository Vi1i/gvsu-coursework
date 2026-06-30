import socket

Server_IP = "127.0.0.1"
port = 9876

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(("", port))
print "Listening on port: ", port
while 1:
	data, addr = s.recvfrom(1024)
	print data
	


