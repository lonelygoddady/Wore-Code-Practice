#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <inttypes.h>

#define SEVPORT 9930
#define CLIPORT 9090
#define MAXLINE 1024
//#define debug

typedef struct {
        uint16_t last;
        uint16_t port;
        uint16_t len;
        uint32_t ip;
        int32_t id;
        uint8_t buffer[MAXLINE];
} message; 

#define MSGOFFSET (sizeof(message) - MAXLINE)

void packet_dump(message* msg);
void packet_process(message* msg, int flag);
void sockaddr_init(struct sockaddr_in* servaddr, struct sockaddr_in* cliaddr);
void reset_message(message *msg, int flag);
int readfile (message *msg, FILE* fd);
int writefile (message *msg, FILE* fd);
int udp_send_msg(struct sockaddr_in cliaddr, int sockfd, message *send_msg, int *send_flg);
int udp_rec_msg(struct sockaddr_in* servaddr, int sockfd, message *rev_msg, int *recv_flg, FILE* writefd);
void hexDump (char *desc, void *addr, int len);
        
