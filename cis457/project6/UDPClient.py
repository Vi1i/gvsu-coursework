import socket
from scapy.all import *
import sys
import select
import re
import os

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

def endTrans():
	sock.close()
	final = open(fileName, 'wb')
	for z in range(0, int(re.match(r'\d+', fileData[2]).group())):
		f = open(".package" + str(z), 'rb')
		final.write(f.read())
		f.close()
	final.close()
	os.system('rm .package*')
	print "File downloaded"
	sys.exit()

serverIP = sys.argv[1] #127.0.0.1
serverPort = int(re.match(r'\d+', sys.argv[2]).group()) #9876

#ownIP = socket.gethostbyname(socket.gethostname())
ownIP = "10.0.0.2"
fileData = []

def getCheckSum(source, dest, port, leng, data):
        tempPacket = IP(version=4, src=source, dst=dest)/UDP(dport=port, len = leng)/data
        del tempPacket[IP].chksum
        del tempPacket[UDP].chksum
        tempPacket = tempPacket.__class__(str(tempPacket))
        #tempPacket.show2()
        return tempPacket[UDP].chksum

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
buf = 1024
fileName = raw_input("Which file do you want to download?")
message = fileName

message = str(getCheckSum(ownIP, serverIP, serverPort, 1000, message)) + '\n' + message

sock.sendto(message, (serverIP, int(serverPort)))

noReply = 1
#This replies with if the file is on the server
while(noReply):
	fileData = []
	data, addr = sock.recvfrom(buf)
	
	originSum = data.split('\n', 2)[0]
	data = '\n'.join(data.split('\n', 1)[1:])
	if str(originSum) == str(getCheckSum(addr[0], ownIP, serverPort, 1000, data)):
		if data.split('\n', 1)[0] == 'Info':
			for z in range(0,4):
				fileData.append(data.split('\n', 4)[z + 1])
			message = 'Info'
			message = str(getCheckSum(ownIP, serverIP, serverPort, 1000, message)) + '\n' + message
			sock.sendto(message, (serverIP, int(serverPort)))
			noReply = 0
		else:
			print "File is not on the server"
			sock.close()
			sys.exit(0)
		
comfirmPort = fileData[3]
packetData = [0] * int(re.match(r'\d+', fileData[2]).group())

#Opens the file to be written to and gets the first set of data
print "File info:\n\t" + "File: " + str(fileData[0]) + '\n\t' + "Bytes: " + str(fileData[1]) + '\n\t' + "Packages: " + str(fileData[2])
try:
	while(1):
		sock.settimeout(5)
		data, addr = sock.recvfrom(buf)

		oldChckSum = data.split('\n', 2)[0]
		data = '\n'.join(data.split('\n', 1)[1:])
		newChckSum = getCheckSum(addr[0], ownIP, serverPort, 1000, data)

		if(str(oldChckSum) == str(newChckSum)):
			print data.split('\n', 1)[0]
			if data.split('\n', 1)[0] == 'EOF':
				endTrans()
				
			packageNumber = int(re.match(r'\d+',(data.split('\n', 1)[0])).group())
			if packageNumber % 2 == 0:
				print bcolors.OKGREEN + str(packageNumber) + bcolors.ENDC
			else:
				print bcolors.OKBLUE + str(packageNumber) + bcolors.ENDC
				
			f = open(".package" + str(packageNumber), 'wb')
			f.write(data.split('\n', 1)[1])
			f.close()
			mesg = str(packageNumber)
			mesg = str(getCheckSum(ownIP, serverIP, int(comfirmPort), 1000, mesg)) + '\n' + mesg
			sock.sendto(mesg, (serverIP, int(comfirmPort)))

except socket.timeout:
	sock.close()
	os.system('rm .package*')
	print "Connection to server was interupted"
	sys.exit()
