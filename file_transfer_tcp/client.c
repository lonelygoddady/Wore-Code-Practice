#include "utility.h"

#define CLI_SEND  "cli_send"
#define CLI_REC "cli_rec"

//Client code
int main(int argc, char *argv[]) {
        int sockfd, ret;
        FILE *readfd;
        
        if (argc != 2){
                perror("Please input correct argurments\n");
                exit(EXIT_FAILURE);
        }

        //servaddr: used by server to send
        struct sockaddr_in remoteaddr;
        
        message send_msg = {0,0,0,0,0,{0}};

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

        //sockaddr_init(&servaddr, &cliaddr); 
        memset(&remoteaddr, 0, sizeof(remoteaddr));

        remoteaddr.sin_family = AF_INET;
        remoteaddr.sin_addr.s_addr = INADDR_ANY;
        remoteaddr.sin_port = htons(SEVPORT);
        bzero(&(remoteaddr.sin_zero), 8);

        if (connect(sockfd, (struct sockaddr *)&remoteaddr, sizeof(remoteaddr)) 
                        < 0 )
        {
                perror("connect failed\n");
                exit(EXIT_FAILURE);
        }
        
        //open the send and recv file descriptor
        readfd = fopen(argv[1], "rb");
        if (NULL == readfd){
                perror("read file failure!\n");
                exit(EXIT_FAILURE);
        } 

        while((ret = readfile(&send_msg, readfd))){
                reset_message(&send_msg, 0);        
                send_msg.len = ret;
                //packet_process(&send_msg, 1);
                ret = send(sockfd, &send_msg, sizeof(message), 0);
                if (ret < 0){
                        perror("send failure\n");
                        exit(EXIT_FAILURE);
                }
#ifdef DEBUG 
                printf("send %u bytes from client\n", ret);
#endif
        }

        fclose(readfd);
        return 0;
}
