import socket
from scapy.all import *
import sys
import select
import re
import os

serverIP = sys.argv[1] #127.0.0.1
serverPort = sys.argv[2] #9876

ownIP = socket.gethostbyname(socket.gethostname())

fileData = []

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
buf = 1024
fileName = raw_input("Which file do you want to download?")
message = fileName

sock.sendto(message, (serverIP, int(serverPort)))

#This replies with if the file is on the server
data, addr = sock.recvfrom(buf)
if data == "File does not exist":
	print "File is not on the server"
	sock.close()
	sys.exit(0)

if data.split('\n', 1)[0] == 'Info':
	for z in range(0,4):
		fileData.append(data.split('\n', 4)[z + 1])
	message = 'Info'
	sock.sendto(message, (serverIP, int(serverPort)))

comfirmPort = fileData[3]
packetData = [0] * int(re.match(r'\d+', fileData[2]).group())

#Opens the file to be written to and gets the first set of data
print "File info:\n\t" + "File: " + str(fileData[0]) + '\n\t' + "Bytes: " + str(fileData[1]) + '\n\t' + "Packages: " + str(fileData[2])
try:
	while(1):
		sock.settimeout(2)
		data, addr = sock.recvfrom(buf)
		packageNumber = int(re.match(r'\d+',(data.split('\n', 2)[1])).group())
		packetData[packageNumber] = 1#data.split('\n', 1)[1]
		f = open(".package" + str(packageNumber), 'wb')
		f.write(data.split('\n', 2)[2])
		f.close()
		
		oldChckSum = data.split('\n', 2)[0]
		data = '\n'.join(data.split('\n', 1)[1:])
		chckSumPacket = IP(version=4, src=addr[0], dst=ownIP)/UDP(dport=9876, len=1000)/data
		del chckSumPacket[IP].chksum
		del chckSumPacket[UDP].chksum
		chckSumPacket.show2()
		chckSumPacket = chckSumPacket.__class__(str(chckSumPacket))
		newChckSum = chckSumPacket[UDP].chksum
		print str(oldChckSum) + " : " + str(newChckSum)

		sock.sendto(str(packageNumber), (serverIP, int(comfirmPort)))
except socket.timeout:
	sock.close()
	final = open(fileName, 'wb')
	for z in range(0, int(re.match(r'\d+', fileData[2]).group())):
		f = open(".package" + str(z), 'rb')
		final.write(f.read())
		f.close()
		os.remove(".package" + str(z))
	final.close()
	print "File downloaded"
