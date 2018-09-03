#include "utility.h"

void packet_dump(message* msg){
#ifdef DEBUG
        printf("\n\n=====packet dump=====\n");
        printf("last packet: %hu\n", msg->last);
        printf("packet port: %hu\n", msg->port);
        printf("packet id: %u\n", msg->id);
        printf("packet actual len: %hu\n", msg->len);
        printf("====================\n\n\n");
#endif
}

void sockaddr_init(struct sockaddr_in* servaddr, struct sockaddr_in* cliaddr){
        memset(servaddr, 0, sizeof(servaddr));
        memset(cliaddr, 0, sizeof(cliaddr));

        servaddr->sin_family = AF_INET;
        servaddr->sin_addr.s_addr = INADDR_ANY;
        servaddr->sin_port = htons(SEVPORT);
        
        cliaddr->sin_family = AF_INET;
        cliaddr->sin_addr.s_addr = INADDR_ANY;
        cliaddr->sin_port = htons(CLIPORT);
}

void reset_message(message *msg, int flag){
        msg->last = 0;
        msg->id = 0;
        msg->len = 0;
        msg->port = (flag == 0) ? SEVPORT : CLIPORT; // flag 0 for server, 1 for cli
        msg->ip = 0;
        msg->port = htons(msg->port);
        memset(&msg->buffer, 0, MAXLINE);
}

int readfile (message *msg, FILE* fd) {
        uint16_t n;
        static int32_t count = 0;
        
        n = fread(msg->buffer, 1, MAXLINE, fd);
        if (n == 0 && !feof(fd)){
                perror("read file error!\n");
                return -1;
        }
        else if (n > 0){ //has count been read
                msg -> id = htonl(count++); //increment the msg id
                msg -> len =  htons(n); //record actual bytes been read
        }
        else if (n == 0){
                msg -> id = htonl(-1); //-1 for last packet
                msg -> last = htons(1);
                printf("mark the last packet!\n");
        }
#ifdef DEBUG 
        printf("read %u bytes from file\n", n);
#endif
        return n;
}

int writefile (message *msg, FILE* fd){
        int n;
        
        printf("msg len: %d\n", msg->len);
        n = fwrite(msg->buffer, 1, msg->len, fd);
        if (n < 0){
                perror("write file error!\n");
                return -1;
        }
        fflush(fd);
#ifdef DEBUG
        printf("write %d bytes to file\n", n);
#endif
        return n;
}

int udp_send_msg(struct sockaddr_in addr, int sockfd, message *send_msg, int *send_flg){
        printf("Send Packet! id: %u\n", send_msg->id);
        //packet_dump(send_msg); 
        hexDump("Packet", send_msg, sizeof(message));
        if ((sendto(sockfd, &send_msg, sizeof(send_msg), 
                        MSG_CONFIRM, (const struct sockaddr*) &addr,
                        sizeof(struct sockaddr_in))) < 0) {
                printf("Sendto error!\n");
                return -1;
        }

        //no more contents to send, unset flag
        if (send_msg->last){
                *send_flg &= ~(*send_flg);
                printf("client send out last packet!\n");
        }
        return 0;
}

int udp_rec_msg(struct sockaddr_in* srcaddr, int sockfd, message *rev_msg, int *recv_flg, FILE* writefd){
        int n;

        //received server file content
        socklen_t sendlen = sizeof(struct sockaddr_in);
        if ((n = recvfrom(sockfd, (message*) rev_msg, sizeof(message),
                        0, (struct sockaddr *) srcaddr, 
                        &sendlen)) < 0) {
                perror("Recvfrom error!\n");
                return -1;
        }
        //take care of the network order transfer 
        packet_process(rev_msg);

        printf("Receive packet! bytes: %d\n", n);
        hexDump("packet", rev_msg, sizeof(message));
        packet_dump(rev_msg); 
        if (rev_msg->port != SEVPORT)
                return 1;
                
        //last packet is a empty packet
        if (rev_msg->last){
                printf("client received last packet from server!\n"); 
                *recv_flg &= ~(*recv_flg);
        }
        else {
                //write received msg into file 
                if (writefile(rev_msg, writefd) == 0){
                        // write file error 
                        perror("cannot wite to file! Abort!\n");
                        return -1;  
                }
        }
        return 0;
}

void hexDump (char *desc, void *addr, int len) {
#ifdef DEBUG
        int i;
        unsigned char buff[17];
        unsigned char *pc = (unsigned char*)addr;

        // Output description if given.
        if (desc != NULL)
        printf ("%s:\n", desc);

        if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
        }
        if (len < 0) {
        printf("  NEGATIVE LENGTH: %i\n",len);
        return;
        }

        // Process every byte in the data.
        for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
        }

        // Pad out last line if not exactly 16 characters.
        while ((i % 16) != 0) {
        printf ("   ");
        i++;
        }

        // And print the final ASCII bit.
        printf ("  %s\n", buff);
#endif
}

void packet_process(message* msg){
        msg->last = ntohs(msg->last);
        msg->port = ntohs(msg->port);
        msg->len = ntohs(msg->len);
        msg->ip = ntohl(msg->ip);
        msg->id = ntohl(msg->id);
}
