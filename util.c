#include <sys/socket.h>
#include <string.h>
#include "util.h"
#include "arraylist.h"
#include "md5-c/md5.h"

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


/**
 * recv into an ArrayList buffer until the connection closes
 *
 * @param abuf Buffer in which to accumulate received chunks
 * @param sockfd Socket ID on which to receive
 * 
 * @return Number of bytes received
 */
int receiveChunkUntilClose(ArrayListBuf* abuf, int sockfd) {
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

/**
 * Compute the md5 hash of a string using Zunawe's submodule:
 * https://github.com/Zunawe/md5-c
 *
 * @param s String to encode
 * @param res Result to which to save 32 character hash string 
 * (should be at least 33 big)
 */
void bytes2md5String(char* s, char* res) {
    uint8_t bytes[16];
    md5String(s, bytes);
    for (int i = 0; i < 16; i++) {
        sprintf(&res[i*2], "%02x", bytes[i]);
    }
}

/**
 * Parse a date in the format <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
 * @param datestr String data
 * @param date HTTP Date format to which to write the result
 * 
 * @return 0 if success, -1 otherwise
 */
int parseDate(char* datestr, struct HTTPDate* date) {
    char dayname[4];
    char monthname[4];
    int ret = 0;

    int nargs = sscanf(datestr, "%3s, %i %3s %i %i:%i:%i", dayname, &date->day, monthname, &date->year, &date->hour, &date->minute, &date->second);
    if (nargs < 7) {
        ret = -1;
    }
    else {
        if (strcmp(monthname, "Jan") == 0) {
            date->month = 1;
        }
        else if (strcmp(monthname, "Feb") == 0) {
            date->month = 2;
        }
        else if (strcmp(monthname, "Mar") == 0) {
            date->month = 3;
        }
        else if (strcmp(monthname, "Apr") == 0) {
            date->month = 4;
        }
        else if (strcmp(monthname, "May") == 0) {
            date->month = 5;
        }
        else if (strcmp(monthname, "Jun") == 0) {
            date->month = 6;
        }
        else if (strcmp(monthname, "Jul") == 0) {
            date->month = 7;
        }
        else if (strcmp(monthname, "Aug") == 0) {
            date->month = 8;
        }
        else if (strcmp(monthname, "Sep") == 0) {
            date->month = 9;
        }
        else if (strcmp(monthname, "Oct") == 0) {
            date->month = 10;
        }
        else if (strcmp(monthname, "Nov") == 0) {
            date->month = 11;
        }
        else if (strcmp(monthname, "Dec") == 0) {
            date->month = 12;
        }
    }
    return ret;
}

/**
 * Check to see if a file exists
 *
 * @param filename Path to file
 * @return 1 if a file exists, or 0 otherwise
 */
int fileExists(char* filename) {
    int ret = 0;
    FILE* file;
    if ((file = fopen(filename, "r"))) {
        fclose(file);
        ret = 1;
    }
    return ret;
}