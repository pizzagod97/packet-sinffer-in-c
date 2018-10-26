#include<stdio.h>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>
#include<netdb.h>
#include<net/if.h>
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
 #include <linux/if_packet.h>
# define size 2048
void main()
{
	
	//htons(ETH_P_IP) FOR ALL PACKETS
	int sockid = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_IP));
	struct ifreq ifr;
	strcpy(ifr.ifr_name,"wlp2s0");
	ifr.ifr_flags |= IFF_PROMISC;
      ioctl(sockid, SIOCSIFFLAGS, &ifr);

        struct packet_mreq mr;
        memset(&mr, 0, sizeof(mr));
       mr.mr_ifindex = ifr.ifr_ifindex;
        mr.mr_type = PACKET_MR_PROMISC;
       setsockopt(sockid, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr));
  
	unsigned char *buf = (unsigned char *)malloc(size);
	struct sockaddr_in list;
	while(1)
	{
	memset(buf,0,size);
	int buflen = recvfrom(sockid,buf,size,0,NULL,NULL);
	struct ethhdr *eth = (struct ethhdr *) buf;
	puts(" ");
	puts("Ethernet header : ");
	printf("source mac : ");
	for(int i=0;i<6;i++)
	printf(" %.2X:",eth->h_source[i]);
	printf("destination mac :");
        for(int i=0;i<6;i++)
	printf(" %.2X:",eth->h_dest[i]);
       puts(" ");
	printf("%d protocol \n",eth->h_proto);
	struct iphdr *ip = (struct iphdr *) (buf + sizeof(struct ethhdr));
	memset(&list,0,sizeof(list));
	list.sin_addr.s_addr  = ip->saddr;
	printf("source ip : %s ",inet_ntoa(list.sin_addr));
	memset(&list,0,sizeof(list));
        list.sin_addr.s_addr = ip->daddr;	
	printf("destination ip : %s ",inet_ntoa(list.sin_addr));
	printf("protocol : %d  ",(unsigned int)ip->protocol);
        printf("Type Of Service : %d  ",(unsigned int)ip->tos);
        printf( "Total Length : %d Bytes ",ntohs(ip->tot_len));
        printf( "Identification : %d  ",ntohs(ip->id));
	int iphdrlen = ip->ihl*4;
        struct tcphdr *tcp=(struct tcphdr*)(buf + iphdrlen + sizeof(struct ethhdr));
	printf(" Source port : %d   ",ntohs(tcp->source));
	printf(" Destination port : %d",ntohs(tcp->dest));
	puts("The payload : ");
	unsigned char * data = buf + ip->ihl*4 + sizeof(struct ethhdr) + sizeof(struct tcphdr);
        int remain = buflen - ip->ihl*4 + sizeof(struct ethhdr) + sizeof(struct tcphdr);
       // for(int i=0;i<remain;i++)
//	printf("%.2X ",*(data+i));
        for(int i=0;i<100;i++)
	printf("_");
	}

}


