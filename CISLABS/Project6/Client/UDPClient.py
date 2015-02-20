from socket import *
import sys
import select

serverIP = sys.argv[1] #127.0.0.1
serverPort = sys.argv[2] #9876


sock = socket(AF_INET, SOCK_DGRAM)
buf = 1000
message = raw_input("Which file do you want to download?")

sock.sendto(message, (serverIP, int(serverPort)))

#This replies with if the file is on the server
data, addr = sock.recvfrom(buf)
if data == "File does not exist":
	print "File is not on the server"
	sock.close()
	sys.exit(0)

#Opens the file to be written to and gets the first set of data
f = open(message, 'wb')
data, addr = sock.recvfrom(buf)

try:
	while(data):
		f.write(data)
		sock.settimeout(2)
		data,addr = sock.recvfrom(buf)
except timeout:
	sock.close()
	f.close()
	print "File downloaded"
	


