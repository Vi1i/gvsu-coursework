import threading
from scapy.all import *
import codecs
import socket

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
def sendThread( ):
	 
	#Creates the "data" packet, taking the IP address and message and
    #puts it into a packet
	sendIP = raw_input( 'Enter an IP Address to send to: \n' )
	sendIP = "192.168.1.1"
	sendMessage = raw_input( 'Enter a message to send: \n' )
	sendPacket = IP(version = 4, dst=sendIP, ihl=21) / UDP(dport=5214, len=len(sendMessage)) / sendMessage
	
	#This converts our entire packet into a hex string
	rawData = str( sendPacket )
	size = len(rawData)
	
	#This is done again to get correct length and checksum values
	newPacket = IP(version = 4, dst=sendIP, ihl=21, len = size) / UDP(dport=5214, len=size-20) / sendMessage
	del newPacket[IP].chksum
	del newPacket[UDP].chksum
	newData = str(newPacket)
	newPacket = IP(newData)
	
	#newPacket.show()
	#hexdump(newPacket)
	#TODO: Check the config file to determine what IP to send to based
    #on the user entered IP
	overlayIP = "127.0.0.121"
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.sendto(newData, (overlayIP, 5214))
	
#Client thread for receiving
#def receiveThread( ):
	#create an INET, STREAMing socket
	#serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	#serversocket.bind((socket.gethostname(), 5214))
	#print socket.gethostname()
	#serversocket.listen(5)	
	#buf = 1024
	#while(1):
	#	currentSocket = serversocket.accept()
	#	data = currentSocket.recv(buf)
	#	print "LOOK BELOW\n"
	#	print data
		


#start threads here
#ownIP = raw_input( 'Enter your IP Address: \n' )
#defSendIP = getDefaultIP( ownIP )
#print( "Sending IP:" + defSendIP )
try:
	#recieve = threading.Thread( target = receiveThread )
	#recieve.start( )
	sendThread = threading.Thread( target = sendThread )
	sendThread.start( )
	
except:
	print "thread Error"
	raise
