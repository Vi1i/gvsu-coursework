from scapy.all import *
import codecs
import threading
routerName = ""

class processInPack(threading.Thread):
	def __init__(self, port):
		threading.Thread.__init__(self)
		self.port = port
	def run(self):
		try:
			while 1:
				receivePack(self.port)
		except KeyboardInterrupt:
			sys.exit()

def receivePack(port):
    serversocket = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
    serversocket.setsockopt( socket.SOL_SOCKET, socket.SO_REUSEADDR, 1 )
	#ListenforPacket
    sercersocket.bind( ( socket.gethostname( ), port ) )
    serversocket.listen( 5 );
    currentSocket = serversocket.accept( )
    buf = 1024
    data = currentSock.recv( buf )
    currentSock.close( )
    serverSocket.close( )
	
    #Get destination out of packet
    
	#Pass to getNextIP for finding the physical IP of destination
	#Send packet on to next destination
	#send ICMP pack back to source with a failure or sucess flag
    #code needs to be type 3, code 7, for unreachable host; or type 11, code 0 for ttl failure. Depending on the situation.
    icmp = ICMP()
    icmp.type = 
    icmp.code = 
    icmpMessage = IP( dst="WHERE WE WANT IT TO SEND BACK" ) / icmp / #Then data goes here.
	pass	

def getNextIP(virtIP):
	for i in range ( 1, 6 ):
		try:
			routerName = "host-10" + str( routerNum ) + ".txt"
			with codecs.open( routerName , 'r', encoding = 'utf8' ) as f:
   				for line in f:
					print(f.readline())
			f.close()
		except:
			pass

def checkValidRouter(routerNum):
	for i in range(1,6):
		try:
			routerName = "router-" + str(routerNum) + ".txt"
			codecs.open(routerName)
			return None
		except:
			pass
	print ("No router was found with number " + str(routerNum) + ", closing")
	sys.exit()
#start threads here
routerNum = raw_input( 'What is your router Number: \n' )
checkValidRouter(routerNum)
print ("Valid Router number, router now operational")
port = 5214
try:
	receiveThreading = processInPack(port)
	receiveThreading.start()
except Exception:
	print "thread Error"
	print Exception
	sys.exit()
