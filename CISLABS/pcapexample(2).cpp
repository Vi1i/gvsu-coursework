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

class IPType{   
   public:
	void increment(){
		++count;
}
   string name;
   int count;
};
vector<IPType> IPTypesV ;
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

void process_pkt(u_char *junk, const struct pcap_pkthdr *h, const u_char *bytes){
	char foo [h->len];
	int pH = 0;  
//printf("Packet length: %d\n",h->len);
 // printf("test %d\n", h->ts);
        
//	for (int i=0; i<h->len; i++){
//	printf("%x" bytes[i]);
//	foo[pH]=bytes[i];
 //       pH++;

//}
//
	
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
	

printf("\n");
}

int main(int argc, char *argv[]){
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
  EIICount =0;
  Count802=0;
  pcap_loop(pcap,500,process_pkt,NULL);
  printf("EII Count: %d \n 802 Count: %d\n", EIICount, Count802);
 // Printing IP Type Vector
  	for(int i=0; i< IPTypesV.size();++i){
		cout << "Name: " << IPTypesV[i].name << "Count: " << IPTypesV[i].count << "\n";
	} 
  return 0;
}
