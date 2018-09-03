#include "utility.h"

#define CLI_SEND  "cli_send"
#define CLI_REC "cli_rec"

//Client code
int main() {
        int sockfd;
        int end_flag, send_flg, recv_flg;
        FILE *readfd, *writefd;
        
        //servaddr: used by server to send
        struct sockaddr_in servaddr, cliaddr;
        
        message rev_msg, send_msg = {0,0,0,0,0,{0}};
        end_flag = send_flg = 1;
        recv_flg = 0; //send_flg << 1;

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

        sockaddr_init(&servaddr, &cliaddr); 

        /*if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) 
                        < 0 )
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }*/
        
        //open the send and recv file descriptor
        readfd = fopen(CLI_SEND, "rb");
        writefd = fopen(CLI_REC, "ab");
        if (!readfd || !writefd) {
                perror("unable to open file!\n");
                exit(EXIT_FAILURE);
        }

        while(end_flag){
                int ret;
                end_flag = recv_flg | send_flg;
                //if still has content to send 
                if (send_flg){
                        //read cli file content and send it out 
                        reset_message(&send_msg, 1);
                        ret = readfile(&send_msg, readfd);
                        if (ret < 0){
                                perror("read file error\n");
                                exit(EXIT_FAILURE);
                        }
                        //readfile success, still has content to send
        
                        else{
                                ret = udp_send_msg(servaddr, sockfd, &send_msg, &send_flg);
                                if (ret < 0)
                                        exit(EXIT_FAILURE);
                        } 
                }

                //if still has content to recv
                if (recv_flg){
                        //received server file content
                        reset_message(&rev_msg, 1);
                        ret = udp_rec_msg(&servaddr, sockfd, &rev_msg, &recv_flg, writefd);
                        if (ret < 0)
                                exit(EXIT_FAILURE);
                        else if (ret > 0 )
                                continue;
                }
        }

        fclose(readfd);
        fclose(writefd);
        return 0;
}
