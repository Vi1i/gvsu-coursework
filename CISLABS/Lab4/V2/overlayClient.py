import threading
from scapy.all import *
import codecs
import socket
import sys

class sendThread(threading.Thread):
	def __init__(self, ipAddress):
		threading.Thread.__init__(self)
		self.ipAddress = ipAddress
	def run(self):
		try:
			while 1:
				sendMessage(self.ipAddress)
		except:
			print ("send thread exception")
		sys.exit()

class receiveThread(threading.Thread):
	def __init__(self, ipAddress):
		threading.Thread.__init__(self)
		self.ipAddress = ipAddress
	def run(self):
		try:
			while 1:
				receiveMessage(self.ipAddress)
		except:
			print ("receive thread exception")
			sys.exit()
		sys.exit()
	
def getDefaultIP( ownIP ):
	#This loops through host files and checks to see if the virtual ip
    #address is the one specified, then returns the UDP physical IP
    #address for sending.
	for i in range ( 1, 6 ):
		try:
			hostName = "host-10" + str( i ) + ".txt"
			with codecs.open( hostName, 'r', encoding = 'utf8' ) as f:
   				text1 = f.readline( )
				text2 = f.readline( )
			#print ( text1[ 8 :- 1 ] )
			#print ( text2[ 15 :- 1 ] )
			if( text1[ 8:-1 ] == ownIP ):
				return ( text2[ 15 :-1 ] )
		except:
			pass
			#print( "NoFile with name: " + hostName )
	print( "No IP address was found, Closing program" )
	sys.exit( )
	
#Client thread for sending
def sendMessage(ipAddress):
	 
	#Creates the "data" packet, taking the IP address and message and
    		#puts it into a packet
	sendIP = raw_input( 'Enter a virtual IP Address to send to: \n' )

	#Gives users a way to exit the loop
	if (sendIP == "EXIT"):
		sys.exit()

	#checks to see if IP address is valid, exits function if invalid
	try:
		socket.inet_aton(sendIP)
	except socket.error:
		print("Illegal IP address")
		return None

	sendMessage = raw_input( 'Enter a message to send: \n' )
	if (sendMessage == "EXIT"):
		sys.exit()

	#Shouldn't the dest be the physical IP address of the router? (ipAddress variable not sendIP)
	sendPacket = IP(version = 4, dst=sendIP, ihl=21) / UDP(dport=5214, len=len(sendMessage)) / sendMessage
	
	#This converts our entire packet into a hex string
	rawData = str( sendPacket )
	size = len(rawData)
	
	#This is done again to get correct length and checksum values
	newPacket = IP(version = 4, dst=ipAddress, ihl=21, len = size) / UDP(dport=5214, len=size-20) / sendMessage
	del newPacket[IP].chksum
	del newPacket[UDP].chksum
	newData = str(newPacket)
	newPacket = IP(newData)
	
	#newPacket.show()
	#hexdump(newPacket)
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)	
	sock.sendto(newData, (ipAddress, 5214))
	print("Message sent to router")
	
#Client thread for receiving
def receiveMessage(ipAddress):
	#create an INET, STREAMing socket
	serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	serversocket.bind((socket.gethostname(), 5216))
	serversocket.listen(5)	
	currentSocket = serversocket.accept()
	buf = 1024
	data = currentSocket.recv(buf)
	currentSock.close()
	serverSocket.close()
	print "LOOK BELOW\n"
	print data


#start threads here
print("Welcome to Lucas, Brandon, and Lawrence's message program \n enter EXIT anytime to leave.")
ownIP = raw_input( 'Define your IP Address: \n' )
defSendIP = getDefaultIP( ownIP )
print( "Your default router is: " + defSendIP )
#print sys.version
try:
	receiveThreading = receiveThread(defSendIP)
	sendThreading = sendThread(defSendIP)
	receiveThreading.start()
	sendThreading.start()
except:
	print "thread Error"
	sys.exit()


