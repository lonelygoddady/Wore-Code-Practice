#include "utility.h"

#define SERVER_SEND "server_send"
#define SERVER_REC "server_rec"
#define BACKLOG 10

// Server code
int main() {
        int sockfd, cli_fd;
        int  cli_cnt;
        unsigned int sin_size;
        struct sockaddr_in localaddr, cliaddr;
        
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }
 
        sockaddr_init(&localaddr, &cliaddr);

        if (bind(sockfd, (const struct sockaddr *)&localaddr, sizeof(struct sockaddr_in)) 
                        < 0 )
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }
       
        if (listen(sockfd, 10) == -1){
                perror("listen error\n");
                exit(EXIT_FAILURE);
        }

        sin_size = sizeof(struct sockaddr_in);
        cli_cnt = 0;

        while(1){
                int ret;

                cli_fd = accept(sockfd, (struct sockaddr*) &localaddr, &sin_size);
                if (-1 == cli_fd){
                        perror("Obtain new fd error\n");
                }
                else {
                        printf("Receive Connection from %s\n", 
                                        inet_ntoa(cliaddr.sin_addr));
                        cli_cnt ++;
                }
                
                // child process 
                if (!fork()){
                        char file_name[MAXLINE];
                        FILE *clif;
                        message rev_msg = {0,0,0,0,0,{0}};

                        // create filename for this client
                        snprintf(file_name, sizeof(char)*MAXLINE, "clientfile%i", cli_cnt);
                        
                        clif = fopen(file_name, "wb");
                        if (NULL == clif){
                                perror("Client File cannot be written\n");
                                exit(EXIT_FAILURE);
                        }
                       
                        // receive message and write to file 
                        while((ret = recv(cli_fd, (message*) &rev_msg, sizeof(message), 0)) > 0){
                                //packet_process(&rev_msg, 0);
#ifdef DEBUG 
                                printf("receive %d bytes from client\n", ret);
                                packet_dump(&rev_msg);
                                hexDump("recv msg", (char*) &rev_msg, MAXLINE);
#endif
                                //write received msg into file 
                                if (writefile(&rev_msg, clif) == 0){
                                        // write file error 
                                        perror("cannot wite to file! Abort!\n");
                                        exit(EXIT_FAILURE);  
                                }
                                reset_message(&rev_msg, 0);
                        }
                        fclose(clif);
                }

        }

        return 0;
}
