#include "utility.h"

#define SERVER_SEND "server_send"
#define SERVER_REC "server_rec"

// Server code
int main() {
        int sockfd;
        int end_flag, send_flg, recv_flg;
        FILE *readfd, *writefd;
        struct sockaddr_in servaddr, cliaddr;
        
        message rev_msg, send_msg = {0,0,0,0,0,{0}};
        end_flag =  send_flg = 1;
        recv_flg = send_flg << 1;

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }
 
        sockaddr_init(&servaddr, &cliaddr);

        if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) 
                        < 0 )
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }
        
        //open the send and recv file descriptor
        readfd = fopen(SERVER_SEND, "rb");
        writefd = fopen(SERVER_REC, "ab");
        if (!readfd || !writefd){
                perror("unable to open file!\n");
                exit(EXIT_FAILURE);
        }

        while(end_flag){
                int ret;
                end_flag = recv_flg | send_flg;
                
                if (recv_flg){
                        //received client file content
                        reset_message(&rev_msg, 0);
                        ret = udp_rec_msg(&cliaddr, sockfd, &rev_msg, &recv_flg, writefd);
                        if (ret < 0)
                                exit(EXIT_FAILURE);
                        else if (ret > 0 )
                                continue;
                }
                
                if (send_flg){
                        //read server file content and send it out 
                        reset_message(&send_msg, 0);
                        ret = udp_send_msg(cliaddr, sockfd, &send_msg, &send_flg);
                        if (ret < 0)
                                exit(EXIT_FAILURE);
                } 
        }

        fclose(readfd);
        fclose(writefd);
        return 0;
}
