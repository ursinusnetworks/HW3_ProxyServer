#ifndef UTIL_H
#define UTIL_H

#include <sys/socket.h>

#define BUFSIZE 1024

/**
 * @brief Send a message over a socket in chunks until
 * the entire message is sent
 * 
 * @param buff ArrayList buffer containing the message to send
 * @param N number of bytes to send
 * @param sockfd Socket to which to write
 * @return ssize_t Number of bytes sent, or -1 if error
 */
ssize_t sendMessage(char* buff, ssize_t N, ssize_t sockfd) {
    ssize_t sentLen = 0;
    ssize_t thisLen = 0;
    do {
        thisLen = send(sockfd, buff+sentLen, N-sentLen, 0);
        sentLen += thisLen;
    }
    while(sentLen < N && thisLen != -1);
    if (thisLen == -1) {
        sentLen = -1; // There was an error
    }
    return sentLen;
}


int receiveChunk(ArrayListBuf* abuf, int sockfd) {
    char miniBuff[BUFSIZE];
    int recvLen = 0;
    int finished = 0;
    do {
        int thisLen = recv(sockfd, miniBuff, BUFSIZE, 0);
        if (thisLen <= 0) {
            finished = 1;
        }
        else {
            ArrayListBuf_push(abuf, miniBuff, thisLen);
            recvLen += thisLen;
        }
        
    }
    while (finished == 0);
    return recvLen;
}


#endif