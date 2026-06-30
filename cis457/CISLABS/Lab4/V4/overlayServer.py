from scapy.all import *
import codecs
import threading
import socket
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
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host = socket.gethostname()
    s.bind((host, port))
    s.listen(1)
    c, addr = s.accept()
    print 'Got connection from', addr
    data = c.recv(1024)
    pack = IP(str(data))
    print pack.show()
    nextDest = getNextIP(str(pack.dst))
    if(pack.ttl ==0):
	print ("time to live is 0, failed")
	sys.exit
    elif(nextDest == "0.0.0.0"):
	#send Fail ICMP packet
	print("Ip address specified not found")
	sys.exit
    else:
    	pack.ttl = pack.ttl-1			
    	#Send packet on to next destination
    	r = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    	rHost = nextDest
    	print "sending packet to: " +str(nextDest) +" with ttl="+ str(pack.ttl)
    	r.connect((nextDest,port))
    	r.send(str(pack))
    	r.close	
    	#send ICMP pack back to source with a failure or sucess flag
   	#code needs to be type 3, code 7, for unreachable host; or type 11, code 0 for ttl failure. Depending on the situation.
    	icmp = ICMP()
    	icmp.type =3 
    	icmp.code =7
    	#icmpMessage = IP( dst="148.61.112.116" ) / icmp()
    	pass	

def getNextIP(virtIP):
	for i in range ( 1, 6 ):
		try:
			routerName = "host-10" + str( routerNum ) + ".txt"
			with codecs.open( routerName , 'r', encoding = 'utf8' ) as f:
   				for line in f:
					IP = f.readline()
					print(IP)
					if(IP[:6]=="address"):
						pass
					if(IP[:5]=="prefix"):
						len = int(IP[16:17])
						len = len/8
						vIP = (IP[6:14]).split(".")
						inVIP = virtIP.split(".")
						for i in range(0,len):
							if(len(i) != inVIP(i)):
								break
							if(i==len):
								return IP[19:-1]
						
			f.close()
		except:
			pass
	return "0.0.0.0"

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
