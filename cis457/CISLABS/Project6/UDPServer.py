import socket
import os
from scapy.all import *
import binascii

Server_IP = "127.0.0.1"
port = 9876

recieveSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sendSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
recieveSocket.bind(("", port))
print "Listening on port: ", port

while 1:
	fileName, addr = recieveSocket.recvfrom(1024)
	#This gets all of the files in the current directory
	#Type is list
	fileList = os.listdir(os.getcwd())
	
	#Checks if the requested file is in the current server directory
	if fileName in fileList:
		recieveSocket.sendto("Sending file", addr)
		#'rb' is read binary, which allows us to read a file as bytes
		with open(fileName, 'rb') as file:
			buf = file.read(1000)
			#Gets 1000 bytes at a time and sends them, then gets the next 1000 bytes in the file
			while (buf):
				recieveSocket.sendto(buf, addr)
				buf = file.read(1000)
				print "Sending..."
			print "File Sent"
	else:
		recieveSocket.sendto("File does not exist", addr)
				
			
			
