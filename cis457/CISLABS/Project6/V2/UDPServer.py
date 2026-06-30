import socket
import os
from scapy.all import *
import binascii

Server_IP = "127.0.0.1"
port = 9876
RESPONSE_PORT = 9898
MAX_PACKET_SIZE = 1000

recieveSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sendSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
recieveSocket.bind(("", port))
print "Listening on port: ", port

while 1:
	fileName, addr = recieveSocket.recvfrom(1024)
	#This gets all of the files in the current directory
	#Type is list
	fileList = os.listdir(os.getcwd())
	print "Recieved request from: " + str(addr) + " For file: " + fileName	
	#Checks if the requested file is in the current server directory
	if fileName in fileList:
		print "File exists, getting info..."
		fileSize = os.path.getsize(fileName)
		packetNumber = (fileSize/(MAX_PACKET_SIZE))+1
		sendInfo = "Info\n" + str(fileName) +"\n" + str(fileSize) + "\n" + str(packetNumber) + "\n" + str(RESPONSE_PORT) 
		recieveInfo = "not Received"
		while(recieveInfo != "Info"):
			print "Sending:\n" + sendInfo
			recieveSocket.sendto(sendInfo, addr)
			recieveInfo,addr = recieveSocket.recvfrom(1024)
			print "Recieved: " + recieveInfo
		sendPacketNumber = 0	
		#'rb' is read binary, which allows us to read a file as bytes
		with open(fileName, 'rb') as file:
			buf = file.read(MAX_PACKET_SIZE)
			#Gets 1000 bytes at a time and sends them, then gets the next 1000 bytes in the file
			while (buf):
				buf = str(sendPacketNumber) + "\n" + buf
				sendPacket = IP(version=4, dst=addr[0])/UDP(dport=9876, len = 1000) / buf
				del sendPacket[IP].chksum
				del sendPacket[UDP].chksum	
				sendPacket.show2()
				sendPacket = sendPacket.__class__(str(sendPacket))
				checkSum =  sendPacket[UDP].chksum
				#print checkSum
				buf = str(checkSum) + "\n" + buf
				print "Sending packet Number: " + str(sendPacketNumber)
				recieveSocket.sendto(buf, addr)
				buf = file.read(MAX_PACKET_SIZE)
				sendPacketNumber += 1
				print "Sending..."
			print "File Sent"
			file.close()
	else:
		recieveSocket.sendto("File does not exist", addr)
	
			
			
