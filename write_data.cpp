/*
 * @Author: Rex 
 * @Date: 2018-09-25 10:58:51 
 * @Last Modified by:  
 * @Last Modified time: 2018-09-28 16:42:05
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>

#define BUFF_SIZE 2048
static const char *g_szIfName = "ens3f1np1"; // 网卡接口
typedef struct _udphdr                       //定义UDP首部
{
    unsigned short uh_sport; //16位源端口
    unsigned short uh_dport; //16位目的端口
    unsigned int uh_len;     //16位UDP包长度
    unsigned int uh_sum;     //16位校验和
} UDP_HEADER;

int main(int argc, char *argv[])
{
    int sockfd;
    int n;
    char buf[BUFF_SIZE];
    char read_buf[BUFF_SIZE];
    struct sockaddr_ll sll;
    struct ifreq ifr;
    struct ethhdr *eth;
    struct iphdr *iph;
    struct udphdr *udph;
    // UDP_HEADER *udph;

    sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (sockfd == -1)
    {
        printf("sockt error!\n");
        return -1;
    }
    strncpy(ifr.ifr_name, g_szIfName, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1) // 获取网卡信息
    {
        perror("ioctl()");
        close(sockfd);
        return -1;
    }
    // ifr.ifr_flags |= IFF_PROMISC;
    // if(ioctl(sockfd, SIOCSIFFLAGS, &ifr) == -1) // 网卡设置混杂模式
    // {
    //     perror("ioctl()");
    //     close(sockfd);
    //     return -1;
    // }
    memset(&sll, 0, sizeof(sll));
    sll.sll_family = PF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_IP);
    if (bind(sockfd, (struct sockaddr *)&sll, sizeof(sll)) == -1)
    {
        perror("bind()");
        close(sockfd);
        return -1;
    }
    FILE *pFile;
    FILE *fp;
    pFile = fopen("data.txt", "ab+");
    int i = 0;
    while (1)
    {
        if (i++ == 204800)
            break;
        n = recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);
        if (n == -1)
        {
            perror("recvfrom()");
            break;
        }

        // buf[strlen(buf)] = '\0';
        fwrite(buf, sizeof(buf), 1, pFile);
        // fclose(pFile);
        // pFile = fopen("data.txt","rb+");
        // fread(&read_buf,sizeof(read_buf),1,pFile);
        // fclose(pFile);
        // 接收到的数据帧头6字节是目的MAC地址，紧接着6字节是源MAC地址。
        eth = (struct ethhdr *)buf;
        // printf("Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
        // printf("Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
        iph = (struct iphdr *)(buf + sizeof(struct ethhdr));
        // if (iph->version == 4 && iph->ihl == 5)
        // {
        //     printf("version:%d,ihl:%d\n",iph->version,iph->ihl);
        //     printf("Source:%s\n", inet_ntoa(*(struct in_addr *) &iph->saddr));
        //     printf("Dest:%s\n", inet_ntoa(*(struct in_addr *) &iph->daddr));
        // }
        if (iph->protocol == IPPROTO_UDP)
        {

            udph = (struct udphdr *)(buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
            switch (ntohs(udph->dest))
            {
            case 51000:
                fp = fopen("51000.txt", "ab+");
                fwrite(buf, sizeof(buf), 1, fp);
                fclose(fp);
                break;
            case 51001:
                fp = fopen("51001.txt", "ab+");
                fwrite(buf, sizeof(buf), 1, fp);
                fclose(fp);
                break;
            case 51002:
                fp = fopen("51002.txt", "ab+");
                fwrite(buf, sizeof(buf), 1, fp);
                fclose(fp);
                break;
            case 51003:
                fp = fopen("51003.txt", "ab+");
                fwrite(buf, sizeof(buf), 1, fp);
                fclose(fp);
                break;
            case 51004:
                fp = fopen("51004.txt", "ab+");
                fwrite(buf, sizeof(buf), 1, fp);
                fclose(fp);
                break;
            case 51005:
                fp = fopen("51005.txt", "ab+");
                fwrite(buf, sizeof(buf), 1, fp);
                fclose(fp);
                break;
            case 51006:
                fp = fopen("51006.txt", "ab+");
                fwrite(buf, sizeof(buf), 1, fp);
                fclose(fp);
                break;
            }

            // printf("Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
            // printf("Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
            // // printf("version:%d,ihl:%d\n", iph->version, iph->ihl);
            // printf("Source:%s\n", inet_ntoa(*(struct in_addr *)&iph->saddr));
            // printf("Dest:%s\n", inet_ntoa(*(struct in_addr *)&iph->daddr));
            // udph = (struct udphdr *)(buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
            // printf("Source port: %u\n", ntohs(udph->source));
            // printf("Dest port: %u\n", ntohs(udph->dest));
            // printf("udp-len:%d\n", ntohs(udph->len));
            // udph = (UDP_HEADER *)(iph + sizeof(UDP_HEADER));
            // printf("Source port: %u\n", ntohs(udph->uh_sport));
            // printf("Dest port: %u\n", ntohs(udph->uh_dport));
            // printf("len: %u\n", ntohs(udph->uh_len));
            // printf("====================================================\n");
        }
    }
    fclose(pFile);
    close(sockfd);
    return 0;
}

