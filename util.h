#ifndef UTIL_H
#define UTIL_H

#include "arraylist.h"
/**
 * @brief Send a message over a socket in chunks until
 * the entire message is sent
 * 
 * @param buff ArrayList buffer containing the message to send
 * @param N number of bytes to send
 * @param sockfd Socket to which to write
 * @return ssize_t Number of bytes sent, or -1 if error
 */
ssize_t sendMessage(char* buff, ssize_t N, ssize_t sockfd);


/**
 * recv into an ArrayList buffer until the connection closes
 *
 * @param abuf Buffer in which to accumulate received chunks
 * @param sockfd Socket ID on which to receive
 * 
 * @return Number of bytes received
 */
int receiveChunkUntilClose(ArrayListBuf* abuf, int sockfd);

/**
 * Compute the md5 hash of a string using Zunawe's submodule:
 * https://github.com/Zunawe/md5-c
 *
 * @param s String to encode
 * @param res Result to which to save 32 character hash string 
 * (should be at least 33 big)
 */
void bytes2md5String(char* s, char* res);


struct HTTPDate {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
};

/**
 * Parse a date in the format <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
 * @param datestr String data
 * @param date HTTP Date format to which to write the result
 * 
 * @return 0 if success, -1 otherwise
 */
int parseDate(char* datestr, struct HTTPDate* date);

/**
 * Check to see if a file exists
 *
 * @param filename Path to file
 * @return 1 if a file exists, or 0 otherwise
 */
int fileExists(char* filename);

#endif