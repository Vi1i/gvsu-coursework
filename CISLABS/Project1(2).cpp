/* Project 1 for CIS 457
 * Written by:
 * Lucas Crandle
 * Brendan Koning
 */


#include <cstdio>
#include <pcap.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;
int EIICount;
int Count802;
int packetCount;
int byteCount;
int totalPairs;
int ipv4Count;
int tcpCount;
int udpCount;
int icmpCount;
class IPType{   
   public:
	void increment(){
		++count;
}
   string name;
   int count;
};

class NetworkProtocol{
	public:
		void addByte(int len){
			byteCount+=len;
		} 
	string protName;
	int byteCount;
};

class sourceDest{
	public:
		string source;
		string dest;
		int count;
		void increment(){
			++count;
		}
};

class TCPsource{
	public: 
		string source;
		int count;
		void increment(){
			++count;
		}
};

class TCPdest{
	public:
		string dest;
		int count;
		void increment(){
			++count;
		}
};

class UDPsource{
	public: 
		string source;
		int count;
		void increment(){
			++count;
		}
};

class UDPdest{
	public:
		string dest;
		int count;
		void increment(){
			++count;
		}
};


vector<IPType> IPTypesV;
vector<NetworkProtocol> networkProtocolsV;
vector<sourceDest> sourceDestPairsV;
vector<TCPsource> sourceTCP;
vector<UDPsource> sourceUDP;
vector<TCPdest> destTCP;
vector<UDPdest> destUDP;

void incrementTCPSource(string source){

	for(int i=0;i<sourceTCP.size();++i){
		if(source.compare(sourceTCP[i].source)==0){
			sourceTCP[i].increment();
			return;
		}
	}
	TCPsource temp;
	temp.source = source;
	temp.count = 1;
	sourceTCP.push_back(temp);
}

void incrementUDPSource(string source){

	for(int i=0;i<sourceUDP.size();++i){
		if(source.compare(sourceUDP[i].source)==0){
			sourceUDP[i].increment();
			return;
		}
	}
	UDPsource temp;
	temp.source = source;
	temp.count = 1;
	sourceUDP.push_back(temp);
}

void incrementTCPDest(string dest){

	for(int i=0;i<destTCP.size();++i){
		if(dest.compare(destTCP[i].dest)==0){
			destTCP[i].increment();
			return;
		}
	}
	TCPdest temp;
	temp.dest = dest;
	temp.count = 1;
	destTCP.push_back(temp);
}

void incrementUDPdest(string dest){

	for(int i=0;i<destUDP.size();++i){
		if(dest.compare(destUDP[i].dest)==0){
			destUDP[i].increment();
			return;
		}
	}
	UDPdest temp;
	temp.dest = dest;
	temp.count = 1;
	destUDP.push_back(temp);
}
	

void incrementLinkCounter(string EthernetType){
        if (EthernetType.compare("II")== 0)
        { ++EIICount;
	}
        else if(EthernetType.compare("802")==0) {
        ++Count802;}
       
	 else{
        printf("COUNT INCREMENT ERROR");
	cout << EthernetType<< "\n";
}}

void incrementIPType(string CurrName){
	
	for(int i=0; i<IPTypesV.size(); ++i){
		if( CurrName.compare(IPTypesV[i].name)==0){
			IPTypesV[i].increment();
			return;
		}
	
	}
	
 	IPType temp;
	temp.count = 1;
	temp.name = CurrName;
	IPTypesV.push_back(temp);
}

void incrementByteCount(string tProtname, int len){
	for(int i=0;i<networkProtocolsV.size();++i){
		if(tProtname.compare(networkProtocolsV[i].protName)==0){
			networkProtocolsV[i].addByte(len);
			return;
		}
	}
	NetworkProtocol temp;
	temp.protName = tProtname;
	temp.byteCount = len;
	networkProtocolsV.push_back(temp);
}

void incrementSourceDestPair(string source, string dest){
	for(int i=0;i<sourceDestPairsV.size();++i){
		if(source.compare(sourceDestPairsV[i].source)==0){
			if(dest.compare(sourceDestPairsV[i].dest)==0){
				sourceDestPairsV[i].increment();
				return;
			}
		}
	}
	sourceDest temp;
	temp.count = 1;
	temp.source = source;
	temp.dest = dest;
	sourceDestPairsV.push_back(temp);
	totalPairs++;
}

void process_pkt(u_char *junk, const struct pcap_pkthdr *h, const u_char *bytes){
	++packetCount;  
	byteCount+= h->len;	
//	printf("%x : %x : %x : %x : %x",bytes[12], bytes[13], bytes[14], bytes[15], bytes[16]);
  	
	if( bytes[12]>= 6){
//	printf("Ethernet II connection");
	incrementLinkCounter("II");
	char tempName[4];
	sprintf(tempName,"%02x%02x", bytes[12], bytes[13]);
	string name(tempName);
//	cout << name;
	incrementIPType(name);
	}
	if(bytes[12] <= 6){
//	printf("802.3 connection");
	incrementLinkCounter("802");
	}
//byteCount calculation
	char tempPname[2];
	sprintf(tempPname, "%02x", bytes[14]);
	string tProtname(tempPname);
	incrementByteCount(tProtname,h->len);	
//source destination calculation
	if(tProtname.compare("45")==0){
		++ipv4Count;
		char tsource[50];
		char tdest[50];
		sprintf(tsource, "%d.%d.%d.%d",bytes[26],bytes[27],bytes[28],bytes[29]); 
		sprintf(tdest, "%d.%d.%d.%d", bytes[30],bytes[31],bytes[32],bytes[33]);
		string source(tsource);
		string dest(tdest);
		
		incrementSourceDestPair(source, dest);
//			printf("%x",bytes[23]);
		if(bytes[23]==17){
			++udpCount;
			incrementUDPSource(tsource);
			incrementUDPdest(tdest);		
		}else if(bytes[23]==6){
			incrementTCPSource(tsource);
			incrementTCPDest(tdest);			
			++tcpCount;
		}else if(bytes[23]==1){
			++icmpCount;
		}
	}
}


bool wayToSortUDPS(const UDPsource obj1, const UDPsource obj2){
	return  obj1.count > obj2.count;
}

bool wayToSortUDPD(const UDPdest obj1, const UDPdest obj2){
	return  obj1.count > obj2.count;
}

bool wayToSortTCPS(const TCPsource obj1, const TCPsource obj2){
	return  obj1.count > obj2.count;
}

bool wayToSortTCPD(const TCPdest obj1, const TCPdest obj2){
	return  obj1.count > obj2.count;
}


int main(int argc, char *argv[]){
//Error Checking
  if(argc!=2){
    fprintf(stderr, "Usage: %s <pcapfile>\n",argv[0]);
 return 1;
  }
  char err[PCAP_ERRBUF_SIZE];
  pcap_t *pcap = pcap_open_offline(argv[1], err);
 if(pcap == NULL){
    fprintf(stderr, "Error: %s\n", err);
   return 1;
  }
//Set counters = 0
  EIICount =0;
  Count802=0;
  packetCount = 0;
  byteCount = 0;
  totalPairs = 0;
  tcpCount =0;
  udpCount=0;
  icmpCount=0;
  ipv4Count=0; 
//Add values to vectors that are needed.
//IPv4
 NetworkProtocol temp;
	temp.protName = "45";
	temp.byteCount = 0;
	networkProtocolsV.push_back(temp);
//IPv6
NetworkProtocol temp2;
	temp2.protName = "60";
	temp2.byteCount=0;
	networkProtocolsV.push_back(temp2);
//Process packtes	
  
	int errorCheck = pcap_loop(pcap,-1,process_pkt,NULL);
  	if (errorCheck == 0){
		cout << "All packets calculated with success";
	}else if (errorCheck == -1){
	cout << "ERROR OCCURED IN LOOP, CANCELLING";
	return 0;
	} else if (errorCheck == -2){
		cout << "Loop didn't complete all packets, pcap_breakloop() was called";
	}
	cout << "\nSpecification 2: \nEthernet II Percent: " << fixed << std::setprecision(2) << EIICount*100.00/packetCount << "%\nEthernet 802.3 Percent:" << Count802*100.00/packetCount << "%\n";
 // Printing IP Type Vector
 	printf("\nSpecification 3:\nEthernet II type Values and Number:\n");
  	for(int i=0; i< IPTypesV.size();++i){
		cout << "Ethernet Type: " << IPTypesV[i].name << " Percentage of EII packets: " << fixed << std::setprecision(2) << (IPTypesV[i].count*100.00/EIICount) << "%\n";
	}
//print percentage of network layer protocol bytes
 	cout << "\nSpecification 4:\nBytes Transferred on Network Layer Protocols:\n";

	cout <<	"Percentage Transferred with IPv4: " << fixed << std::setprecision(2) << networkProtocolsV[0].byteCount*100.00/byteCount << "%\n";
	
	cout << "Percentage Transferred with IPv6: " << fixed << std::setprecision(2) << networkProtocolsV[1].byteCount*100.00/byteCount << "%\n";

//Look through rest of vector for network Protocols.
	for(int j=2;j<networkProtocolsV.size();++j){
	cout << "Percentage Transferred with Protocol "<< fixed << std::setprecision(2) <<networkProtocolsV[j].protName<<": "<< (networkProtocolsV[j].byteCount*100.00/byteCount)<<"%	\n";
	}
	cout << "\n";
//print out source and destination pairs.	
	cout << "Specification 5: \n";		
	for(int z=0;z<sourceDestPairsV.size();++z){
		cout<<"Pair:\n Source: " << fixed << std::setprecision(2) << sourceDestPairsV[z].source << "\n Destination:" << sourceDestPairsV[z].dest <<"\n Percentage: "<< (sourceDestPairsV[z].count*100.00/ipv4Count)<<"%\n"<<" Count: " << sourceDestPairsV[z].count<<"\n\n";
	}
// Print percentage of transport layer protocols.
	cout << "\nSpecification 6:";
	cout << "\nTransport Layer protocols:\n"<< fixed << std::setprecision(2) << "TCP Count: " << tcpCount << " TCP Percentage: " << (tcpCount*100.00/ipv4Count) << "%\nUDP count: " << udpCount<< " UDP Percentage: " << (udpCount*100.00/ipv4Count)<< "%\nICMP Count: " << icmpCount << " ICMP Percentage: " << (icmpCount*100.00/ipv4Count) << "%\n"; 


sort(sourceTCP.begin(), sourceTCP.end(), wayToSortTCPS);
sort(sourceUDP.begin(), sourceUDP.end(), wayToSortUDPS);
sort(destTCP.begin(), destTCP.end(), wayToSortTCPD);
sort(destUDP.begin(), destUDP.end(), wayToSortUDPD);


cout << "\n";
cout << "\nSpecification 7: ";
cout << "\nTCP Sources: " << "\n";
for (int i = 0; i < sourceTCP.size() && i < 5; ++i){
	cout << i+1 << ". " << sourceTCP[i].source << " Count: " << sourceTCP[i].count << " Percent: " << (sourceTCP[i].count*100.00/tcpCount) <<"%\n";
}
cout << "\n";
cout << "UDP Sources: " << "\n";
for (int i = 0; i < sourceUDP.size() && i < 5; ++i){
	cout << i+1 << ". " << sourceUDP[i].source << " Count: " << sourceUDP[i].count << " Percent: " << (sourceUDP[i].count*100.00/udpCount) <<"%\n";
}
cout << "\n";
cout << "TCP Destinations: " << "\n";
for (int i = 0; i < destTCP.size() && i < 5; ++i){
	cout << i+1 << ". " << destTCP[i].dest << " Count: " << destTCP[i].count << " Percent: " << (destTCP[i].count*100.00/tcpCount) <<"%\n";
}
cout << "\n";
cout << "UDP Destinations: " << "\n";
for (int i = 0; i < destUDP.size() && i < 5; ++i){
	cout << i+1 << ". " << destUDP[i].dest << " Count: " << destUDP[i].count << " Percent: " << (destUDP[i].count*100.00/udpCount) <<"%\n";
}


cout << "Created by: \n Lucas Crandle \n Brendan Koning\n";


 return 0;
}
