#include <cstdio>
#include <pcap.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <cstring>
#include <iostream>
#include <vector>


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

vector<IPType> IPTypesV;
vector<NetworkProtocol> networkProtocolsV;
vector<sourceDest> sourceDestPairsV;

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
	temp.byteCount =0;
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
	temp.count = 0;
	temp.source = source;
	temp.dest = dest;
	sourceDestPairsV.push_back(temp);
	totalPairs++;
}

void process_pkt(u_char *junk, const struct pcap_pkthdr *h, const u_char *bytes){
	++packetCount;  
	byteCount+= h->len;	
	printf("%x : %x : %x : %x : %x",bytes[12], bytes[13], bytes[14], bytes[15], bytes[16]);
  	
	if( bytes[12]>= 6){
	printf("Ethernet II connection");
	incrementLinkCounter("II");
	char tempName[4];
	sprintf(tempName,"%02x%02x", bytes[12], bytes[13]);
	string name(tempName);
	cout << name;
	incrementIPType(name);
	}
	if(bytes[12] <= 6){
	printf("802.3 connection");
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
			printf("%x",bytes[23]);
		if(bytes[23]==17){
			++udpCount;		
		}else if(bytes[23]==6){
			++tcpCount;
		}else if(bytes[23]==1){
			++icmpCount;
		}
	}

printf("\n");
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
  pcap_loop(pcap,500,process_pkt,NULL);
  printf("EII Count: %d \n 802 Count: %d\n", EIICount, Count802);
 // Printing IP Type Vector
 	printf("Ethernet II type Values and Number:\n");
  	for(int i=0; i< IPTypesV.size();++i){
		cout << "Name: " << IPTypesV[i].name << " Percentage of packets: " << (IPTypesV[i].count*100/EIICount) << "\n";
	}
//printing the percentages	
	printf( "Percentage of Ethernet II packets: %d\n", ((EIICount*100)/packetCount));
	printf( "Percentage of IEEE 802.3 Packets: %d\n", ((Count802*100)/packetCount));
 //print percentage of network layer protocol bytes
 	cout << "Ethernet type Values:\n";
	printf("Percentage of IPv4 Packets: %d\n", (networkProtocolsV[0].byteCount*100/byteCount));
	printf("Percentage of Ipv6 Packtes: %d\n", (networkProtocolsV[1].byteCount*100/byteCount));
//Look through rest of vector for network Protocols.
	for(int j=2;j<networkProtocolsV.size();++j){
	cout << "Percentage of "<<networkProtocolsV[j].protName<<" Packets: "<< (networkProtocolsV[j].byteCount/byteCount)<<"\n";
	}
//print out source and destination pairs.	
	for(int z=0;z<sourceDestPairsV.size();++z){
		cout<<"Pair:\n Source: " << sourceDestPairsV[z].source << "\n Destination:" << sourceDestPairsV[z].dest <<"\n Percentage: "<< (sourceDestPairsV[z].count*100/ipv4Count)<<"\n"<<"Count: " << sourceDestPairsV[z].count<<"\n";
	}
// Print percentage of transport layer protocols.
	cout << "Transport Layer protocols:\n"<< "TCP Count: " << tcpCount << " TCP Percentage: " << (tcpCount*100/ipv4Count) << "\nUDP count: " << udpCount<< " UDP Percentage: " << (udpCount*100/ipv4Count)<< "\nICMP Count: " << icmpCount << " ICMP Percentage: " << (icmpCount*100/ipv4Count) << "\n"; 

 return 0;
}
